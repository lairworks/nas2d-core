#include "NAS2D/NAS2D.h"

using namespace NAS2D;

class Test2State: public State
{
public:
	Test2State();
	~Test2State();

protected:
	void initialize();
	State* update();


	void onKeyDown(KeyCode key, KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseDown(MouseButton button, int x, int y);

private:

	FpsCounter	mFps;

	Font		mFont;
	Image		mBackgroundImage;
	Image		mPointer;

	Point_2d	mMousePosition;

};
