#include "TestGraphics.h"

#include <NAS2D/EnumKeyCode.h>
#include <NAS2D/Utility.h>
#include <NAS2D/Filesystem.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/Resource/Font.h>
#include <NAS2D/Math/Rectangle.h>

#include <functional>
#include <random>


namespace
{
	std::mt19937 generator;


	NAS2D::Font getFont()
	{
		static const std::string fileName = "../../../data/fonts/opensans.ttf";

		const auto& filesystem = NAS2D::Utility<NAS2D::Filesystem>::get();
		if (filesystem.exists(fileName))
		{
			return NAS2D::Font{fileName, 16};
		}
		return NAS2D::Font::null();
	}


	void drawBoundedText(NAS2D::Renderer& renderer, NAS2D::Point<int> position, const std::string& text)
	{
		static const NAS2D::Font font = getFont();

		renderer.drawText(font, text, position);
		const auto textSize = font.size(text);
		const auto boxRect = NAS2D::Rectangle{position, textSize}.inset(-1);
		renderer.drawBox(boxRect);
	}
}


TestGraphics::TestGraphics() :
	mDxImage{"Test_DirectX.png"},
	mOglImage{"Test_OpenGL.png"}
{}

TestGraphics::~TestGraphics()
{
	auto& eventHandler = NAS2D::Utility<NAS2D::EventHandler>::get();
	eventHandler.mouseMotion().disconnect({this, &TestGraphics::onMouseMove});
	eventHandler.mouseButtonDown().disconnect({this, &TestGraphics::onMouseDown});
	eventHandler.keyDown().disconnect({this, &TestGraphics::onKeyDown});

}

void TestGraphics::initialize()
{
	auto& eventHandler = NAS2D::Utility<NAS2D::EventHandler>::get();
	eventHandler.mouseMotion().connect({this, &TestGraphics::onMouseMove});
	eventHandler.mouseButtonDown().connect({this, &TestGraphics::onMouseDown});
	eventHandler.keyDown().connect({this, &TestGraphics::onKeyDown});

	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
	renderer.showSystemPointer(true);
	const auto minSize = NAS2D::Vector{10 + 1024 + 10, 134 + 512 + 10};
	renderer.minimumSize(minSize);
	renderer.size(minSize);
}

NAS2D::State* TestGraphics::update()
{
	NAS2D::Renderer& r = NAS2D::Utility<NAS2D::Renderer>::get();

	r.clearScreen(NAS2D::Color::Gray);

	r.drawBox({{10, 10}, {40, 40}});
	r.drawBoxFilled({{70, 10}, {40, 40}}, NAS2D::Color{200, 0, 0});

	for (auto i = 0; i < 10; ++i)
	{
		NAS2D::Rectangle<int> boxRect = {{120 + 10 * i, 10}, {i, i}};
		r.drawBox(boxRect, NAS2D::Color::Red);
		r.drawBoxFilled(boxRect.inset(1), NAS2D::Color::White);
	}

	r.drawCircle({250, 30}, 20, NAS2D::Color{0, 200, 0, 255}, 16);
	r.drawCircle({290, 30}, 20, NAS2D::Color{0, 200, 0, 255}, 16, {0.5f, 0.5f});
	r.drawCircle({330, 30}, 20, NAS2D::Color{0, 200, 0, 255}, 16, {1.0f, 0.5f});

	drawBoundedText(r, {360, 10}, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	drawBoundedText(r, {360, 34}, "abcdefghijklmnopqrstuvwxyz");
	drawBoundedText(r, {360, 58}, "WWWW");
	drawBoundedText(r, {360, 82}, "iii");
	drawBoundedText(r, {360, 106}, " ");

	drawBoundedText(r, {630, 10}, "A\nmulti\nline\nstring.");

	r.drawGradient({{10, 60}, {64, 64}}, NAS2D::Color::Blue, NAS2D::Color::Green, NAS2D::Color::Red, NAS2D::Color::Magenta);

	for (auto i = 0u; i < 2000u; ++i)
	{
		std::uniform_int_distribution<int> jitterDistribution(0, 63);
		auto jitter = [&jitterDistribution](){ return jitterDistribution(generator); };

		const uint8_t grey = static_cast<uint8_t>(jitter()) * 2u + 100u;
		const auto offset = NAS2D::Vector{jitter(), jitter()};
		r.drawPoint(NAS2D::Point{84, 60} + offset, NAS2D::Color{grey, grey, grey});
	}

	r.drawImage(mDxImage, {10, 134});
	r.drawImage(mOglImage, {10 + 512, 134});

	return this;
}

void TestGraphics::onKeyDown(NAS2D::KeyCode key, NAS2D::KeyModifier /*mod*/, bool /*repeat*/)
{
	switch (key)
	{
	case NAS2D::KeyCode::Escape:
		NAS2D::postQuitEvent();
		break;
	case NAS2D::KeyCode::F1:
	{
		auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
		renderer.fullscreen(!renderer.fullscreen());
		break;
	}
	case NAS2D::KeyCode::F2:
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

void TestGraphics::onMouseDown(NAS2D::MouseButton /*button*/, NAS2D::Point<int> /*position*/)
{}

void TestGraphics::onWindowResized(int /*w*/, int /*h*/)
{}
