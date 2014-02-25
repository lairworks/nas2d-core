#include "GameState.h"


const int MACHINE_GUN_DELAY_TIME = 100;
const int MACHINE_GUN_JITTER = 25;
const int MACHINE_GUN_HALF_JITTER = MACHINE_GUN_JITTER / 2;

const float PLAYER_MOVE_SPEED = 1.0f;

#include <iostream>
#include <sstream>

#include <stdlib.h>
#include <time.h>

using namespace std;


GameState::GameState():	mMouseCoords(0, 0),
						mFont("tork.ttf"),
						mPointer("pointer.png"),
						mTileset("tileset.png"),
						mBackground("bg.bmp"),
						mBulletHole("bullet_hole.png"),
						mBloodSplat("spurt/spurt_02.png"),
						mMachineGun("machine_gun.wav"),
						mLastTick(0),
						mDeltaTime(0),
						mWeaponTimerCount(0),
						mPlayerX(Singleton<Renderer>::get().getScreenCenterX()),
						mPlayerY(Singleton<Renderer>::get().getScreenCenterY()),
						mLeftButtonDown(false),
						mPlayerMoving(false)
{
}


void GameState::initialize()
{
	Renderer& r = Singleton<Renderer>::get();
	EventHandler& e = Singleton<EventHandler>::get();
	mGui = new Gui();
	
	mGui->setSkinTheme("LoM Theme");
	mGui->setPointer(MOUSE_NORMAL);

	for(int row = 0; row < (r.getHeight() / 32) + 1; row++)
		for(int col = 0; col < r.getWidth() / 32; col++)
			r.drawImageToImage(mTileset, Rectangle_2d(64, 96, 32, 32), mBackground, Point_2d(col * 32, row * 32));

	srand(time(NULL));

	using namespace std::tr1;
	using namespace std::tr1::placeholders;
	
	Window *mWindow = new Window("Test Window", 50, 50, 125, 125);
	mGui->add(mWindow);

	e.event(SDL_MOUSEMOTION).connect(this, &GameState::onMouseMotion);
	e.event(SDL_MOUSEBUTTONDOWN).connect(this, &GameState::onMouseButtonDown);
	e.event(SDL_MOUSEBUTTONUP).connect(this, &GameState::onMouseButtonUp);
}


State* GameState::update()
{
	Renderer& r = Singleton<Renderer>::get();

	r.drawImage(mBackground, 0, 0);

	r.drawCircle(mPlayerX, mPlayerY, 16, 255, 255, 0, 255, 8);

	stringstream str;
	float direction = angleFromPoints(mPlayerX, mPlayerY, mMouseCoords.x, mMouseCoords.y);
	str << direction;

	r.drawText(mFont, str.str(), 5, 5, 255, 255, 255);

	str.str("");
	str << mFps.getFps();

	r.drawText(mFont, str.str(), 5, mFont.getHeight() + 1, 255, 255, 255);

	updateTimer();

	updatePlayer();

	r.drawImage(mPointer, mMouseCoords.x - 16, mMouseCoords.y - 16);

	//handler().pump();
	mGui->update();

	return this;
}


void GameState::doShoot()
{
	Renderer& r = Singleton<Renderer>::get();

	int jitterX = (rand() % MACHINE_GUN_JITTER) - MACHINE_GUN_HALF_JITTER;
	int jitterY = (rand() % MACHINE_GUN_JITTER) - MACHINE_GUN_HALF_JITTER;

	mBulletPoint = Point_2d((mMouseCoords.x - mBulletHole.getWidth() / 2) + jitterX, (mMouseCoords.y -  mBulletHole.getHeight() / 2) + jitterY);

	r.drawImageToImage(mBulletHole, mBulletHole.getRect(), mBackground, mBulletPoint);

	Singleton<Mixer>::get().playSound(mMachineGun);
}


void GameState::updatePlayer()
{
	handlePlayerMove();
	handlePlayerAction();
}


void GameState::handlePlayerMove()
{
	if(!mPlayerMoving)
		return;


}


void GameState::handlePlayerAction()
{
	if(mLeftButtonDown)
	{
		mWeaponTimerCount += mDeltaTime;

		if(mWeaponTimerCount >= MACHINE_GUN_DELAY_TIME)
		{
			mWeaponTimerCount -= MACHINE_GUN_DELAY_TIME;
			if(mWeaponTimerCount < 0)
				mWeaponTimerCount = 0;

			doShoot();
		}

		Singleton<Renderer>::get().drawLine(mPlayerX, mPlayerY, mBulletPoint.x, mBulletPoint.y);
	}
}


void GameState::onKeyDown(SDL_Event& event)
{
	switch(event.key.keysym.sym)
	{
		case SDLK_w:
		case SDLK_UP:
			mPlayerMoving = true;
			break;

		default:
			break;
	}
}


void GameState::onKeyUp(SDL_Event& event)
{
	switch(event.key.keysym.sym)
	{
		case SDLK_w:
		case SDLK_UP:
			mPlayerMoving = false;
			break;

		default:
			break;
	}
}


void GameState::onMouseButtonDown(SDL_Event& event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		mLeftButtonDown = true;
		doShoot();

		Singleton<Renderer>::get().drawImageToImage(mBloodSplat, mBloodSplat.getRect(), mBackground, Point_2d(event.button.x, event.button.y));
	}
}


void GameState::onMouseButtonUp(SDL_Event& event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		mLeftButtonDown = false;

		// Reset the weapon timer
		mWeaponTimerCount = 0;
	}
}


void GameState::onMouseMotion(SDL_Event& event)
{
	mMouseCoords.x = event.motion.x;
	mMouseCoords.y = event.motion.y;
}


void GameState::updateTimer()
{
	int tick = mTimer.ms();

	mDeltaTime = tick - mLastTick;
	mLastTick = tick;
}