#include "NAS2D/NAS2D.h"

class TestGraphics : public NAS2D::State
{
  public:
	TestGraphics();
	~TestGraphics();

  protected:
	void initialize();
	NAS2D::State* update();

	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);

	void onWindowResized(int w, int h);

  private:
	NAS2D::Timer mTimer;

	NAS2D::Image mDxImage;
	NAS2D::Image mOglImage;

};
