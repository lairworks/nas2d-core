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

/**
 * \class RendererOpenGL
 * \brief OpenGL Renderer.
 *
 * Implements an OpenGL based Renderer.
 */
class RendererOpenGL: public Renderer
{
public:
	RendererOpenGL() = delete;
	explicit RendererOpenGL(const std::string& title);
	RendererOpenGL(const RendererOpenGL& other) = delete;
	RendererOpenGL(RendererOpenGL&& other) = delete;
	RendererOpenGL& operator=(const RendererOpenGL& rhs) = delete;
	RendererOpenGL& operator=(RendererOpenGL&& rhs) = delete;
	virtual ~RendererOpenGL() override;

	void drawImage(Image& image, float x, float y, float scale, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	void drawImageRotated(Image& image, float x, float y, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float scale) override;
	void drawImageStretched(Image& image, float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	void drawImageRepeated(Image& image, float x, float y, float w, float h) override;
	void drawSubImageRepeated(Image& image, float rasterX, float rasterY, float w, float h, float subX, float subY, float subW, float subH) override;

	void drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint) override;

	void drawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawLine(float x, float y, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int line_width) override;
	void drawBox(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawBoxFilled(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
	void drawCircle(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int num_segments, float scale_x, float scale_y) override;

	void drawGradient(float x, float y, float w, float h, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4) override;

	void drawText(NAS2D::Font& font, const std::string& text, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

	void showSystemPointer(bool) override;
	void addCursor(const std::string& filePath, int cursorId, int offx, int offy) override;
	void setCursor(int cursorId) override;

	void clearScreen(uint8_t r, uint8_t g, uint8_t b) override;

	float width() override;
	float height() override;

	void size(int w, int h) override;
	void minimum_size(int w, int h) override;

	void fullscreen(bool fs, bool maintain = false) override;
	bool fullscreen() override;

	void resizeable(bool resizable) override;
	bool resizeable() override;

	void clipRect(float x, float y, float width, float height) final override;

	void window_icon(const std::string& path) override;

	void update() override;

private:

	void initGL();
	void initVideo(unsigned int resX, unsigned int resY, bool fullscreen, bool vsync);

	void _resize(int w, int h);
};

} // namespace
