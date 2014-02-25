#include <NAS2D/NAS2D.h>
//#include <NAS2D_GUI/Gui.h>
//#include <NAS2D_GUI/Controls.h>

#include "Zombie.h"

#include <bitset>
#include <vector>

class GameState: public State
{
public:
	GameState();

protected:
	void initialize();
	State* update();

	void onKeyUp(KeyCode key, KeyModifier mod);
	void onKeyDown(KeyCode key, KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseUp(MouseButton button, int x, int y);
	void onMouseDown(MouseButton button, int x, int y);
	
	void onQuit();

private:
	typedef std::vector<Zombie> ZombieList;

	void doShoot();

	void updateTimer();

	void updatePlayer();
	void handlePlayerMove();
	void handlePlayerAction();

	void spawnSwarm();
	void updateZombies();

	Timer		mTimer;				/**<  */
	Timer		mGunTimer;			/**<  */

	FpsCounter	mFps;				/**< FPS Counter */

	ZombieList	mZombies;			/**<  */
	ZombieList	mDeadZombies;		/**<  */
	size_t		mZombieSpawnCount;	/**< Number of zombies to spawn. */

	Font		mFont;				/**< Font used for interface and information text. */
	Font		mAnnounceFont;		/**< Font used for displaying the announcement. */

	Point_2d	mPlayerPosition;	/**< Position of the 'player'. */
	
	Point_2d	mMouseCoords;		/**< Mouse coordinates. */
	Point_2d	mBulletPoint;		/**<  */

	Image		mPointer;			/**< Pointer image. */
	Image		mBackground;		/**< Background buffer. */
	Image		mBulletHole;		/**< Bullet hole. */
	Image		mTent;				/**< Tent graphic. */
	Image		mTentShadow;		/**< Tent shadow graphic. */

	Music		mBgMusic;			/**< Background music track. */

	Sound		mGunFire;			/**< Gun fire sound. */

	int			mTimeDelta;			/**< Delta time between the previous cycle and the current cycle. */

	bool		mLeftButtonDown;	/**< Flag indicating that the left mouse button is down. */
};
