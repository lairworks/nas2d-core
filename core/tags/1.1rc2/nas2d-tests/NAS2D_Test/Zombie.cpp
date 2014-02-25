#include "Zombie.h"


const int RECT_MARGIN = 7;
const int RECT_MARGIN_2 = RECT_MARGIN * 2;

const Rectangle_2d		BOUNDING_BOX_BODY		= Rectangle_2d(-10, -40, 14, 46);
const Rectangle_2d		BOUNDING_BOX_HEAD		= Rectangle_2d(-7, -50, 8, 8);

const Rectangle_2d		HEALTH_METER			= Rectangle_2d(0, 0, 24, 4);

const std::string		SPRITE_PATH				= "zombie_0.xml";



const std::string		IDLE_WEST				= "WalkWest";



Zombie::Zombie(float x, float y, float speed):	mSprite(SPRITE_PATH),
												mPosition(x, y),
												mHealth(100),
												mMaxHealth(mHealth),
												mDirection(0.0f),
												mSpeed(speed),
												mSpeedModifier(0.0f),
												mBodyRect(BOUNDING_BOX_BODY),
												mHeadRect(BOUNDING_BOX_HEAD)
{
	mSprite.play(IDLE_WEST);
}


Zombie::Zombie(int x, int y, int speed):	mSprite(SPRITE_PATH),
											mPosition(static_cast<float>(x), static_cast<float>(y)),
											mHealth(100),
											mMaxHealth(mHealth),
											mDirection(0.0f),
											mSpeed(static_cast<float>(speed)),
											mSpeedModifier(0.0f),
											mBodyRect(BOUNDING_BOX_BODY),
											mHeadRect(BOUNDING_BOX_HEAD)
{
	mSprite.play(IDLE_WEST);
}


bool Zombie::hit(const Point_2d& pt)
{
	return isPointInRect(pt, mBodyRect) || isPointInRect(pt, mHeadRect);
}


unsigned int Zombie::deadTime()
{
	if(!dead())
		return 0;

	return mTimer.accumulator();
}

void Zombie::update(int timeDelta, const Point_2df& playerPosition)
{
	mSprite.update(mPosition.x(), mPosition.y());

	if(dead())
		return;

	// Ultra basic bee-line AI
	mDirection = angleFromPoints(mPosition.x(), mPosition.y(), playerPosition.x(), playerPosition.y());
	doMove(timeDelta);


	// Update bounding boxes.
	mBodyRect.x() = mPosition.x() + BOUNDING_BOX_BODY.x();
	mBodyRect.y() = mPosition.y() + BOUNDING_BOX_BODY.y();

	mHeadRect.x() = mPosition.x() + BOUNDING_BOX_HEAD.x();
	mHeadRect.y() = mPosition.y() + BOUNDING_BOX_HEAD.y();
	

	// Health bar
	Renderer& r = Utility<Renderer>::get();

	int startX = mPosition.x() - HEALTH_METER.w() / 2;
	int healthWidth = HEALTH_METER.w() * (static_cast<float>(mHealth) / static_cast<float>(mMaxHealth));
	
	r.drawBoxFilled(startX, mHeadRect.y() - 5, HEALTH_METER.w(), 2, 0, 0, 0);
	r.drawBoxFilled(startX, mHeadRect.y() - 5, healthWidth, 2, 255, 255, 0);

	r.drawBox(mHeadRect, 255, 255, 255);
	r.drawBox(mBodyRect, 255, 255, 255);
}


void Zombie::doMove(int timeDelta)
{
	Point_2df dir = getDirectionVector(mDirection);

	mPosition.x() += (dir.x() * (timeDelta / 1000.0f)) * mSpeed;
	mPosition.y() += (dir.y() * (timeDelta / 1000.0f)) * mSpeed;
}


void Zombie::setAnimationState()
{}


void Zombie::damage(int d, const Point_2d& pt)
{
	if(dead())
		return;

	if(!dead())
	{
		if(isPointInRect(pt, mHeadRect))
		{
			mHealth = 0;
			mSprite.play("Dead2West");
			mTimer.reset();
			return;
		}
		else
		{
			mHealth -= d;
		}
	}
	
	if(mHealth <= 0)
	{
		mHealth = 0;
		mSprite.play("Dead1West");
		mTimer.reset(); // reset timer so we know how long it's been since the zombie died.
	}
}