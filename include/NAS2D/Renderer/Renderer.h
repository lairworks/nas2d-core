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

#include "Color.h"
#include "DisplayDesc.h"
#include "Point.h"
#include "Vector.h"
#include "Rectangle.h"
#include "../Signal.h"
#include "../Resources/Image.h"
#include "../Resources/Font.h"

#include <string>
#include <vector>

namespace NAS2D {

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
	Renderer() = default;
	Renderer(const Renderer& rhs) = default;
	Renderer& operator=(const Renderer& rhs) = default;
	Renderer(Renderer&& rhs) = default;
	Renderer& operator=(Renderer&& rhs) = default;
	virtual ~Renderer();
	
	virtual std::vector<DisplayDesc> getDisplayModes() const = 0;
	virtual DisplayDesc getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const = 0;
	virtual Vector<int> getWindowClientArea() const noexcept = 0;

	const std::string& driverName() const;

	const std::string& title() const;
	void title(const std::string& title);

	virtual void window_icon(const std::string& path) = 0;

	void drawImage(Image& image, Point<float> position, float scale = 1.0f) const;
	void drawImage(Image& image, float x, float y, float scale = 1.0f) const;
	void drawImage(Image& image, Point<float> position, float scale, Color color) const;
	virtual void drawImage(Image& image, float x, float y, float scale, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const = 0;

	void drawSubImage(Image& image, Point<float> raster, Point<float> position, Vector<float> size) const;
	void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height) const;
	virtual void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const = 0;

	void drawSubImageRotated(Image& image, Point<float> raster, Point<float> position, Vector<float> size, float degrees, const Color& color = Color::Normal) const;
	void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, const Color& color = Color::Normal) const;
	virtual void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const = 0;

	void drawImageRotated(Image& image, Point<float> position, float degrees, const Color& color = Color::Normal, float scale = 1.0f) const;
	void drawImageRotated(Image& image, float x, float y, float degrees, const Color& color = Color::Normal, float scale = 1.0f) const;
	virtual void drawImageRotated(Image& image, float x, float y, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float scale = 1.0f) const = 0;

	void drawImageStretched(Image& image, Point<float> position, Vector<float> size, Color color = Color::Normal) const;
	void drawImageStretched(Image& image, float x, float y, float w, float h, Color color = Color::Normal) const;
	virtual void drawImageStretched(Image& image, float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const = 0;

	void drawImageRepeated(Image& image, Rectangle<float> rect) const;
	void drawImageRepeated(Image& image, Point<float> position, Vector<float> size) const;
	virtual void drawImageRepeated(Image& image, float x, float y, float w, float h) const = 0;
	
	void drawSubImageRepeated(Image& image, const Rectangle_2df& source, const Rectangle_2df& destination) const;
	virtual void drawSubImageRepeated(Image& image, float rasterX, float rasterY, float w, float h, float subX, float subY, float subW, float subH) const = 0;

	void drawImageRect(Point<float> position, Vector<float> size, Image& topLeft, Image& top, NAS2D::Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight) const;
	void drawImageRect(float x, float y, float w, float h, Image& topLeft, Image& top, NAS2D::Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight) const;
	void drawImageRect(Point<float> position, Vector<float> size, ImageList& images) const;
	void drawImageRect(float x, float y, float w, float h, ImageList& images) const;

	virtual void drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint) const = 0;

	void drawPoint(Point<float> position, const Color& color = Color::White) const;
	void drawPoint(float x, float y, const Color& color = Color::White) const;
	virtual void drawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const = 0;

	void drawLine(Point<float> startPosition, Point<float> endPosition, const Color& color = Color::White, int line_width = 1) const;
	void drawLine(float x, float y, float x2, float y2, const Color& color = Color::White, int line_width = 1) const;
	virtual void drawLine(float x, float y, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, int line_width = 1) const = 0;

	void drawBox(const Rectangle<float>& rect, const Color& color = Color::White) const;
	void drawBox(const Rectangle_2d& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const;
	void drawBox(const Rectangle_2df& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const;
	virtual void drawBox(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const = 0;

	void drawBoxFilled(const Rectangle<float>& rect, const Color& color = Color::White) const;
	void drawBoxFilled(const Rectangle_2d& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const;
	void drawBoxFilled(const Rectangle_2df& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const;
	virtual void drawBoxFilled(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const = 0;

	void drawCircle(Point<float> position, float radius, Color color, int num_segments = 10, Vector<float> scale = Vector<float>{1.0f, 1.0f}) const;
	virtual void drawCircle(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int num_segments = 10, float scale_x = 1.0f, float scale_y = 1.0f) const = 0;

	void drawGradient(Rectangle<float> rect, const Color& c1, const Color& c2, const Color& c3, const Color& c4) const;
	void drawGradient(Point<float> position, Vector<float> size, const Color& c1, const Color& c2, const Color& c3, const Color& c4) const;
	void drawGradient(float x, float y, float w, float h, const Color& c1, const Color& c2, const Color& c3, const Color& c4) const;
	virtual void drawGradient(float x, float y, float w, float h, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4) const = 0;

	void drawText(Font& font, const std::string& text, Point<float> position, Color color) const;
	virtual void drawText(Font& font, const std::string& text, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const = 0;
	void drawTextShadow(Font& font, const std::string& text, Point<float> position, Vector<float> shadowOffset, Color textColor, Color shadowColor) const;
	void drawTextShadow(Font& font, const std::string& text, float x, float y, int sDistance, uint8_t r, uint8_t g, uint8_t b, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t a = 255) const;

	void fadeColor(const Color& color);
	void fadeIn(float delayTime);
	void fadeOut(float delayTime);
	bool isFading() const;
	bool isFaded() const;
	NAS2D::Signals::Signal<>& fadeComplete() const;

	virtual void showSystemPointer(bool) = 0;
	virtual void addCursor(const std::string& filePath, int cursorId, int offx, int offy) = 0;
	virtual void setCursor(int cursorId) = 0;

	void clearScreen(const Color& color);
	virtual void clearScreen(uint8_t r, uint8_t g, uint8_t b) = 0;

	virtual float width() const = 0;
	virtual float height() const = 0;

	Vector<float> size() const;
	virtual void size(int w, int h) = 0;

	virtual void minimum_size(int w, int h) = 0;

	Point<float> center() const;
	float center_x() const;
	float center_y() const;

	void clipRect(const Rectangle_2df& rect);
	void clipRectClear();
	virtual void clipRect(float x, float y, float width, float height) = 0;

	virtual void fullscreen(bool fs, bool maintain = false) = 0;
	virtual bool fullscreen() const = 0;

	virtual void resizeable(bool _r) = 0;
	virtual bool resizeable() const = 0;

	virtual void update();

	virtual void setViewport(const Rectangle<int>& viewport) = 0;
	virtual void setOrthoProjection(const Rectangle<float>& orthoBounds) = 0;
	void setResolution(const Vector<float>& newResolution);

protected:
	Renderer(const std::string& appTitle);

	void driverName(const std::string& name);

	Vector<float> mResolution{1600,900}; /**< Screen resolution. Reasonable default in 2019*/

private:
	enum class FadeType
	{
		In = -1,
		None = 0,
		Out = 1
	};

	std::string mDriverName{"NULL Renderer"};	/**< OS Driver name */
	std::string mTitle{"Default Application"};	/**< Title to use for the application. */

	Color mFadeColor{0,0,0,255}; /**< Fade Color. */
	float mFadeStep{0.0f}; /**< Amount of fading to do per milisecond. */
	float mCurrentFade{0.0f}; /**< Current fade amount. */

	FadeType mCurrentFadeType{FadeType::None};
};

} // namespace
