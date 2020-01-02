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

#include "NAS2D/Timer.h"

#include <iostream>
#include <algorithm>

using namespace NAS2D;

NAS2D::Timer		_TIMER;

NAS2D::Signals::Signal0<void>	_FADE_COMPLETE;

/**
 * Internal constructor used by derived types to set the name of the Renderer.
 *
 * This c'tor is not public and can't be invoked externally.
 */
Renderer::Renderer(const std::string& appTitle): mTitle(appTitle)
{}


/**
 * D'tor
 */
Renderer::~Renderer()
{
	_FADE_COMPLETE.clear();
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
void Renderer::drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height)
{
	drawSubImage(image, rasterX, rasterY, x, y, width, height, 255, 255, 255, 255);
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
void Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, const Color_4ub& color)
{
	drawSubImageRotated(image, rasterX, rasterY, x, y, width, height, degrees, color.red(), color.green(), color.blue(), color.alpha());
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
void Renderer::drawImageRotated(Image& image, float x, float y, float degrees, const Color_4ub& color, float scale)
{
	drawImageRotated(image, x, y, degrees, color.red(), color.green(), color.blue(), color.alpha(), scale);
}


/**
 * Draws a stretched image using a Color_4ub color structure.
 *
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate to draw the Image at.
 * \param	y		Y-Coordinate to draw the Image at.
 * \param	w		Width to use for drawing the Image.
 * \param	h		Height to use for drawing the Image.
 * \param	color	Color to tint the Image with. Default is COLOR_NORMAL (full bright, no color tinting).
 */
void Renderer::drawImageStretched(Image& image, float x, float y, float w, float h, Color_4ub color)
{
	drawImageStretched(image, x, y, w, h, color.red(), color.green(), color.blue(), color.alpha());
}


/**
 * Draws part of an Image repeated over a rectangular area.
 */
void Renderer::drawSubImageRepeated(Image& image, const Rectangle_2df& source, const Rectangle_2df& destination)
{
	drawSubImageRepeated(image, destination.x(), destination.y(), destination.width(), destination.height(), source.x(), source.y(), source.width(), source.height());
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
void Renderer::fadeColor(const Color_4ub& color)
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

	_TIMER.delta();	// clear timer
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

	mCurrentFadeType = FadeType::None;
	mFadeStep = 255.0f / delay;

	_TIMER.delta(); // clear timer
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
NAS2D::Signals::Signal0<void>& Renderer::fadeComplete() const
{
	return _FADE_COMPLETE;
}


/**
 * Draws a single Pixel to the primary surface.
 *
 * \param	x		X-Coordinate of the pixel to draw.
 * \param	y		Y-Coordinate of the pixel to draw.
 * \param	color	A references to a Color_4ub.
 */
void Renderer::drawPoint(float x, float y, const Color_4ub& color)
{
	drawPoint(x, y, color.red(), color.green(), color.blue(), color.alpha());
}


/**
 * Draws a line from (x, y) - (x2, y2) on the primary surface.
 *
 * \param	x			X-Coordinate of the start of the line.
 * \param	y			Y-Coordinate of the start of the line.
 * \param	x2			X-Coordinate of the end of the line.
 * \param	y2			Y-Coordinate of the end of the line.
 * \param	color		A reference to a Color_4ub.
 * \param	line_width	Width, in pixels, of the line to draw.
 */
void Renderer::drawLine(float x, float y, float x2, float y2, const Color_4ub& color, int line_width)
{
	drawLine(x, y, x2, y2, color.red(), color.green(), color.blue(), color.alpha(), line_width);
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
 * \param	c1	A Color_4ub color value used for point 1.
 * \param	c2	A Color_4ub color value used for point 2.
 * \param	c3	A Color_4ub color value used for point 3.
 * \param	c4	A Color_4ub color value used for point 4.
 */
void Renderer::drawGradient(float x, float y, float w, float h, const Color_4ub& c1, const Color_4ub& c2, const Color_4ub& c3, const Color_4ub& c4)
{
	drawGradient(x, y, w, h, c1.red(), c1.green(), c1.blue(), c1.alpha(), c2.red(), c2.green(), c2.blue(), c2.alpha(), c3.red(), c3.green(), c3.blue(), c3.alpha(), c4.red(), c4.green(), c4.blue(), c4.alpha());
}


/**
 * Gets the current screen resolution as a Point_2df.
 */
const Point_2df& Renderer::size()
{
	return mResolution;
}


/**
 * Internal accessor function for derived Renderer types.
 */
Point_2df& Renderer::_size()
{
	return mResolution;
}


/**
 * Gets the center X-Coordinate of the screen.
 */
float Renderer::center_x()
{
	return width() / 2;
}


/**
 * Gets the center Y-Coordinate of the screen.
 */
float Renderer::center_y()
{
	return height() / 2;
}


/**
 * Returns the name of the Renderer.
 */
const std::string& Renderer::name()
{
	return mRendererName;
}


/**
 * Returns the name of the driver as named by the operating system.
 */
const std::string& Renderer::driverName()
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
const std::string& Renderer::title()
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
 * Clears the screen with a given Color_4ub.
 *
 * \param color	A reference to a Color_4ub.
 */
void Renderer::clearScreen(const Color_4ub& color)
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
		float fade = (_TIMER.delta() * mFadeStep) * static_cast<int>(mCurrentFadeType);

		mCurrentFade += fade;

		if (mCurrentFade < 0.0f || mCurrentFade > 255.0f)
		{
			mCurrentFade = std::clamp(mCurrentFade, 0.0f, 255.0f);
			mCurrentFadeType = FadeType::None;
			_FADE_COMPLETE();
		}
	}

	if (mCurrentFade > 0.0f)
	{
		drawBoxFilled(0, 0, width(), height(), mFadeColor.red(), mFadeColor.green(), mFadeColor.blue(), static_cast<uint8_t>(mCurrentFade));
	}
}
