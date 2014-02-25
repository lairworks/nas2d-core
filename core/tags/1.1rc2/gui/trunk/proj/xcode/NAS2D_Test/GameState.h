#include <NAS2D/NAS2D.h>
#include <NAS2D_GUI/Gui.h>
#include <NAS2D_GUI/Controls.h>

#include <bitset>
#include <vector>


class GameState: public State
{
public:
	GameState();

protected:
	void initialize();
	State* update();

	// Keyhandling functions
	void onKeyDown(SDL_Event& event);
	void onKeyUp(SDL_Event& event);

	// Mouse Handling Functions
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
	void onMouseMotion(SDL_Event& event);

private:
	void doShoot();

	void updateTimer();

	void updatePlayer();
	void handlePlayerMove();
	void handlePlayerAction();

	Point_2d	mMouseCoords;
	Point_2d	mBulletPoint;

	Font		mFont;

	Image		mPointer;
	Image		mTileset;

	Image		mBackground;
	Image		mBulletHole;

	Image		mBloodSplat;

	Sound		mMachineGun;

	Timer		mTimer;

	int			mLastTick;
	int			mDeltaTime;

	int			mWeaponTimerCount;

	float		mPlayerX;
	float		mPlayerY;

	bool		mLeftButtonDown;

	bool		mPlayerMoving;
	
	Gui			*mGui;

	FpsCounter	mFps;
};
