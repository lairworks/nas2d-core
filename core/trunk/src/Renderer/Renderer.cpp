// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Renderer.h"

// Preset Colors
const Color_4ub COLOR_BLACK(0, 0, 0, 255);
const Color_4ub COLOR_BLUE(0, 0, 255, 255);
const Color_4ub COLOR_BRIGHT_GREEN(0, 255, 0, 255);
const Color_4ub COLOR_CYAN(0, 255, 255, 255);
const Color_4ub COLOR_GREEN(0, 185, 0, 255);
const Color_4ub COLOR_GREY(128, 128, 128, 255);
const Color_4ub COLOR_MAGENTA(255, 0, 255, 255);
const Color_4ub COLOR_NAVY(35, 60, 85, 255);
const Color_4ub COLOR_ORANGE(255, 127, 0, 255);
const Color_4ub COLOR_RED(255, 0, 0, 255);
const Color_4ub COLOR_SILVER(192, 192, 192, 255);
const Color_4ub COLOR_WHITE(255, 255, 255, 255);
const Color_4ub COLOR_YELLOW(255, 255, 0, 255);

const Color_4ub COLOR_NORMAL = COLOR_WHITE;


/**
 * C'tor
 */
Renderer::Renderer():	mLetterBoxHeight(0),
						mCinematic(false),
						mLetterbox(false),
						mRendererName("DEBUG"),
						mFadeColor(COLOR_BLACK),
						mFade(Renderer::FADE_NONE),
						mFadeStep(0.0f),
						mCurrentFade(0.0f),
						mTickDelta(0)
{
}


/**
 * Internal constructor used by derived types to set the name of the Renderer.
 * 
 * This c'tor is not public and can't be invoked externally.
 */
Renderer::Renderer(const std::string& rendererName, const std::string& appTitle):	mLetterBoxHeight(0),
																					mCinematic(false),
																					mLetterbox(false),
																					mRendererName(rendererName),
																					mTitle(appTitle),
																					mFadeColor(COLOR_BLACK),
																					mFade(Renderer::FADE_NONE),
																					mFadeStep(0.0f),
																					mCurrentFade(0.0f),
																					mTickDelta(0)
{
}


/**
 * D'tor
 */
Renderer::~Renderer()
{
	// This is purely a debugging aid.
	#if defined(_DEBUG)
	if(!mMessages.empty())
	{
		cout << "Renderer contains the following " << mMessages.size() << " messages:" << endl;
		for(size_t i = 0; i < mMessages.size(); i++)
			cout << i << " Description: " << mMessages[i] << endl;
	}
	#endif

	cout << "Renderer Terminated." << endl;
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
{}


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
{}


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
 * \param	r			Red value to tint the image at (0 - 255).
 * \param	g			Green value to tint the image at (0 - 255).
 * \param	b			Blue value to tint the image at (0 - 255).
 * \param	a			Alpha value to draw the image at (0 - 255).
 */
void Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, int r, int g, int b, int a)
{}


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
 * Draws a rotated and scaled image.
 * 
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate to draw the Image at.
 * \param	y		Y-Coordinate to draw the Image at.
 * \param	degrees	Angle of rotation in degrees (0.0 - 360.0).
 * \param	r		Red value to tint the image at (0 - 255).
 * \param	g		Green value to tint the image at (0 - 255).
 * \param	b		Blue value to tint the image at (0 - 255).
 * \param	a		Alpha value to draw the image at (0 - 255).
 * \param	scale	Scale to draw the Image at. Default is 1.0 (no scaling).
 */
void Renderer::drawImageRotated(Image& image, float x, float y, float degrees, int r, int g, int b, int a, float scale)
{}


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
 * Draws a stretched image.
 * 
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate to draw the Image at.
 * \param	y		Y-Coordinate to draw the Image at.
 * \param	w		Width to use for drawing the Image.
 * \param	h		Height to use for drawing the Image.
 * \param	r		Red value to tint the image at (0 - 255).
 * \param	g		Green value to tint the image at (0 - 255).
 * \param	b		Blue value to tint the image at (0 - 255).
 * \param	a		Alpha value to draw the image at (0 - 255).
 */
void Renderer::drawImageStretched(Image& image, float x, float y, float w, float h, int r, int g, int b, int a)
{}


/**
 * Trevor, Comment Me!
 *
 * \param	image	A reference to an Image Resource.
 * \param	x		Comment me!
 * \param	y		Comment me!
 * \param	w		Comment me!
 * \param	h		Comment me!
 */
void Renderer::drawImageRepeated(Image& image, float x, float y, float w, float h)
{}


/**
 * Comment me!
 */
void Renderer::drawImageRect(float x, float y, float w, float h, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	// Draw the center area if it's defined.
	drawImageRepeated(center, x + topLeft.width(), y + topLeft.height(), w - topRight.width() - topLeft.width(), h - topLeft.height() - bottomLeft.height());

	// Draw the sides
	drawImageRepeated(top, x + topLeft.width(), y, w - topLeft.width() - topRight.width(), top.height());
	drawImageRepeated(bottom, x + bottomLeft.width(), y + h - bottom.height(), w - bottomLeft.width() - bottomRight.width(), bottom.height());
	drawImageRepeated(left, x, y + topLeft.height(), left.width(), h - topLeft.height() - bottomLeft.height());
	drawImageRepeated(right, x + w - right.width(), y + topRight.height(), right.width(), h - topRight.height() - bottomRight.height());

	// Draw the corners
	drawImage(topLeft, x, y);
	drawImage(topRight, x + w - topRight.width(), y);
	drawImage(bottomLeft, x, y + h - bottomLeft.height());
	drawImage(bottomRight, x + w - bottomRight.width(), y + h - bottomRight.height());
}


/**
 * Draws a rectangle using a series of set of images.
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
	if(images.size() == 9)
		drawImageRect(x, y, w, h, images[0], images[1], images[2], images[3], images[4], images[5], images[6], images[7], images[8]);
}


/**
 * Draws a source image to a destination image.
 * 
 * \param	source		A reference to a source Image.
 * \param	destination	A reference to the destination Image.
 * \param	dstPoint	A point indicating where to draw the source Image on the destination Image.
 */
void Renderer::drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint)
{}


/**
 * Draws a source image to a destination image.
 * 
 * \param	source		A reference to a source Image.
 * \param	destination	A reference to the destination Image.
 * \param	dstPoint	A point indicating where to draw the source Image on the destination Image.
 */
void Renderer::drawImageToImage(Image& source, Image& destination, const Point_2d& dstPoint)
{
	drawImageToImage(source, destination, Point_2df(dstPoint.x, dstPoint.y));
}


/**
 * Sets the color of the fade.
 * 
 * \param	color	A reference to aColor_4ub.
 */
void Renderer::setFadeColor(const Color_4ub& color)
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
	if(delay == 0)
	{
		mCurrentFade = 0.0f;
		mFade = Renderer::FADE_NONE;
		return;
	}

	mFade = Renderer::FADE_IN;
	mFadeStep = 255.0f /delay;

	mTimer.delta();	// clear timer
}


/**
 * Non-blocking screen fade.
 *
 * \param	delay	Time in miliseconds the fade should last. A value of 0
 *					will instantly fade the screen in.
 */
void Renderer::fadeOut(float delay)
{
	if(delay == 0)
	{
		mCurrentFade = 255.0f;
		mFade = Renderer::FADE_NONE;
		return;
	}

	mFade = Renderer::FADE_OUT;
	mFadeStep = 255.0f / delay;

	mTimer.delta(); // clear timer
}


/**
 * Gets whether or not a fade is in progress.
 */
bool Renderer::isFading() const
{
	return (mFade != Renderer::FADE_NONE);
}


/**
 * Gets whether the screen is faded or not.
 */
bool Renderer::isFaded() const
{
	return (mCurrentFade != 0.0f);
}


/**
 * Toggles LetterBox mode.
 */
void Renderer::toggleLetterBox()
{
	mLetterbox = !mLetterbox;

	#if defined(_DEBUG)
		mLetterbox ? cout << "Letterbox Mode on..." << endl : cout << "Letterbox Mode off..." << endl;
	#endif
}


/**
 * Toggles Cinematic Mode.
 */
void Renderer::toggleCinematicMode()
{
	mCinematic = !mCinematic;

	#if defined(_DEBUG)
		mCinematic ? cout << "Cinematic Mode on..." << endl : cout << "Cinematic Mode off..." << endl;
	#endif
}


/**
 * Returns a description of the last error message, if any.
 */
const std::string& Renderer::lastError()
{
	return mMessages.back();
}


/**
 * Draws a single Pixel to the primary surface.
 *
 * \param	x		X-Coordinate of the pixel to draw.
 * \param	y		Y-Coordinate of the pixel to draw.
 * \param	color	A references to a Color_4ub.
 */
void Renderer::drawPixel(float x, float y, const Color_4ub& color)
{
	drawPixel(x, y, color.red(), color.green(), color.blue(), color.alpha());
}


/**
 * Draws a single Pixel to the primary surface.
 *
 * \param	x	X-Coordinate of the pixel to draw.
 * \param	y	Y-Coordinate of the pixel to draw.
 * \param	r	Red Color Value. Must be between 0 - 255.
 * \param	g	Green Color Value. Must be between 0 - 255.
 * \param	b	Blue Color Value. Must be between 0 - 255.
 * \param	a	Alpha Value. Must be between 0 - 255. Defaults to 255.
 */
void Renderer::drawPixel(float x, float y, int r, int g, int b, int a)
{}


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
 * Draws a line from (x, y) - (x2, y2) on the primary surface.
 *
 * \param	x			X-Coordinate of the start of the line.
 * \param	y			Y-Coordinate of the start of the line.
 * \param	x2			X-Coordinate of the end of the line.
 * \param	y2			Y-Coordinate of the end of the line.
 * \param	r			Red Color Value. Must be between 0 - 255.
 * \param	g			Green Color Value. Must be between 0 - 255.
 * \param	b			Blue Color Value. Must be between 0 - 255.
 * \param	a			Alpha Value. Must be between 0 - 255. Defaults to 255.
 * \param	line_width	Width, in pixels, of the line to draw.
 */
void Renderer::drawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, int line_width)
{}


/**
 * Draws a hollow box on the primary surface.
 *
 * \param	rect	A reference to a Rectangle_2d defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBox(const Rectangle_2d& rect, int r, int g, int b, int a)
{
	drawBox(static_cast<float>(rect.x), static_cast<float>(rect.y), static_cast<float>(rect.w), static_cast<float>(rect.h), r, g, b, a);
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
void Renderer::drawBox(const Rectangle_2df& rect, int r, int g, int b, int a)
{
	drawBox(rect.x, rect.y, rect.w, rect.h, r, g, b, a);
}


/**
 * Draws a hollow box on the primary surface.
 *
 * \param	x	X-Coordinate of the box.
 * \param	y	Y-Coordinate of the box.
 * \param	w	Width, in pixels, of the box.
 * \param	h	Height, in pixels, of the box.
 * \param	r	Red Color Value. Must be between 0 - 255.
 * \param	g	Green Color Value. Must be between 0 - 255.
 * \param	b	Blue Color Value. Must be between 0 - 255.
 * \param	a	Alhpa value to use. Must be between 0 (completely transparent) - 255 (completely opaque).
 */
void Renderer::drawBox(float x, float y, float w, float h, int r, int g, int b, int a)
{}


/**
 * Fills a given area with a solid color.
 *
 * \param	rect	A reference to a Rectangle_2df defining the box dimensions.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255.
 */
void Renderer::drawBoxFilled(const Rectangle_2d& rect, int r, int g, int b, int a)
{
	drawBoxFilled(static_cast<float>(rect.x), static_cast<float>(rect.y), static_cast<float>(rect.w), static_cast<float>(rect.h), r, g, b, a);
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
void Renderer::drawBoxFilled(const Rectangle_2df& rect, int r, int g, int b, int a)
{
	drawBoxFilled(rect.x, rect.y, rect.w, rect.h, r, g, b, a);
}


/**
 * Fills a given area with a solid color.
 *
 * \param	x		X-Coordinate of the area to fill.
 * \param	y		Y-Coordinate of the area to fill.
 * \param	width	Width, in pixels, of the area to fill.
 * \param	height	Height, in pixels, of the area to fill.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha value. Must be between 0 - 255.
 */
void Renderer::drawBoxFilled(float x, float y, float width, float height, int r, int g, int b, int a)
{}


/**
 * Draws a circle.
 * 
 * \param	x				X-Coordinate of the center of the circle.
 * \param	y				Y-Coordinate of the center of the circle.
 * \param	radius			Radius of the circle.
 * \param	r				Red Color Value. Must be between 0 - 255.
 * \param	g				Green Color Value. Must be between 0 - 255.
 * \param	b				Blue Color Value. Must be between 0 - 255.
 * \param	a				Alpha value. Must be between 0 - 255.
 * \param	num_segments	Number of segments to use to draw the circle. The higher the number, the smoother the circle will appear.
 * \param	scale_x			Width scale of the circle. Used to draw ellipses.
 * \param	scale_y			Height scale of the circle. Used to draw ellipses.
 */
void Renderer::drawCircle(float x, float y, float radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y)
{}


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
 * \param	r1	Red value for point 1.
 * \param	g1	Green value for point 1.
 * \param	b1	Blue value for point 1.
 * \param	a1	Alpha value for point 1.
 * \param	r2	Red value for point 2.
 * \param	g2	Green value for point 2.
 * \param	b2	Blue value for point 2.
 * \param	a2	Alpha value for point 2.
 * \param	r3	Red value for point 3.
 * \param	g3	Green value for point 3.
 * \param	b3	Blue value for point 3.
 * \param	a3	Alpha value for point 3.
 * \param	r4	Red value for point 4.
 * \param	g4	Green value for point 4.
 * \param	b4	Blue value for point 4.
 * \param	a4	Alpha value for point 4.
 */
void Renderer::drawGradient(float x, float y, float w, float h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int r3, int g3, int b3, int a3, int r4, int g4, int b4, int a4)
{}


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
 * Returns the primary display's Width.
 */
float Renderer::width()
{
	return 0.0f;
}


/**
 * Returns the primary display's Height.
 */
float Renderer::height()
{
	return 0.0f;
}


/**
 * Gets the current screen resolution as a Point_2d.
 */
Point_2df Renderer::getScreenResolution()
{
	return Point_2df(width(), height());
}


/**
 * Gets the center X-Coordinate of the screen.
 */
float Renderer::screenCenterX()
{
	return width() / 2;
}


/**
 * Gets the center Y-Coordinate of the screen.
 */
float Renderer::screenCenterY()
{
	return height() / 2;
}


/**
 * Builds a list of available display modes.
 */
void Renderer::buildDisplayModeList()
{}


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
 * Writes debug information to the log file.
 */
void Renderer::debug()
{
	cout << endl;

	if(!mMessages.empty())
	{
		cout << "=== Renderer Debug Info ===" << endl;
		cout << mMessages.size() << " messages:" << endl;

		cout << endl;

		for(size_t i = 0; i < mMessages.size(); i++)
			cout << mMessages[i] << endl;
	}

	cout << endl;
}


/*
 * Renders Text in the Color specified.
 *
 * \param font		A reference to a Font Resource.
 * \param text		The text to draw.
 * \param x			X-Coordinate to render text string.
 * \param y			Y-Coordinate to render text string.
 * \param r			Red color value between 0 - 255.
 * \param g			Green color value between 0 - 255.
 * \param b			Blue color value between 0 - 255.
 * \param a			Alpha color value between 0 - 255.
 */
void Renderer::drawText(Font& font, const std::string& text, float x, float y, int r, int g, int b, int a)
{}


/*
 * Renders Text in the Color specified clamped within a specified width and height.
 *
 * \param font		A reference to a Font Resource.
 * \param text		The text to draw.
 * \param rasterX	X-Coordinate to render text string.
 * \param rasterY	Y-Coordinate to render text string.
 * \param x			X-Coordinate to grab from the rendered font area.
 * \param y			Y-Coordinate to grab from the rendered font area.
 * \param w			Maximum width of the target area that text should be rendered to.
 * \param h			Maximum Height of the target area that text should be rendered to.
 * \param r			Red color value between 0 - 255.
 * \param g			Green color value between 0 - 255.
 * \param b			Blue color value between 0 - 255.
 * \param a			Alpha color value between 0 - 255.
 */
void Renderer::drawTextClamped(Font& font, const std::string& text, float rasterX, float rasterY, float x, float y, float w, float h, int r, int g, int b, int a)
{}


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
void Renderer::drawTextShadow(Font& font, const std::string& text, float x, float y, int distance, int r, int g, int b, int sr, int sg, int sb, int a )
{
	drawText(font, text, x + distance, y + distance, sr, sg, sb, a);
	drawText(font, text, x, y, r, g, b, a);
}


/**
 * Renders a text string with a drop shadow.
 *
 * \param font		A reference to a Font Resource.
 * \param text		The text to draw.
 * \param rasterX	X-Coordinate to render text string.
 * \param rasterY	Y-Coordinate to render text string.
 * \param x			X-Coordinate to grab from the rendered font area.
 * \param y			Y-Coordinate to grab from the rendered font area.
 * \param w			Maximum width of the target area that text should be rendered to.
 * \param h			Maximum Height of the target area that text should be rendered to.
 * \param distance	Distance in pixels the drop shadow should be rendered.
 * \param r			Red color value between 0 - 255.
 * \param g			Green color value between 0 - 255.
 * \param b			Blue color value between 0 - 255.
 * \param sr		Red color value between 0 - 255.
 * \param sg		Green color value between 0 - 255.
 * \param sb		Blue color value between 0 - 255.
 * \param a			Alpha color value between 0 - 255.
 */
void Renderer::drawTextShadowClamped(Font& font, const std::string& text, float rasterX, float rasterY, float x, float y, float w, float h, int distance, int r, int g, int b, int sr, int sg, int sb, int a)
{
	drawTextClamped(font, text, rasterX + distance, rasterY + distance, x, y, w, h, sr, sg, sb, a);
	drawTextClamped(font, text, rasterX, rasterY, x, y, w, h, r, g, b, a);
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
 * Clears the screen with a given RGB value.
 *
 * \param r	Red color value between 0 - 255.
 * \param g	Green Color Value between 0 - 255.
 * \param b	Blue Color Value between 0 - 255.
 *
 */
void Renderer::clearScreen(int r, int g, int b)
{}


/**
 * Updates the screen.
 * 
 * \note	All derived Renderer objects must call Renderer::update()
 *			before performing screen refreshes.
 */
void Renderer::update()
{
	mTickDelta = mTimer.delta();

	if(mFade != Renderer::FADE_NONE)
	{
		float fade = mTickDelta * mFadeStep;

		if(mFade == Renderer::FADE_IN)
		{
			mCurrentFade -= fade;

			if(mCurrentFade < 0.0f)
			{
				mCurrentFade = 0.0f;
				mFade = Renderer::FADE_NONE;
			}
		}
		else
		{
			mCurrentFade += fade;

			if(mCurrentFade > 255.0f)
			{
				mCurrentFade = 255.0f;
				mFade = Renderer::FADE_NONE;
			}
		}
	}

	if(mCurrentFade > 0.0f)
		drawBoxFilled(0, 0, width(), height(), mFadeColor.red(), mFadeColor.green(), mFadeColor.blue(), (int)mCurrentFade);
}


/**
 * Adds a new message to Renderer's message list.
 */
void Renderer::pushMessage(const std::string& str)
{
	if(mMessages.empty())
	{
		mMessages.push_back(str);
		return;
	}
	
	if(str != mMessages.back())
	{
		cout << str;
		mMessages.push_back(str);
	}
}