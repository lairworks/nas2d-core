#include <NAS2D/NAS2D.h>

using namespace NAS2D;

class Test3State: public State
{
public:
	Test3State();

protected:
	void initialize();
	State* update();

	void onKeyDown(KeyCode key, KeyModifier mod, bool repeat);

private:

	Timer		mCausticsTimer;		/**< Caustics timer. */

	FpsCounter	mFps;				/**< FPS Counter */

	Font		mFont;				/**< Main font. */
	Font		mSmallFont;			/**< FPS font. */

	Image		mMud;				/**< Background image. */
	Image		mCaustics;			/**< Caustics image atlas. */
	Image		mCaustics2;			/**< Caustics image atlas with no alpha. */

	Image		mCaustics3;			/**< Caustics image atlas. */
	Image		mCaustics4;			/**< Caustics image atlas with no alpha. */

	bool		mAlpha;
	bool		mMultiply;			/**< Flag to indicate that we want to draw screen blend mode. */
	bool		mCausticsOnly;		/**< Flag to indicate that we only want to show the caustics. */

	bool		mAlpha2;
	bool		mMultiply2;
	bool		mCausticsOnly2;
};
