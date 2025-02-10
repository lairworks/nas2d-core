#include "TestGraphics.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/Math/Rectangle.h>

#include <functional>
#include <random>


namespace
{
	std::mt19937 generator;
	std::uniform_int_distribution<int> jitterDistribution(0, 64);


	auto jitter()
	{
		return jitterDistribution(generator);
	}
}


TestGraphics::TestGraphics() :
	mDxImage{"Test_DirectX.png"},
	mOglImage{"Test_OpenGL.png"}
{}

TestGraphics::~TestGraphics()
{
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseMotion().disconnect({this, &TestGraphics::onMouseMove});
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseButtonDown().disconnect({this, &TestGraphics::onMouseDown});
	NAS2D::Utility<NAS2D::EventHandler>::get().keyDown().disconnect({this, &TestGraphics::onKeyDown});

}

void TestGraphics::initialize()
{
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseMotion().connect({this, &TestGraphics::onMouseMove});
	NAS2D::Utility<NAS2D::EventHandler>::get().mouseButtonDown().connect({this, &TestGraphics::onMouseDown});
	NAS2D::Utility<NAS2D::EventHandler>::get().keyDown().connect({this, &TestGraphics::onKeyDown});

	NAS2D::Utility<NAS2D::Renderer>::get().showSystemPointer(true);
	NAS2D::Utility<NAS2D::Renderer>::get().minimumSize({1600, 900});
}

NAS2D::State* TestGraphics::update()
{
	NAS2D::Renderer& r = NAS2D::Utility<NAS2D::Renderer>::get();

	r.clearScreen(NAS2D::Color::Gray);

	r.drawBox({{10, 50}, {40, 40}});
	r.drawBoxFilled({{70, 50}, {40, 40}}, NAS2D::Color{200, 0, 0});

	r.drawGradient({{10, 100}, {100, 100}}, NAS2D::Color::Blue, NAS2D::Color::Green, NAS2D::Color::Red, NAS2D::Color::Magenta);

	r.drawCircle({150, 70}, 20, NAS2D::Color{0, 200, 0, 255}, 16);
	r.drawCircle({150, 120}, 20, NAS2D::Color{0, 200, 0, 255}, 16, {0.5f, 0.5f});
	r.drawCircle({150, 170}, 20, NAS2D::Color{0, 200, 0, 255}, 16, {1.0f, 0.5f});

	for (auto i = 0; i < 10; ++i)
	{
		NAS2D::Rectangle<int> boxRect = {{200 + 10 * i, 50}, {i, i}};
		r.drawBox(boxRect, NAS2D::Color::Red);
		r.drawBoxFilled(boxRect.inset(1), NAS2D::Color::White);
	}

	for (auto i = 0u; i < 2000u; ++i)
	{
		const uint8_t grey = static_cast<uint8_t>(jitter()) * 2u + 100u;
		r.drawPoint(NAS2D::Point{10 + jitter(), 250 + jitter()}, NAS2D::Color{grey, grey, grey});
	}

	r.drawImage(mDxImage, {256, 256});
	r.drawImage(mOglImage, {768, 256});

	return this;
}

void TestGraphics::onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier /*mod*/, bool /*repeat*/)
{
	switch (key)
	{
	case NAS2D::EventHandler::KeyCode::KEY_ESCAPE:
		NAS2D::postQuitEvent();
		break;
	case NAS2D::EventHandler::KeyCode::KEY_F1:
	{
		auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
		renderer.fullscreen(!renderer.fullscreen());
		break;
	}
	case NAS2D::EventHandler::KeyCode::KEY_F2:
	{
		auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
		renderer.resizeable(!renderer.resizeable());
		break;
	}
	default:
		break;
	}
}

void TestGraphics::onMouseMove(NAS2D::Point<int> /*position*/, NAS2D::Vector<int> /*change*/)
{}

void TestGraphics::onMouseDown(NAS2D::EventHandler::MouseButton /*button*/, NAS2D::Point<int> /*position*/)
{}

void TestGraphics::onWindowResized(int /*w*/, int /*h*/)
{}
