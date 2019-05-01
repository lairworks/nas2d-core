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

#include "NAS2D/Signal.h"
#include "NAS2D/Renderer/Primitives.h"
#include "NAS2D/Resources/Image.h"
#include "NAS2D/Resources/Font.h"

#include <string>

namespace NAS2D {

// Color Presets
extern const NAS2D::Color_4ub COLOR_BLACK;
extern const NAS2D::Color_4ub COLOR_BLUE;
extern const NAS2D::Color_4ub COLOR_BRIGHT_GREEN;
extern const NAS2D::Color_4ub COLOR_CYAN;
extern const NAS2D::Color_4ub COLOR_GREEN;
extern const NAS2D::Color_4ub COLOR_GREY;
extern const NAS2D::Color_4ub COLOR_MAGENTA;
extern const NAS2D::Color_4ub COLOR_NAVY;
extern const NAS2D::Color_4ub COLOR_ORANGE;
extern const NAS2D::Color_4ub COLOR_RED;
extern const NAS2D::Color_4ub COLOR_SILVER;
extern const NAS2D::Color_4ub COLOR_WHITE;
extern const NAS2D::Color_4ub COLOR_YELLOW;

extern const NAS2D::Color_4ub COLOR_NORMAL;

/**
 * \class Renderer
 * \brief Renderer base class.
 *
 * Provides a standard Renderer interface. The base Renderer can be used
 * but will act as a NULL interface.
 */
class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

	const std::string& name();
	const std::string& driverName();

	const std::string& title();
	void title(const std::string& title);

	virtual void window_icon(const std::string& path);

	void drawImage(Image& image, float x, float y, float scale = 1.0f);
	virtual void drawImage(Image& image, float x, float y, float scale, int r, int g, int b, int a);

	void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height);
	virtual void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, int r, int g, int b, int a);

	void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, const NAS2D::Color_4ub& color = NAS2D::COLOR_NORMAL);
	virtual void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, int r, int g, int b, int a);

	void drawImageRotated(Image& image, float x, float y, float degrees, const Color_4ub& color = COLOR_NORMAL, float scale = 1.0f);
	virtual void drawImageRotated(Image& image, float x, float y, float degrees, int r, int g, int b, int a, float scale = 1.0f);

	void drawImageStretched(Image& image, float x, float y, float w, float h, Color_4ub color = COLOR_NORMAL);
	virtual void drawImageStretched(Image& image, float x, float y, float w, float h, int r, int g, int b, int a);

	virtual void drawImageRepeated(Image& image, float x, float y, float w, float h);
	void drawImageRect(float x, float y, float w, float h, Image& topLeft, Image& top, NAS2D::Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight);
	void drawImageRect(float x, float y, float w, float h, ImageList& images);

	virtual void drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint);

	void drawPoint(float x, float y, const Color_4ub& color = COLOR_WHITE);
	virtual void drawPoint(float x, float y, int r, int g, int b, int a = 255);

	void drawLine(float x, float y, float x2, float y2, const Color_4ub& color = COLOR_WHITE, int line_width = 1);
	virtual void drawLine(float x, float y, float x2, float y2, int r, int g, int b, int a = 255, int line_width = 1);

	void drawBox(const Rectangle_2d& rect, int r, int g, int b, int a = 255);
	void drawBox(const Rectangle_2df& rect, int r, int g, int b, int a = 255);
	virtual void drawBox(float x, float y, float w, float h, int r, int g, int b, int a = 255);

	void drawBoxFilled(const Rectangle_2d& rect, int r, int g, int b, int a = 255);
	void drawBoxFilled(const Rectangle_2df& rect, int r, int g, int b, int a = 255);
	virtual void drawBoxFilled(float x, float y, float width, float height, int r, int g, int b, int a = 255);

	virtual void drawCircle(float x, float y, float radius, int r, int g, int b, int a, int num_segments = 10, float scale_x = 1.0f, float scale_y = 1.0f);

	void drawGradient(float x, float y, float w, float h, const Color_4ub& c1, const Color_4ub& c2, const Color_4ub& c3, const Color_4ub& c4);
	virtual void drawGradient(float x, float y, float w, float h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int r3, int g3, int b3, int a3, int r4, int g4, int b4, int a4);

	virtual void drawText(Font& font, const std::string& text, float x, float y, int r, int g, int b, int a = 255);
	void drawTextShadow(Font& font, const std::string& text, float x, float y, int sDistance, int r, int g, int b, int sr, int sg, int sb, int a = 255);

	void setFadeColor(const Color_4ub& color);
	void fadeIn(float delayTime);
	void fadeOut(float delayTime);
	bool isFading() const;
	bool isFaded() const;
	NAS2D::Signals::Signal0<void>& fadeComplete() const;

	virtual void showSystemPointer(bool);
	virtual void addCursor(const std::string& filePath, int cursorId, int offx, int offy);
	virtual void setCursor(int cursorId);

	void clearScreen(const Color_4ub& color);
	virtual void clearScreen(int r, int g, int b);

	virtual float width();
	virtual float height();

	virtual void size(int w, int h);
	const Point_2df& size();

	virtual void minimum_size(int w, int h);

	float center_x();
	float center_y();

	void clipRect(const Rectangle_2df& rect);
	void clipRectClear();
	virtual void clipRect(float x, float y, float width, float height);

	virtual void fullscreen(bool fs, bool maintain = false);
	virtual bool fullscreen();

	virtual void resizeable(bool _r);
	virtual bool resizeable();

	virtual void update();

protected:
	Renderer(const std::string& rendererName, const std::string& appTitle);

	virtual void initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync) {}
	void driverName(const std::string& name);

	Point_2df& _size();

private:
	/**
	 * Copy c'tor.
	 *
	 * The copy c'tor is intentionally private and undefined. It should
	 * never be invoked at any time.
	 */
	Renderer(const Renderer&);

	/**
	 * Copy operator.
	 *
	 * The copy operator is intentionally private and undefined. It should
	 * never be invoked at any time.
	 */
	Renderer& operator=(const Renderer&);



private:
	std::string			mRendererName;	/**< Internal name of the Renderer. */
	std::string			mDriverName;	/**< OS Driver name */
	std::string			mTitle;			/**< Title to use for the application. */

	Color_4ub			mFadeColor;		/**< Fade Color. */
	float				mFadeStep;		/**< Amount of fading to do per milisecond. */
	float				mCurrentFade;	/**< Current fade amount. */

	Point_2df			mResolution;	/**< Screen resolution. */
};

} // namespace
