// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Renderer.h"

#include <iostream>
#include <algorithm>

using namespace NAS2D;


/**
 * Internal constructor used by derived types to set the name of the Renderer.
 *
 * This c'tor is not public and can't be invoked externally.
 */
Renderer::Renderer(const std::string& appTitle) : mTitle(appTitle)
{}


/**
 * D'tor
 */
Renderer::~Renderer()
{
	fadeCompleteSignal.clear();
	std::cout << "Renderer Terminated." << std::endl;
}


/**
 * Draws an Image to the screen.
 *
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate of the Image to draw.
 * \param	y		Y-Coordinate of the Image to draw.
 * \param	scale	Scale to draw the Image at. Default is 1.0 (no scaling).
 */
void Renderer::drawImage(Image& image, float x, float y, float scale)
{
	drawImage(image, x, y, scale, 255, 255, 255, 255);
}


void NAS2D::Renderer::drawImage(Image& image, Point<float> position, float scale, Color color)
{
	drawImage(image, position.x(), position.y(), scale, color.red(), color.green(), color.blue(), color.alpha());
}


void NAS2D::Renderer::drawSubImage(Image& image, Point<float> raster, Rectangle<float> subImageRect, const Color& color)
{
	drawSubImage(image, raster, subImageRect.startPoint(), subImageRect.size(), color);
}


void NAS2D::Renderer::drawSubImage(Image& image, Point<float> raster, Point<float> position, Vector<float> size, const Color& color)
{
	drawSubImage(image, raster.x(), raster.y(), position.x(), position.y(), size.x, size.y, color);
}


/**
 * Draws a portion of a given Image to the screen.
 *
 * \param	image		A refernece to an Image Resource.
 * \param	rasterX		X-Coordinate to draw the Image at.
 * \param	rasterY		Y-Coordinate to draw the Image at.
 * \param	x			X-Coordinate of the area to start getting pixel data from.
 * \param	y			Y-Coordinate of the area to start getting pixel data from.
 * \param	width		Width of the area to start getting pixel data from.
 * \param	height		Height of the area to start getting pixel data from.
 */
void Renderer::drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, const Color& color)
{
	drawSubImage(image, rasterX, rasterY, x, y, width, height, color.red(), color.green(), color.blue(), color.alpha());
}


void NAS2D::Renderer::drawSubImageRotated(Image& image, Point<float> raster, Rectangle<float> subImageRect, float degrees, const Color& color)
{
	drawSubImageRotated(image, raster, subImageRect.startPoint(), subImageRect.size(), degrees, color);
}


void NAS2D::Renderer::drawSubImageRotated(Image& image, Point<float> raster, Point<float> position, Vector<float> size, float degrees, const Color& color)
{
	drawSubImageRotated(image, raster.x(), raster.y(), position.x(), position.y(), size.x, size.y, degrees, color);
}


/**
 * Draws a portion of a given Image to the screen with optional rotation.
 *
 * \param	image		A refernece to an Image Resource.
 * \param	rasterX		X-Coordinate to draw the Image at.
 * \param	rasterY		Y-Coordinate to draw the Image at.
 * \param	x			X-Coordinate of the area to start getting pixel data from.
 * \param	y			Y-Coordinate of the area to start getting pixel data from.
 * \param	width		Width of the area to start getting pixel data from.
 * \param	height		Height of the area to start getting pixel data from.
 * \param	degrees		Angle of rotation in degrees.
 * \param	color		Color to tint the Image with. Default is COLOR_NORMAL (full bright, no color tinting).
 */
void Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, const Color& color)
{
	drawSubImageRotated(image, rasterX, rasterY, x, y, width, height, degrees, color.red(), color.green(), color.blue(), color.alpha());
}


void NAS2D::Renderer::drawImageRotated(Image& image, Point<float> position, float degrees, const Color& color, float scale)
{
	drawImageRotated(image, position.x(), position.y(), degrees, color, scale);
}


/**
 * Draws a rotated and scaled image.
 *
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate to draw the Image at.
 * \param	y		Y-Coordinate to draw the Image at.
 * \param	degrees	Angle of rotation in degrees (0.0 - 360.0).
 * \param	color	Color to tint the Image with. Default is COLOR_NORMAL (full bright, no color tinting).
 * \param	scale	Scale to draw the Image at. Default is 1.0 (no scaling).
 */
void Renderer::drawImageRotated(Image& image, float x, float y, float degrees, const Color& color, float scale)
{
	drawImageRotated(image, x, y, degrees, color.red(), color.green(), color.blue(), color.alpha(), scale);
}


void NAS2D::Renderer::drawImageStretched(Image& image, Point<float> position, Vector<float> size, Color color)
{
	drawImageStretched(image, position.x(), position.y(), size.x, size.y, color);
}


/**
 * Draws a stretched image using a Color color structure.
 *
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate to draw the Image at.
 * \param	y		Y-Coordinate to draw the Image at.
 * \param	w		Width to use for drawing the Image.
 * \param	h		Height to use for drawing the Image.
 * \param	color	Color to tint the Image with. Default is COLOR_NORMAL (full bright, no color tinting).
 */
void Renderer::drawImageStretched(Image& image, float x, float y, float w, float h, Color color)
{
	drawImageStretched(image, x, y, w, h, color.red(), color.green(), color.blue(), color.alpha());
}


void Renderer::drawImageRepeated(Image& image, Rectangle<float> rect)
{
	drawImageRepeated(image, rect.startPoint(), rect.size());
}


void NAS2D::Renderer::drawImageRepeated(Image& image, Point<float> position, Vector<float> size)
{
	drawImageRepeated(image, position.x(), position.y(), size.x, size.y);
}


/**
 * Draws part of an Image repeated over a rectangular area.
 */
void Renderer::drawSubImageRepeated(Image& image, const Rectangle_2df& source, const Rectangle_2df& destination)
{
	drawSubImageRepeated(image, destination.x(), destination.y(), destination.width(), destination.height(), source.x(), source.y(), source.width(), source.height());
}


void NAS2D::Renderer::drawImageRect(Point<float> position, Vector<float> size, Image& topLeft, Image& top, NAS2D::Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	drawImageRect(position.x(), position.y(), size.x, size.y, topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight);
}

/**
 * Comment me!
 */
void Renderer::drawImageRect(float x, float y, float w, float h, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	// Draw the center area if it's defined.
	drawImageRepeated(center, x + topLeft.width(), y + topLeft.height(), w - topRight.width() - topLeft.width(), h - topLeft.height() - bottomLeft.height());

	// Draw the sides
	drawImageRepeated(top, x + static_cast<float>(topLeft.width()), y, w - static_cast<float>(topLeft.width()) - static_cast<float>(topRight.width()), static_cast<float>(top.height()));
	drawImageRepeated(bottom, x + static_cast<float>(bottomLeft.width()), y + h - static_cast<float>(bottom.height()), w - static_cast<float>(bottomLeft.width()) - bottomRight.width(), static_cast<float>(bottom.height()));
	drawImageRepeated(left, x, y + static_cast<float>(topLeft.height()), static_cast<float>(left.width()), h - static_cast<float>(topLeft.height()) - static_cast<float>(bottomLeft.height()));
	drawImageRepeated(right, x + w - static_cast<float>(right.width()), y + static_cast<float>(topRight.height()), static_cast<float>(right.width()), h - static_cast<float>(topRight.height()) - static_cast<float>(bottomRight.height()));

	// Draw the corners
	drawImage(topLeft, x, y);
	drawImage(topRight, x + w - topRight.width(), y);
	drawImage(bottomLeft, x, y + h - bottomLeft.height());
	drawImage(bottomRight, x + w - bottomRight.width(), y + h - bottomRight.height());
}


void NAS2D::Renderer::drawImageRect(Point<float> position, Vector<float> size, ImageList& images)
{
	drawImageRect(position.x(), position.y(), size.x, size.y, images);
}


/**
 * Draws a rectangle using a set of images.
 *
 * This function expects an ImageList with 9 images in it: four corners,
 * four edges and a center image to fill the rest in with.
 *
 * Images in the ImageList are used in the following order:
 *
 * \code
 * +---+---+---+
 * | 0 | 1 | 2 |
 * +---+---+---+
 * | 3 | 4 | 5 |
 * +---+---+---+
 * | 6 | 7 | 8 |
 * +---+---+---+
 * \endcode
 *
 * \param	x		X-Coordinate to start drawing the image rect at.
 * \param	y		X-Coordinate to start drawing the image rect at.
 * \param	w		Width of the image rect.
 * \param	h		Height of the image rect.
 * \param	images	A set of 9 images used to draw the image rect.
 */
void Renderer::drawImageRect(float x, float y, float w, float h, ImageList &images)
{
	// We need 9 images in order to render a rectangle, one for each corner, one for each edge and one for the background.
	if (images.size() == 9)
	{
		drawImageRect(x, y, w, h, images[0], images[1], images[2], images[3], images[4], images[5], images[6], images[7], images[8]);
	}
}


/**
 * Sets the color of the fade.
 *
 * \param	color	A reference to aColor_4ub.
 */
void Renderer::fadeColor(const Color& color)
{
	mFadeColor = color;
}


/**
 * Non-blocking screen fade.
 *
 * \param	delay	Time in miliseconds the fade should last. A value of 0
 *					will instantly fade the screen in.
 */
void Renderer::fadeIn(float delay)
{
	if (delay == 0)
	{
		mCurrentFade = 0.0f;
		mCurrentFadeType = FadeType::None;
		return;
	}

	mCurrentFadeType = FadeType::In;
	mFadeStep = 255.0f / delay;

	fadeTimer.delta();	// clear timer
}


/**
 * Non-blocking screen fade.
 *
 * \param	delay	Time in miliseconds the fade should last. A value of 0
 *					will instantly fade the screen in.
 */
void Renderer::fadeOut(float delay)
{
	if (delay == 0)
	{
		mCurrentFade = 255.0f;
		mCurrentFadeType = FadeType::None;
		return;
	}

	mCurrentFadeType = FadeType::Out;
	mFadeStep = 255.0f / delay;

	fadeTimer.delta(); // clear timer
}


/**
 * Gets whether or not a fade is in progress.
 */
bool Renderer::isFading() const
{
	return (mCurrentFadeType != FadeType::None);
}


/**
 * Gets whether the screen is faded or not.
 */
bool Renderer::isFaded() const
{
	return (mCurrentFade == 255.0f);
}


/**
 * Gets a refernece to the callback signal for fade transitions.
 */
NAS2D::Signals::Signal<>& Renderer::fadeComplete()
{
	return fadeCompleteSignal;
}


void NAS2D::Renderer::drawPoint(Point<float> position, const Color& color)
{
	drawPoint(position.x(), position.y(), color);
}


/**
 * Draws a single Pixel to the primary surface.
 *
 * \param	x		X-Coordinate of the pixel to draw.
 * \param	y		Y-Coordinate of the pixel to draw.
 * \param	color	A references to a Color.
 */
void Renderer::drawPoint(float x, float y, const Color& color)
{
	drawPoint(x, y, color.red(), color.green(), color.blue(), color.alpha());
}


void NAS2D::Renderer::drawLine(Point<float> startPosition, Point<float> endPosition, const Color& color, int line_width)
{
	drawLine(startPosition.x(), startPosition.y(), endPosition.x(), endPosition.y(), color, line_width);
}


/**
 * Draws a line from (x, y) - (x2, y2) on the primary surface.
 *
 * \param	x			X-Coordinate of the start of the line.
 * \param	y			Y-Coordinate of the start of the line.
 * \param	x2			X-Coordinate of the end of the line.
 * \param	y2			Y-Coordinate of the end of the line.
 * \param	color		A reference to a Color.
 * \param	line_width	Width, in pixels, of the line to draw.
 */
void Renderer::drawLine(float x, float y, float x2, float y2, const Color& color, int line_width)
{
	drawLine(x, y, x2, y2, color.red(), color.green(), color.blue(), color.alpha(), line_width);
}


void Renderer::drawBox(const Rectangle<float>& rect, const Color& color)
{
	drawBox(rect.x(), rect.y(), rect.width(), rect.height(), color.red(), color.green(), color.blue(), color.alpha());
}


/**
 * Draws a hollow box on the primary surface.
 *
 * \param	rect	A reference to a Rectangle_2d defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBox(const Rectangle_2d& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBox(static_cast<float>(rect.x()), static_cast<float>(rect.y()), static_cast<float>(rect.width()), static_cast<float>(rect.height()), r, g, b, a);
}


/**
 * Draws a hollow box on the primary surface.
 *
 * \param	rect	A reference to a Rectangle_2df defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBox(const Rectangle_2df& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBox(rect.x(), rect.y(), rect.width(), rect.height(), r, g, b, a);
}


void Renderer::drawBoxFilled(const Rectangle<float>& rect, const Color& color)
{
	drawBoxFilled(rect.x(), rect.y(), rect.width(), rect.height(), color.red(), color.green(), color.blue(), color.alpha());
}


/**
 * Fills a given area with a solid color.
 *
 * \param	rect	A reference to a Rectangle_2df defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBoxFilled(const Rectangle_2d& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBoxFilled(static_cast<float>(rect.x()), static_cast<float>(rect.y()), static_cast<float>(rect.width()), static_cast<float>(rect.height()), r, g, b, a);
}


/**
 * Fills a given area with a solid color.
 *
 * \param	rect	A reference to a Rectangle_2df defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBoxFilled(const Rectangle_2df& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBoxFilled(rect.x(), rect.y(), rect.width(), rect.height(), r, g, b, a);
}


void Renderer::drawCircle(Point<float> position, float radius, Color color, int num_segments, Vector<float> scale)
{
	drawCircle(position.x(), position.y(), radius, color.red(), color.green(), color.blue(), color.alpha(), num_segments, scale.x, scale.y);
}


void Renderer::drawGradient(Rectangle<float> rect, const Color& c1, const Color& c2, const Color& c3, const Color& c4)
{
	drawGradient(rect.startPoint(), rect.size(), c1, c2, c3, c4);
}


void Renderer::drawGradient(Point<float> position, Vector<float> size, const Color& c1, const Color& c2, const Color& c3, const Color& c4)
{
	drawGradient(position.x(), position.y(), size.x, size.y, c1, c2, c3, c4);
}


/**
 * Draws a rectangular area with a color gradient.
 *
 * Each point of the rectangular area can be given a different color value to
 * produce a variety of effects. The vertex orders are as follows:
 *
 * 1-----4
 * |     |
 * |     |
 * 2-----3
 *
 * \param	x	X-Position of the rectangular area to draw.
 * \param	y	Y-Position of the rectangular area to draw.
 * \param	w	Width of the rectangular area to draw.
 * \param	h	Height of the rectangular area to draw.
 * \param	c1	A Color color value used for point 1.
 * \param	c2	A Color color value used for point 2.
 * \param	c3	A Color color value used for point 3.
 * \param	c4	A Color color value used for point 4.
 */
void Renderer::drawGradient(float x, float y, float w, float h, const Color& c1, const Color& c2, const Color& c3, const Color& c4)
{
	drawGradient(x, y, w, h, c1.red(), c1.green(), c1.blue(), c1.alpha(), c2.red(), c2.green(), c2.blue(), c2.alpha(), c3.red(), c3.green(), c3.blue(), c3.alpha(), c4.red(), c4.green(), c4.blue(), c4.alpha());
}


void NAS2D::Renderer::drawText(Font& font, const std::string& text, Point<float> position, Color color)
{
	drawText(font, text, position.x(), position.y(), color.red(), color.green(), color.blue(), color.alpha());
}


void NAS2D::Renderer::drawTextShadow(Font& font, const std::string& text, Point<float> position, Vector<float> shadowOffset, Color textColor, Color shadowColor)
{
	const auto shadowPosition = position + shadowOffset;
	drawText(font, text, shadowPosition.x(), shadowPosition.y(), shadowColor.red(), shadowColor.green(), shadowColor.blue(), shadowColor.alpha());
	drawText(font, text, position.x(), position.y(), textColor.red(), textColor.green(), textColor.blue(), textColor.alpha());
}

/**
 * Gets the current screen resolution as a Vector.
 */
Vector<float> Renderer::size() const
{
	return mResolution;
}


/**
 * Gets the center coordinates of the screen.
 */
Point<float> Renderer::center() const
{
	return Point<float>{} + mResolution / 2;
}


/**
 * Gets the center X-Coordinate of the screen.
 */
float Renderer::center_x() const
{
	return width() / 2;
}


/**
 * Gets the center Y-Coordinate of the screen.
 */
float Renderer::center_y() const
{
	return height() / 2;
}


/**
 * Returns the name of the driver as named by the operating system.
 */
const std::string& Renderer::driverName() const
{
	return mDriverName;
}


/**
 * Sets the driver name.
 *
 * \note	Internal function used only by derived
 *			renderer types.
 */
void Renderer::driverName(const std::string& name)
{
	mDriverName = name;
}


/**
 * Returns the title of the application window.
 */
const std::string& Renderer::title() const
{
	return mTitle;
}


/**
 * Sets the title of the application window.
 */
void Renderer::title(const std::string& title)
{
	mTitle = title;
}


/**
 * Renders a text string with a drop shadow.
 *
 * \param font		A reference to a Font Resource.
 * \param text		The text to draw.
 * \param x			X-Coordinate to render text string.
 * \param y			Y-Coordinate to render text string.
 * \param distance	Distance in pixels the drop shadow should be rendered.
 * \param r			Red color value between 0 - 255.
 * \param g			Green color value between 0 - 255.
 * \param b			Blue color value between 0 - 255.
 * \param sr		Red color value between 0 - 255.
 * \param sg		Green color value between 0 - 255.
 * \param sb		Blue color value between 0 - 255.
 * \param a			Alpha color value between 0 - 255.
 */
void Renderer::drawTextShadow(Font& font, const std::string& text, float x, float y, int distance, uint8_t r, uint8_t g, uint8_t b, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t a )
{
	drawText(font, text, x + distance, y + distance, sr, sg, sb, a);
	drawText(font, text, x, y, r, g, b, a);
}


/**
 * Sets a rectangular area of the screen outside of which nothing is drawn.
 *
 * \param	rect	Reference to a Rectangle_2df representing to area to clip against.
 *
 * \see clipRectClear()
 */
void Renderer::clipRect(const Rectangle_2df& rect)
{
	clipRect(rect.x(), rect.y(), rect.width(), rect.height());
}


/**
 * Clears the clipping rectangle.
 */
void Renderer::clipRectClear()
{
	clipRect(0, 0, 0, 0);
}


/**
 * Clears the screen with a given Color.
 *
 * \param color	A reference to a Color.
 */
void Renderer::clearScreen(const Color& color)
{
	clearScreen(color.red(), color.green(), color.blue());
}


/**
 * Updates the screen.
 *
 * \note	All derived Renderer objects must call Renderer::update()
 *			before performing screen refreshes.
 */
void Renderer::update()
{
	if (mCurrentFadeType != FadeType::None)
	{
		float fade = (fadeTimer.delta() * mFadeStep) * static_cast<int>(mCurrentFadeType);

		mCurrentFade += fade;

		if (mCurrentFade < 0.0f || mCurrentFade > 255.0f)
		{
			mCurrentFade = std::clamp(mCurrentFade, 0.0f, 255.0f);
			mCurrentFadeType = FadeType::None;
			fadeCompleteSignal();
		}
	}

	if (mCurrentFade > 0.0f)
	{
		drawBoxFilled(0, 0, width(), height(), mFadeColor.red(), mFadeColor.green(), mFadeColor.blue(), static_cast<uint8_t>(mCurrentFade));
	}
}

void NAS2D::Renderer::setResolution(const Vector<float>& newResolution)
{
	if (!fullscreen())
	{
		mResolution = {newResolution.x, newResolution.y};
	}
}
