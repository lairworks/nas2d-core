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

#include "Primitives.h"
#include "../Signal.h"
#include "../Resources/Image.h"
#include "../Resources/Font.h"

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
	Renderer(const Renderer& rhs) = default;
	Renderer& operator=(const Renderer& rhs) = default;
	Renderer(Renderer&& rhs) = default;
	Renderer& operator=(Renderer&& rhs) = default;
	virtual ~Renderer();

	const std::string& name();
	const std::string& driverName();

	const std::string& title();
	void title(const std::string& title);

	virtual void window_icon(const std::string& path) = 0;

	void drawImage(Image& image, float x, float y, float scale = 1.0f);
	virtual void drawImage(Image& image, float x, float y, float scale, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

	void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height);
	virtual void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

	void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, const NAS2D::Color_4ub& color = NAS2D::COLOR_NORMAL);
	virtual void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

	void drawImageRotated(Image& image, float x, float y, float degrees, const Color_4ub& color = COLOR_NORMAL, float scale = 1.0f);
	virtual void drawImageRotated(Image& image, float x, float y, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float scale = 1.0f) = 0;

	void drawImageStretched(Image& image, float x, float y, float w, float h, Color_4ub color = COLOR_NORMAL);
	virtual void drawImageStretched(Image& image, float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

	virtual void drawImageRepeated(Image& image, float x, float y, float w, float h) = 0;
	
	void drawSubImageRepeated(Image& image, const Rectangle_2df& source, const Rectangle_2df& destination);
	virtual void drawSubImageRepeated(Image& image, float rasterX, float rasterY, float w, float h, float subX, float subY, float subW, float subH) = 0;

	void drawImageRect(float x, float y, float w, float h, Image& topLeft, Image& top, NAS2D::Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight);
	void drawImageRect(float x, float y, float w, float h, ImageList& images);

	virtual void drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint) = 0;

	void drawPoint(float x, float y, const Color_4ub& color = COLOR_WHITE);
	virtual void drawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;

	void drawLine(float x, float y, float x2, float y2, const Color_4ub& color = COLOR_WHITE, int line_width = 1);
	virtual void drawLine(float x, float y, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, int line_width = 1) = 0;

	void drawBox(const Rectangle_2d& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void drawBox(const Rectangle_2df& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	virtual void drawBox(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;

	void drawBoxFilled(const Rectangle_2d& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void drawBoxFilled(const Rectangle_2df& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	virtual void drawBoxFilled(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;

	virtual void drawCircle(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int num_segments = 10, float scale_x = 1.0f, float scale_y = 1.0f) = 0;

	void drawGradient(float x, float y, float w, float h, const Color_4ub& c1, const Color_4ub& c2, const Color_4ub& c3, const Color_4ub& c4);
	virtual void drawGradient(float x, float y, float w, float h, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4) = 0;

	virtual void drawText(Font& font, const std::string& text, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;
	void drawTextShadow(Font& font, const std::string& text, float x, float y, int sDistance, uint8_t r, uint8_t g, uint8_t b, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t a = 255);

	void fadeColor(const Color_4ub& color);
	void fadeIn(float delayTime);
	void fadeOut(float delayTime);
	bool isFading() const;
	bool isFaded() const;
	NAS2D::Signals::Signal0<void>& fadeComplete() const;

	virtual void showSystemPointer(bool) = 0;
	virtual void addCursor(const std::string& filePath, int cursorId, int offx, int offy) = 0;
	virtual void setCursor(int cursorId) = 0;

	void clearScreen(const Color_4ub& color);
	virtual void clearScreen(uint8_t r, uint8_t g, uint8_t b) = 0;

	virtual float width() = 0;
	virtual float height() = 0;

	virtual void size(int w, int h) = 0;
	const Point_2df& size();

	virtual void minimum_size(int w, int h) = 0;

	float center_x();
	float center_y();

	void clipRect(const Rectangle_2df& rect);
	void clipRectClear();
	virtual void clipRect(float x, float y, float width, float height) = 0;

	virtual void fullscreen(bool fs, bool maintain = false) = 0;
	virtual bool fullscreen() = 0;

	virtual void resizeable(bool _r) = 0;
	virtual bool resizeable() = 0;

	virtual void update();

protected:
	Renderer(const std::string& rendererName, const std::string& appTitle);

	void driverName(const std::string& name);

	Point_2df& _size();

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
