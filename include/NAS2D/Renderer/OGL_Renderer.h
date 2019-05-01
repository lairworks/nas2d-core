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
 * \class OGL_Renderer
 * \brief OpenGL Renderer.
 *
 * Implements an OpenGL based Renderer.
 */
class OGL_Renderer: public Renderer
{
public:
	OGL_Renderer(const std::string& title);

	~OGL_Renderer();

	void drawImage(Image& image, float x, float y, float scale, int r, int g, int b, int a);

	void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, int r, int g, int b, int a);
	void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, int r, int g, int b, int a);

	void drawImageRotated(Image& image, float x, float y, float degrees, int r, int g, int b, int a, float scale);
	void drawImageStretched(Image& image, float x, float y, float w, float h, int r, int g, int b, int a);

	void drawImageRepeated(Image& image, float x, float y, float w, float h);

	void drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint);

	void drawPoint(float x, float y, int r, int g, int b, int a);
	void drawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, int line_width);
	void drawBox(float x, float y, float width, float height, int r, int g, int b, int a);
	void drawBoxFilled(float x, float y, float width, float height, int r, int g, int b, int a);
	void drawCircle(float x, float y, float radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y);

	void drawGradient(float x, float y, float w, float h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int r3, int g3, int b3, int a3, int r4, int g4, int b4, int a4);

    void drawText(NAS2D::Font& font, const std::string& text, float x, float y, int r, int g, int b, int a);

	virtual void showSystemPointer(bool);
	void addCursor(const std::string& filePath, int cursorId, int offx, int offy);
	void setCursor(int cursorId);

	void clearScreen(int r, int g, int b);

	virtual float width();
	virtual float height();

	virtual void size(int w, int h);
	virtual void minimum_size(int w, int h);

	virtual void fullscreen(bool fs, bool maintain = false);
	virtual bool fullscreen();

	virtual void resizeable(bool _r);
	virtual bool resizeable();

	virtual void clipRect(float x, float y, float width, float height) final;

	virtual void window_icon(const std::string& path);

	void update();

private:
	OGL_Renderer(const OGL_Renderer&);				// Intentionally left undefined;
	OGL_Renderer& operator=(const OGL_Renderer&);	// Intentionally left undefined;

	void initGL();
	void initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);

	void _resize(int w, int h);
};

} // namespace
