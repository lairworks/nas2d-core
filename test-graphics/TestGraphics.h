#pragma once

#include "NAS2D/State.h"
#include "NAS2D/EventHandler.h"
#include "NAS2D/Timer.h"
#include "NAS2D/Resource/Image.h"


class TestGraphics : public NAS2D::State
{
public:
	TestGraphics();
	~TestGraphics() override;

protected:
	void initialize() override;
	NAS2D::State* update() override;

	void onKeyDown(NAS2D::KeyCode key, NAS2D::KeyModifier mod, bool repeat);
	void onMouseMove(NAS2D::Point<int> position, NAS2D::Vector<int> change);
	void onMouseDown(NAS2D::MouseButton button, NAS2D::Point<int> position);

	void onWindowResized(int w, int h);

private:
	NAS2D::Timer mTimer;
	NAS2D::Image mGear;
	NAS2D::Image mDxImage;
	NAS2D::Image mOglImage;
};
