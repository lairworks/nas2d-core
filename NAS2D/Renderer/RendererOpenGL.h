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


struct SDL_Cursor;


namespace NAS2D {

/**
 * \class RendererOpenGL
 * \brief OpenGL Renderer.
 *
 * Implements an OpenGL based Renderer.
 */
class RendererOpenGL: public Renderer
{
public:
	struct Options
	{
		Vector<int> resolution;
		bool fullscreen;
		bool vsync;
	};

	RendererOpenGL() = delete;
	explicit RendererOpenGL(const std::string& title);
	RendererOpenGL(const std::string& title, const Options& options);
	RendererOpenGL(const RendererOpenGL& other) = delete;
	RendererOpenGL(RendererOpenGL&& other) = delete;
	RendererOpenGL& operator=(const RendererOpenGL& rhs) = delete;
	RendererOpenGL& operator=(RendererOpenGL&& rhs) = delete;
	virtual ~RendererOpenGL() override;

	std::vector<DisplayDesc> getDisplayModes() const override;
	DisplayDesc getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const override;
	Vector<int> getWindowClientArea() const noexcept override;

	void drawImage(Image& image, Point<float> position, float scale = 1.0, Color color = Color::Normal) override;

	void drawSubImage(Image& image, Point<float> raster, Rectangle<float> subImageRect, Color color = Color::Normal) override;
	void drawSubImageRotated(Image& image, Point<float> raster, Rectangle<float> subImageRect, float degrees, Color color = Color::Normal) override;

	void drawImageRotated(Image& image, Point<float> position, float degrees, Color color = Color::Normal, float scale = 1.0f) override;
	void drawImageStretched(Image& image, Rectangle<float> rect, Color color = Color::Normal) override;

	void drawImageRepeated(Image& image, Rectangle<float> rect) override;
	void drawSubImageRepeated(Image& image, const Rectangle<float>& source, const Rectangle<float>& destination) override;

	void drawImageToImage(Image& source, Image& destination, const Point<float>& dstPoint) override;

	void drawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawLine(float x, float y, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int line_width) override;
	void drawBox(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawBoxFilled(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawCircle(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int num_segments, float scale_x, float scale_y) override;

	void drawGradient(float x, float y, float w, float h, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4) override;

	void drawText(const Font& font, std::string_view text, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	void showSystemPointer(bool) override;
	void addCursor(const std::string& filePath, int cursorId, int offx, int offy) override;
	void setCursor(int cursorId) override;

	void clearScreen(uint8_t r, uint8_t g, uint8_t b) override;

	float width() const override;
	float height() const override;

	void size(int w, int h) override;
	void minimum_size(int w, int h) override;

	void fullscreen(bool fs, bool maintain = false) override;
	bool fullscreen() const override;

	void resizeable(bool resizable) override;
	bool resizeable() const override;

	void clipRect(float x, float y, float width, float height) final override;

	void window_icon(const std::string& path) override;

	void update() override;

	void setViewport(const Rectangle<int>& viewport) override;
	void setOrthoProjection(const Rectangle<float>& orthoBounds) override;

private:

	void initGL();
	void initVideo(Vector<int> resolution, bool fullscreen, bool vsync);

	void onResize(int w, int h);


	Point<float> desktopResolution;

	std::map<int, SDL_Cursor*> cursors;
};

} // namespace
