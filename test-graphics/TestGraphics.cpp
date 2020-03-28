#include "TestGraphics.h"

#include <functional>
#include <random>

static std::mt19937 generator;
static std::uniform_int_distribution<int> jitter_distribution(0, 64);

TestGraphics::TestGraphics() :
	mDxImage("Test_DirectX.png"),
	mOglImage("Test_OpenGL.png")
{}

TestGraphics::~TestGraphics()
{
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseMotion().disconnect(this, &TestGraphics::onMouseMove);
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseButtonDown().disconnect(this, &TestGraphics::onMouseDown);
	NAS2D::Utility<NAS2D::EventHandler>::get().keyDown().disconnect(this, &TestGraphics::onKeyDown);

}

void TestGraphics::initialize()
{
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseMotion().connect(this, &TestGraphics::onMouseMove);
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseButtonDown().connect(this, &TestGraphics::onMouseDown);
	NAS2D::Utility<NAS2D::EventHandler>::get().keyDown().connect(this, &TestGraphics::onKeyDown);

	NAS2D::Utility<NAS2D::Renderer>::get().showSystemPointer(true);
	NAS2D::Utility<NAS2D::Renderer>::get().minimum_size(1600, 900);
}

NAS2D::State* TestGraphics::update()
{
	NAS2D::Renderer& r = NAS2D::Utility<NAS2D::Renderer>::get();

	r.clearScreen(NAS2D::Color::Grey);

	r.drawImage(mDxImage, 256, 256.0f);
	r.drawImage(mOglImage, 768, 256.0f);

	{
		const auto jitter = [&]() -> decltype(auto) {
			std::uniform_int_distribution<int> dist(0, 64);
			return dist(generator);
		};
		for (int i = 0; i < 2000; ++i)
		{
			int grey = jitter() * 2;
			r.drawPoint(10.0f + jitter(), 250.0f + jitter(), 100 + grey, 100 + grey, 100 + grey);
		}
	}

	r.drawBox(10, 50, 40, 40, 255, 255, 255);
	r.drawBoxFilled(70, 50, 40, 40, 200, 0, 0);

	r.drawGradient(10, 100, 100, 100, NAS2D::Color::Blue, NAS2D::Color::Green, NAS2D::Color::Red, NAS2D::Color::Magenta);

	r.drawCircle(150, 70, 20, 0, 200, 0, 255, 16);
	r.drawCircle(150, 120, 20, 0, 200, 0, 255, 16, 0.5f);
	r.drawCircle(150, 170, 20, 0, 200, 0, 255, 16, 1.0f, 0.5f);

	return this;
}

void TestGraphics::onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat)
{
	switch (key)
	{
	case NAS2D::EventHandler::KeyCode::KEY_ESCAPE:
		NAS2D::postQuitEvent();
		break;
	case NAS2D::EventHandler::KeyCode::KEY_F1:
		NAS2D::Utility<NAS2D::Renderer>::get().fullscreen(!NAS2D::Utility<NAS2D::Renderer>::get().fullscreen());
		break;
	case NAS2D::EventHandler::KeyCode::KEY_F2:
		NAS2D::Utility<NAS2D::Renderer>::get().resizeable(!NAS2D::Utility<NAS2D::Renderer>::get().resizeable());
		break;
	default:
		break;
	}
}

void TestGraphics::onMouseMove(int x, int y, int relX, int relY)
{}

void TestGraphics::onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y)
{}

void TestGraphics::onWindowResized(int w, int h)
{}
