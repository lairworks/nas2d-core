#include "Player.h"

const float PLAYER_MOVE_SPEED = 75.0f;


void Player::update(int timeDelta)
{
	doMove(timeDelta);
	mSprite.update(mPosition.x(), mPosition.y());
}


void Player::doMove(int timeDelta)
{

	float deltaX = (timeDelta / 1000.0f) * PLAYER_MOVE_SPEED;
	float deltaY = (timeDelta / 1000.0f) * PLAYER_MOVE_SPEED;


	if(mMovingForward)
	{
		mPosition.y() -= deltaY;
	}

	if(mMovingBackward)
	{
		mPosition.y() += deltaY;
	}

	if(mStrafingLeft)
	{
		mPosition.x() -= deltaX;
	}

	if(mStrafingRight)
	{
		mPosition.x() += deltaX;
	}

	/*
	Vec_2f dir = getDirectionVector(mSprite.rotation());

	float deltaX = (dir.x * (timeDelta / 1000.0f)) * PLAYER_MOVE_SPEED;
	float deltaY = (dir.y * (timeDelta / 1000.0f)) * PLAYER_MOVE_SPEED;

	if(mMovingForward)
	{
		mPosition.x += deltaX;
		mPosition.y += deltaY;
	}

	if(mMovingBackward)
	{
		mPosition.x -= deltaX;
		mPosition.y -= deltaY;
	}

	if(mStrafingLeft)
	{

		mPosition.x += deltaY;
		mPosition.y -= deltaX;
	}

	if(mStrafingRight)
	{

		mPosition.x -= deltaY;
		mPosition.y += deltaX;
	}
	*/

	// Cast the screen dimensions to force use of the float version of clamp();
	mPosition.x() = clamp(mPosition.x(), 0.0f, static_cast<float>(Utility<Renderer>::get().width()));
	mPosition.y() = clamp(mPosition.y(), 0.0f, static_cast<float>(Utility<Renderer>::get().height()));
}


void Player::onKeyDown(KeyCode key, KeyModifier mod)
{
	switch(key)
	{
		case KEY_w:
		case KEY_UP:
			mMovingForward = true;
			mMovingBackward = false;
			setAnimationState();
			break;

		case KEY_a:
		case KEY_LEFT:
			mStrafingLeft = true;
			mStrafingRight = false;
			setAnimationState();
			break;

		case KEY_d:
		case KEY_RIGHT:
			mStrafingRight = true;
			mStrafingLeft = false;
			setAnimationState();
			break;

		case KEY_s:
		case KEY_DOWN:
			mMovingBackward = true;
			mMovingForward = false;
			setAnimationState();
			break;

		default:
			break;
	}
}


void Player::onKeyUp(KeyCode key, KeyModifier mod)
{
	switch(key)
	{
		case KEY_w:
		case KEY_UP:
			mMovingForward = false;
			setAnimationState();
			break;

		case KEY_a:
		case KEY_LEFT:
			mStrafingLeft = false;
			setAnimationState();
			break;

		case KEY_d:
		case KEY_RIGHT:
			mStrafingRight = false;
			setAnimationState();
			break;

		case KEY_s:
		case KEY_DOWN:
			mMovingBackward = false;
			setAnimationState();
			break;

		default:
			break;
	}
}


void Player::setAnimationState()
{
	if(mShootingWeapon)
		mSprite.play("Shoot");
	else if(mMovingForward || mMovingBackward  || mStrafingLeft  || mStrafingRight)
		mSprite.play("Walk");
	else
		mSprite.play("Idle");
}


void Player::onMouseMotion(int x, int y, int relX, int relY)
{
	mSprite.rotation(angleFromPoints(mPosition.x(), mPosition.y(), static_cast<float>(x), static_cast<float>(y)));
}


void Player::onMouseUp(MouseButton button, int x, int y)
{
	if(button == BUTTON_LEFT)
	{
		mShootingWeapon = false;
		setAnimationState();
	}
}


void Player::onMouseDown(MouseButton button, int x, int y)
{
	if(button == BUTTON_LEFT)
	{
		mShootingWeapon = true;
		setAnimationState();
	}
}
