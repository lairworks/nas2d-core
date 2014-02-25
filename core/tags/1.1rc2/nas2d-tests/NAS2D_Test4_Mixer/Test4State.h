#ifndef __TEST_4_STATE__
#define __TEST_4_STATE__

#include <NAS2D/NAS2D.h>

using namespace NAS2D;

class Test4State: public State
{
public:
	Test4State();

protected:
	void initialize();
	State* update();

	void onKeyDown(KeyCode key, KeyModifier mod, bool repeat);

private:

	Timer			mTimer;				/**< Caustics timer. */

	Font			mFont;				/**< Main font. */
	Font			mSmallFont;			/**< FPS font. */

	Image			mBackgroundImg;

	Music			mModule1;			/**<  */
	Music			mModule2;			/**<  */

	Rectangle_2d	mLeftVuRect;		/**<  */
	Rectangle_2d	mRightVuRect;		/**<  */

	int				mCurrentSelection;	/**< Current Track Selection; */
};

#endif