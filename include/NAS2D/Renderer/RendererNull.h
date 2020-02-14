// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "Renderer.h"

namespace NAS2D {

class RendererNull : public Renderer
{
public:
	RendererNull() = default;

	~RendererNull() override {}

	std::vector<DisplayDesc> getDisplayModes() const override { return {}; }
	DisplayDesc getClosestMatchingDisplayMode(const DisplayDesc&) const override { return{}; };

	void drawImage(Image&, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}

	void drawSubImage(Image&, float, float, float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}
	void drawSubImageRotated(Image&, float, float, float, float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}

	void drawImageRotated(Image&, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t, float) override {}
	void drawImageStretched(Image&, float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}

	void drawImageRepeated(Image&, float, float, float, float) override {}
	void drawSubImageRepeated(Image&, float, float, float, float, float, float, float, float) override {}

	void drawImageToImage(Image&, Image&, const Point_2df&) override {}

	void drawPoint(float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}
	void drawLine(float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t, int) override {}
	void drawBox(float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}
	void drawBoxFilled(float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}
	void drawCircle(float, float, float, uint8_t, uint8_t, uint8_t, uint8_t, int, float, float) override {}

	void drawGradient(float, float, float, float, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t) override {}

	void drawText(NAS2D::Font&, const std::string&, float, float, uint8_t, uint8_t, uint8_t, uint8_t) override {}

	void showSystemPointer(bool) override {}
	void addCursor(const std::string&, int, int, int) override {}
	void setCursor(int) override {}

	void clearScreen(uint8_t, uint8_t, uint8_t) override {}

	float width() override { return 0.0f; }
	float height() override { return 0.0f; }

	void size(int, int) override {}
	void minimum_size(int, int) override {}

	void fullscreen(bool, bool = false) override {}
	bool fullscreen() override { return false; }

	void resizeable(bool) override {}
	bool resizeable() override { return false; }

	void clipRect(float, float, float, float) final override {}

	void window_icon(const std::string&) override {}

	void update() override {}
};

} // namespace
