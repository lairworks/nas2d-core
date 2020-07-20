// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Renderer.h"

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
 * Returns the title of the application window.
 */
const std::string& Renderer::title() const
{
	return mTitle;
}


/**
 * Returns the name of the driver as named by the operating system.
 */
const std::string& Renderer::driverName() const
{
	return mDriverName;
}


/**
 * Sets the title of the application window.
 */
void Renderer::title(const std::string& title)
{
	mTitle = title;
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
	drawImage(image, {x, y}, scale);
}


void Renderer::drawImage(Image& image, float x, float y, float scale, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawImage(image, {x, y}, scale, {r, g, b, a});
}


void Renderer::drawSubImage(Image& image, Point<float> raster, Point<float> position, Vector<float> size, Color color)
{
	drawSubImage(image, raster, {position.x, position.y, size.x, size.y}, color);
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
void Renderer::drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, Color color)
{
	drawSubImage(image, {rasterX, rasterY}, {x, y, width, height}, color);
}


void Renderer::drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawSubImage(image, {rasterX, rasterY}, {x, y, width, height}, {r, g, b, a});
}


void Renderer::drawSubImageRotated(Image& image, Point<float> raster, Point<float> position, Vector<float> size, float degrees, Color color)
{
	drawSubImageRotated(image, raster, {position.x, position.y, size.x, size.y}, degrees, color);
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
void Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, Color color)
{
	drawSubImageRotated(image, {rasterX, rasterY}, {x, y, width, height}, degrees, color);
}


void Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawSubImageRotated(image, {rasterX, rasterY}, {x, y, width, height}, degrees, {r, g, b, a});
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
void Renderer::drawImageRotated(Image& image, float x, float y, float degrees, Color color, float scale)
{
	drawImageRotated(image, {x, y}, degrees, color, scale);
}


void Renderer::drawImageRotated(Image& image, float x, float y, float degrees, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float scale)
{
	drawImageRotated(image, {x, y}, degrees, {r, g, b, a}, scale);
}


void Renderer::drawImageStretched(Image& image, Point<float> position, Vector<float> size, Color color)
{
	drawImageStretched(image, {position.x, position.y, size.x, size.y}, color);
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
	drawImageStretched(image, {x, y, w, h}, color);
}


void Renderer::drawImageStretched(Image& image, float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawImageStretched(image, {x, y, w, h}, {r, g, b, a});
}


void Renderer::drawImageRepeated(Image& image, Point<float> position, Vector<float> size)
{
	drawImageRepeated(image, {position.x, position.y, size.x, size.y});
}


void Renderer::drawImageRepeated(Image& image, float x, float y, float w, float h)
{
	drawImageRepeated(image, {x, y, w, h});
}


/**
 * Draws part of an Image repeated over a rectangular area.
 */
void Renderer::drawSubImageRepeated(Image& image, float rasterX, float rasterY, float w, float h, float subX, float subY, float subW, float subH)
{
	drawSubImageRepeated(image, {rasterX, rasterY, w, h}, {subX, subY, subW, subH});
}


void Renderer::drawImageRect(Rectangle<float> rect, ImageList& images)
{
	if (images.size() != 9)
	{
		throw std::runtime_error("Must pass 9 images to drawImageRect, but images.size() == " + std::to_string(images.size()));
	}

	drawImageRect({rect.x, rect.y, rect.width, rect.height}, images[0], images[1], images[2], images[3], images[4], images[5], images[6], images[7], images[8]);
}


void Renderer::drawImageRect(Point<float> position, Vector<float> size, ImageList& images)
{
	drawImageRect({position.x, position.y, size.x, size.y}, images);
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
	drawImageRect({x, y, w, h}, images);
}


void Renderer::drawImageRect(Rectangle<float> rect, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	const auto p1 = rect.startPoint() + topLeft.size();
	const auto p2 = rect.crossXPoint() + topRight.size().reflectX();
	const auto p3 = rect.crossYPoint() + bottomLeft.size().reflectY();
	const auto p4 = rect.endPoint() - bottomRight.size();

	// Draw the center area
	drawImageRepeated(center, Rectangle<float>::Create(p1, p4));

	// Draw the sides
	drawImageRepeated(top, Rectangle<float>::Create({p1.x, rect.y}, p2));
	drawImageRepeated(bottom, Rectangle<float>::Create(p3, Point{p4.x, rect.endPoint().y}));
	drawImageRepeated(left, Rectangle<float>::Create({rect.x, p1.y}, p3));
	drawImageRepeated(right, Rectangle<float>::Create(p2, Point{rect.endPoint().x, p4.y}));

	// Draw the corners
	drawImage(topLeft, rect.startPoint());
	drawImage(topRight, {p2.x, rect.y});
	drawImage(bottomLeft, {rect.x, p3.y});
	drawImage(bottomRight, p4);
}


void Renderer::drawImageRect(Point<float> position, Vector<float> size, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	drawImageRect({position.x, position.y, size.x, size.y}, topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight);
}

/**
 * Comment me!
 */
void Renderer::drawImageRect(float x, float y, float w, float h, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	drawImageRect({x, y, w, h}, topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight);
}


/**
 * Draws a single Pixel to the primary surface.
 *
 * \param	x		X-Coordinate of the pixel to draw.
 * \param	y		Y-Coordinate of the pixel to draw.
 * \param	color	A Color.
 */
void Renderer::drawPoint(float x, float y, Color color)
{
	drawPoint({x, y}, color);
}


void Renderer::drawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawPoint({x, y}, {r, g, b, a});
}


/**
 * Draws a line from (x, y) - (x2, y2) on the primary surface.
 *
 * \param	x			X-Coordinate of the start of the line.
 * \param	y			Y-Coordinate of the start of the line.
 * \param	x2			X-Coordinate of the end of the line.
 * \param	y2			Y-Coordinate of the end of the line.
 * \param	color		A Color.
 * \param	line_width	Width, in pixels, of the line to draw.
 */
void Renderer::drawLine(float x, float y, float x2, float y2, Color color, int line_width)
{
	drawLine({x, y}, {x2, y2}, color, line_width);
}


void Renderer::drawLine(float x, float y, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int line_width)
{
	drawLine({x, y}, {x2, y2}, {r, g, b, a}, line_width);
}


/**
 * Draws a hollow box on the primary surface.
 *
 * \param	rect	A reference to a Rectangle<float> defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBox(const Rectangle<float>& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBox(rect, {r, g, b, a});
}


void Renderer::drawBox(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBox({x, y, w, h}, {r, g, b, a});
}


/**
 * Fills a given area with a solid color.
 *
 * \param	rect	A reference to a Rectangle<float> defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBoxFilled(const Rectangle<float>& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBoxFilled(rect, {r, g, b, a});
}


void Renderer::drawBoxFilled(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawBoxFilled({x, y, width, height}, {r, g, b, a});
}


void Renderer::drawCircle(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int num_segments, float scale_x, float scale_y)
{
	drawCircle({x, y}, radius, {r, g, b, a}, num_segments, {scale_x, scale_y});
}


void Renderer::drawGradient(Point<float> position, Vector<float> size, Color c1, Color c2, Color c3, Color c4)
{
	drawGradient({position.x, position.y, size.x, size.y}, c1, c2, c3, c4);
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
void Renderer::drawGradient(float x, float y, float w, float h, Color c1, Color c2, Color c3, Color c4)
{
	drawGradient({x, y, w, h}, c1, c2, c3, c4);
}


void Renderer::drawGradient(float x, float y, float w, float h, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4)
{
	drawGradient({x, y, w, h}, {r1, g1, b1, a1}, {r2, g2, b2, a2}, {r3, g3, b3, a3}, {r4, g4, b4, a4});
}


void Renderer::drawText(const Font& font, std::string_view text, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	drawText(font, text, {x, y}, {r, g, b, a});
}


void Renderer::drawTextShadow(const Font& font, std::string_view text, Point<float> position, Vector<float> shadowOffset, Color textColor, Color shadowColor)
{
	const auto shadowPosition = position + shadowOffset;
	drawText(font, text, shadowPosition, shadowColor);
	drawText(font, text, position, textColor);
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
void Renderer::drawTextShadow(const Font& font, std::string_view text, float x, float y, int distance, uint8_t r, uint8_t g, uint8_t b, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t a )
{
	drawTextShadow(font, text, {x, y}, Vector{distance, distance}, {r, g, b, a}, {sr, sg, sb, a});
}


/**
 * Sets the color of the fade.
 *
 * \param	color	A Color.
 */
void Renderer::fadeColor(Color color)
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

	fadeTimer.delta(); // clear timer
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
Signals::Signal<>& Renderer::fadeComplete()
{
	return fadeCompleteSignal;
}


/**
 * Clears the screen with a given Color.
 */
void Renderer::clearScreen(uint8_t r, uint8_t g, uint8_t b)
{
	clearScreen({r, g, b});
}


int Renderer::width() const
{
	return size().x;
}


int Renderer::height() const
{
	return size().y;
}


void Renderer::size(int width, int height)
{
	size({width, height});
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
 * Sets a rectangular area of the screen outside of which nothing is drawn.
 *
 * \see clipRectClear()
 */
void Renderer::clipRect(float x, float y, float width, float height)
{
	clipRect({x, y, width, height});
}


/**
 * Clears the clipping rectangle.
 */
void Renderer::clipRectClear()
{
	clipRect({0, 0, 0, 0});
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
		drawBoxFilled(Rectangle<int>::Create({0, 0}, size()), mFadeColor.alphaFade(static_cast<uint8_t>(mCurrentFade)));
	}
}


void Renderer::setResolution(const Vector<float>& newResolution)
{
	if (!fullscreen())
	{
		mResolution = newResolution;
	}
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
