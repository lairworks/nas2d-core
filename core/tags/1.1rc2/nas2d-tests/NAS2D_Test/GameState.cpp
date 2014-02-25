#include "GameState.h"


const int GUN_DELAY_TIME = 210;
const int GUN_JITTER = 10;
const int GUN_HALF_JITTER = GUN_JITTER / 2;

#include <iostream>
#include <sstream>

using namespace std;
using namespace NAS2D;


const unsigned int	ZOMBIE_DEAD_TIMEOUT		= 8000; // Time, in miliseconds, a dead zombie should continue to exist


GameState::GameState(): mPlayerPosition(Utility<Renderer>::get().screenCenterX(), Utility<Renderer>::get().screenCenterY()),
						mZombieSpawnCount(5),
						mFont("fonts/tork.ttf", 15),
						//mFont("fonts/geebeeyay_8x16.png", 8, 16, 0),
						mAnnounceFont("fonts/zombified.ttf", 50),
						mPointer("pointer.png"),
						mBackground("grass_bg.png"),
						mBulletHole("bullet_hole.png"),
						mTent("tent.png"),
						mTentShadow("tent_shadow.png"),
						mBgMusic("music/clearside-shapeshifter.ogg"),
						mGunFire("sfx/machine_gun.wav"),
						mTimeDelta(0),
						mLeftButtonDown(false)
{
}


void GameState::initialize()
{
	Renderer& r = Utility<Renderer>::get();

	spawnSwarm();

	EventHandler& e = Utility<EventHandler>::get();
	e.keyUp().Connect(this, &GameState::onKeyUp);
	e.keyDown().Connect(this, &GameState::onKeyDown);
	e.mouseMotion().Connect(this, &GameState::onMouseMove);
	e.mouseButtonUp().Connect(this, &GameState::onMouseUp);
	e.mouseButtonDown().Connect(this, &GameState::onMouseDown);
	e.quit().Connect(this, &GameState::onQuit);

	Utility<Mixer>::get().playMusic(mBgMusic);
}


State* GameState::update()
{
	Renderer& r = Utility<Renderer>::get();
	r.drawImage(mBackground, 0, 0);
	
	updateTimer();
	updatePlayer();
	updateZombies();


	// Tent shadow and base
	r.drawImage(mTentShadow, mPlayerPosition.x() - 256, mPlayerPosition.y() - 100);
	r.drawSubImage(mTent, mPlayerPosition.x() - 128, mPlayerPosition.y() + 0, 0, 162, 256, 94);


	if(mLeftButtonDown)
		r.drawLine(mPlayerPosition.x(), mPlayerPosition.y(), mBulletPoint.x(), mBulletPoint.y(), COLOR_WHITE, 1);


	// Tent top
	r.drawSubImage(mTent, mPlayerPosition.x() - 128, mPlayerPosition.y() - 70, 0, 0, 256, 139);


	r.drawImage(mPointer, mMouseCoords.x() - 7, mMouseCoords.y() - 7);

	/**
	r.drawText(mAnnounceFont, "Zombies are Coming!", r.screenCenterX() - mAnnounceFont.width("Zombies are Coming!") / 2, 10, 255, 255, 255);
	r.drawText(mFont, "Defend Yourself!", r.screenCenterX() - mFont.width("Defend Yourself!") / 2, 75, 255, 255, 255);
	*/
    r.drawTextClamped(mFont, "This is a test!", 10, 140, 0, 0, 50, 100, 255, 255, 255);

	std::stringstream str;
	str << "FPS: " << mFps.fps();
	r.drawText(mFont, str.str(), 10, 100, 255, 255, 255);

	str.str("");
	str << "Zombies: " << mZombies.size();
	r.drawText(mFont, str.str(), 10, 120, 255, 255, 255);

	return this;
}


void GameState::doShoot()
{
	Renderer& r = Utility<Renderer>::get();

	mBulletPoint((mMouseCoords.x() - mBulletHole.width() / 2) + Random::ranged_integer(-GUN_HALF_JITTER, GUN_JITTER), (mMouseCoords.y() -  mBulletHole.height() / 2) + Random::ranged_integer(-GUN_HALF_JITTER, GUN_JITTER));

	Utility<Mixer>::get().playSound(mGunFire);

	for(size_t i = 0; i < mZombies.size(); i++)
	{
		if(mZombies[i].hit(mBulletPoint))
		{
			mZombies[i].damage(10, mBulletPoint);
			if(mZombies[i].dead())
			{
				mDeadZombies.push_back(mZombies[i]);
				mZombies.erase(mZombies.begin() + i);
			}

			return;
		}
	}

	r.drawImageToImage(mBulletHole, mBackground, mBulletPoint);
}


void GameState::updatePlayer()
{
	handlePlayerAction();
}



void GameState::handlePlayerAction()
{
	if(mLeftButtonDown)
	{
		while(mGunTimer.accumulator() >= GUN_DELAY_TIME)
		{
			mGunTimer.adjust_accumulator(GUN_DELAY_TIME);
			doShoot();
		}

		//Utility<Renderer>::get().drawLine(mPlayer.position().x, mPlayer.position().y, mBulletPoint.x, mBulletPoint.y, 255, 255, 255, 100, 3);
	}
}


/**
 * Spawns a group of zombies around the player.
 */
void GameState::spawnSwarm()
{
	for(size_t i = 0; i < mZombieSpawnCount; i++)
		mZombies.push_back(Zombie(0 + i * 200, -20, 15));

	//for(size_t i = 0; i < 500; i++)
	//	mZombies.push_back(Zombie(0 + i * 10, 0, 15));

	mZombieSpawnCount += 2;
}


void GameState::updateZombies()
{
	for(size_t i = 0; i < mDeadZombies.size(); i++)
	{
		mDeadZombies[i].update(0, mPlayerPosition);

		if(mDeadZombies[i].deadTime() >= ZOMBIE_DEAD_TIMEOUT)
			mDeadZombies.erase(mDeadZombies.begin() + i);
	}

	for(size_t i = 0; i < mZombies.size(); i++)
	{
		mZombies[i].update(mTimeDelta, mPlayerPosition);
	}
}


void GameState::onKeyDown(KeyCode key, KeyModifier mod, bool repeat)
{
	if(repeat)
		return;
}


void GameState::onKeyUp(KeyCode key, KeyModifier mod)
{
	if(key == KEY_ESCAPE)
		postQuitEvent();
}


void GameState::onMouseDown(MouseButton button, int x, int y)
{
	if(button == BUTTON_LEFT)
	{
		mGunTimer.reset();
		mLeftButtonDown = true;
		doShoot();
	}
}


void GameState::onMouseUp(MouseButton button, int x, int y)
{
	if(button == BUTTON_LEFT)
	{
		mLeftButtonDown = false;
	}
}


void GameState::onMouseMove(int x, int y, int relX, int relY)
{
	mMouseCoords(x, y);
}


void GameState::updateTimer()
{
	mTimeDelta = mTimer.delta();
}


void GameState::onQuit()
{
	cout << "Toodles!" << endl;
}