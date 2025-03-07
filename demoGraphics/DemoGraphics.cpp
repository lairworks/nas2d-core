#include "DemoGraphics.h"

#include <NAS2D/EnumKeyCode.h>
#include <NAS2D/Utility.h>
#include "NAS2D/EventHandler.h"
#include <NAS2D/Filesystem.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/Resource/Font.h>
#include <NAS2D/Math/Rectangle.h>
#include "NAS2D/Math/Angle.h"

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


DemoGraphics::DemoGraphics() :
	mGear{"Gear.png"},
	mDxImage{"Test_DirectX.png"},
	mOglImage{"Test_OpenGL.png"},
	mTimer{}
{}

DemoGraphics::~DemoGraphics()
{
	auto& eventHandler = NAS2D::Utility<NAS2D::EventHandler>::get();
	eventHandler.mouseMotion().disconnect({this, &DemoGraphics::onMouseMove});
	eventHandler.mouseButtonDown().disconnect({this, &DemoGraphics::onMouseDown});
	eventHandler.keyDown().disconnect({this, &DemoGraphics::onKeyDown});

}

void DemoGraphics::initialize()
{
	auto& eventHandler = NAS2D::Utility<NAS2D::EventHandler>::get();
	eventHandler.mouseMotion().connect({this, &DemoGraphics::onMouseMove});
	eventHandler.mouseButtonDown().connect({this, &DemoGraphics::onMouseDown});
	eventHandler.keyDown().connect({this, &DemoGraphics::onKeyDown});

	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
	renderer.showSystemPointer(true);
	const auto minSize = NAS2D::Vector{10 + 1024 + 10, 134 + 512 + 10};
	renderer.minimumSize(minSize);
	renderer.size(minSize);
}

NAS2D::State* DemoGraphics::update()
{
	NAS2D::Renderer& renderer = NAS2D::Utility<NAS2D::Renderer>::get();

	renderer.clearScreen(NAS2D::Color::Gray);

	renderer.drawBox({{10, 10}, {40, 40}});
	renderer.drawBoxFilled({{70, 10}, {40, 40}}, NAS2D::Color{200, 0, 0});

	for (auto i = 0; i < 10; ++i)
	{
		NAS2D::Rectangle<int> boxRect = {{120 + 10 * i, 10}, {i, i}};
		renderer.drawBox(boxRect, NAS2D::Color::Red);
		renderer.drawBoxFilled(boxRect.inset(1), NAS2D::Color::White);
	}

	renderer.drawCircle({250, 30}, 20, NAS2D::Color{0, 200, 0, 255}, 16);
	renderer.drawCircle({290, 30}, 20, NAS2D::Color{0, 200, 0, 255}, 16, {0.5f, 0.5f});
	renderer.drawCircle({330, 30}, 20, NAS2D::Color{0, 200, 0, 255}, 16, {1.0f, 0.5f});

	drawBoundedText(renderer, {360, 10}, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	drawBoundedText(renderer, {360, 34}, "abcdefghijklmnopqrstuvwxyz");
	drawBoundedText(renderer, {360, 58}, "WWWW");
	drawBoundedText(renderer, {360, 82}, "iii");
	drawBoundedText(renderer, {360, 106}, " ");

	drawBoundedText(renderer, {630, 10}, "A\nmulti\nline\nstring.");

	renderer.drawGradient({{10, 60}, {64, 64}}, NAS2D::Color::Blue, NAS2D::Color::Green, NAS2D::Color::Red, NAS2D::Color::Magenta);

	for (auto i = 0u; i < 2000u; ++i)
	{
		std::uniform_int_distribution<int> jitterDistribution(0, 63);
		auto jitter = [&jitterDistribution](){ return jitterDistribution(generator); };

		const uint8_t grey = static_cast<uint8_t>(jitter()) * 2u + 100u;
		const auto offset = NAS2D::Vector{jitter(), jitter()};
		renderer.drawPoint(NAS2D::Point{84, 60} + offset, NAS2D::Color{grey, grey, grey});
	}

	const auto angle = NAS2D::Angle::degrees(static_cast<float>(mTimer.tick() * 360 / 10 / 1000));
	renderer.drawImageRotated(mGear, {158, 60}, angle);
	renderer.drawImageRotated(mGear, {219, 60}, -angle);

	renderer.drawSubImage(mGear, {284, 60}, {{32, 0}, {32, 32}});
	renderer.drawSubImageRotated(mGear, {320, 60}, {{0, 0}, {32, 32}}, angle);
	renderer.drawSubImageRepeated(mGear, {{284, 98}, {60, 26}}, {{16, 0}, {16, 16}});

	renderer.drawImage(mDxImage, {10, 134});
	renderer.drawImage(mOglImage, {10 + 512, 134});

	return this;
}

void DemoGraphics::onKeyDown(NAS2D::KeyCode key, NAS2D::KeyModifier /*mod*/, bool /*repeat*/)
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

void DemoGraphics::onMouseMove(NAS2D::Point<int> /*position*/, NAS2D::Vector<int> /*change*/)
{}

void DemoGraphics::onMouseDown(NAS2D::MouseButton /*button*/, NAS2D::Point<int> /*position*/)
{}

void DemoGraphics::onWindowResized(int /*w*/, int /*h*/)
{}
