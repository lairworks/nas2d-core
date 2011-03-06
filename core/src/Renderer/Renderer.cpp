// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
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
						mCurrentTick(0),
						mLastTick(0)
{
}


/**
 * Internal constructor used by derived types to set the name of the Renderer.
 * 
 * This c'tor is not public and can't be invoked externally.
 */
Renderer::Renderer(const string& rendererName):	mLetterBoxHeight(0),
												mCinematic(false),
												mLetterbox(false),
												mRendererName(rendererName),
												mFadeColor(COLOR_BLACK),
												mFade(Renderer::FADE_NONE),
												mFadeStep(0.0f),
												mCurrentFade(0.0f),
												mCurrentTick(0),
												mLastTick(0)
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
 * Sets the title used for the application window.
 * 
 * \param	title	A \c std::string to use for the window title.
 */
void Renderer::setApplicationTitle(const std::string& title)
{}


/**
 * Draws an Image to the screen.
 *
 * \param	image	A reference to an Image Resource.
 * \param	x		X-Coordinate of the Image to draw.
 * \param	y		Y-Coordinate of the Image to draw.
 * \param	scale	Scale to draw the Image at. Default is 1.0 (no scaling).
 */
void Renderer::drawImage(Image& image, int x, int y, float scale)
{}


/**
 * Draws a portion of a given Image to the screen.
 *
 * \param	image		A refernece to an Image Resource.
 * \param	rasterX		X-Coordinate to draw the Image at.
 * \param	rasterY		Y-Coordinate to draw the Image at.
 * \param	imgX		X-Coordinate of the area to start getting pixel data from.
 * \param	imgY		Y-Coordinate of the area to start getting pixel data from.
 * \param	imgWidth	Width of the area to start getting pixel data from.
 * \param	imgHeight	Height of the area to start getting pixel data from.
 */
void Renderer::drawSubImage(Image& image, int rasterX, int rasterY, int imgX, int imgY, int imgWidth, int imgHeight)
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
void Renderer::drawImageRotated(Image& image, int x, int y, float degrees, Color_4ub color, float scale)
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
void Renderer::drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale)
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
void Renderer::drawImageStretched(Image& image, int x, int y, int w, int h, Color_4ub color)
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
void Renderer::drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a)
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
void Renderer::drawImageRepeated(Image& image, int x, int y, int w, int h)
{}


/**
 * Comment me!
 */
void Renderer::drawImageRect(int x, int y, int w, int h, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight)
{
	// Draw the center area if it's defined.
	drawImageRepeated(center, x + topLeft.getWidth(), y + topLeft.getHeight(), w - topRight.getWidth() - topLeft.getWidth(), h - topLeft.getHeight() - bottomLeft.getHeight());

	// Draw the sides
	drawImageRepeated(top, x + topLeft.getWidth(), y, w - topLeft.getWidth() - topRight.getWidth(), top.getHeight());
	drawImageRepeated(bottom, x + bottomLeft.getWidth(), y + h - bottom.getHeight(), w - bottomLeft.getWidth() - bottomRight.getWidth(), bottom.getHeight());
	drawImageRepeated(left, x, y + topLeft.getHeight(), left.getWidth(), h - topLeft.getHeight() - bottomLeft.getHeight());
	drawImageRepeated(right, x + w - right.getWidth(), y + topRight.getHeight(), right.getWidth(), h - topRight.getHeight() - bottomRight.getHeight());

	// Draw the corners
	drawImage(topLeft, x, y);
	drawImage(topRight, x + w - topRight.getWidth(), y);
	drawImage(bottomLeft, x, y + h - bottomLeft.getHeight());
	drawImage(bottomRight, x + w - bottomRight.getWidth(), y + h - bottomRight.getHeight());
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
void Renderer::drawImageRect(int x, int y, int w, int h, ImageList &images)
{
	// We need 9 images in order to render a rectangle, one for each corner, one for each edge and one for the background.
	if(images.size() == 9)
		drawImageRect(x, y, w, h, images[0], images[1], images[2], images[3], images[4], images[5], images[6], images[7], images[8]);
}


/**
 * Draws a source image to a destination image.
 * 
 * \param	source		A reference to a source Image.
 * \param	srcRect		A source retangle to grab from the source Image.
 * \param	destination	A reference to the destination Image.
 * \param	dstPoint	A point indicating where to draw the source Image on the destination Image.
 */
void Renderer::drawImageToImage(Image& source, const Rectangle_2d& srcRect, Image& destination, const Point_2d& dstPoint)
{}


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
 * \param	delayTime	Time in miliseconds the fade should last. A value of 0
 *						will instantly fade the screen in.
 */
void Renderer::fadeIn(int delay)
{
	if(delay == 0)
	{
		mCurrentFade = 0.0f;
		mFade = Renderer::FADE_NONE;
		return;
	}

	mFade = Renderer::FADE_IN;
	mFadeStep = 255.0f / static_cast<float>(delay);
}


/**
 * Non-blocking screen fade.
 *
 * \param	delayTime	Time in miliseconds the fade should last. A value of 0
 *						will instantly fade the screen in.
 */
void Renderer::fadeOut(int delay)
{
	if(delay == 0)
	{
		mCurrentFade = 255.0f;
		mFade = Renderer::FADE_NONE;
		return;
	}

	mFade = Renderer::FADE_OUT;
	mFadeStep = 255.0f / static_cast<float>(delay);
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
const string& Renderer::getLastError()
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
void Renderer::drawPixel(int x, int y, const Color_4ub& color)
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
void Renderer::drawPixel(int x, int y, int r, int g, int b, int a)
{}


/**
 * Draws a line from (x, y) - (x2, y2) on the primary surface.
 *
 * \param	x		X-Coordinate of the start of the line.
 * \param	y		Y-Coordinate of the start of the line.
 * \param	x2		X-Coordinate of the end of the line.
 * \param	y2		Y-Coordinate of the end of the line.
 * \param	color	A reference to a Color_4ub.
 */
void Renderer::drawLine(int x, int y, int x2, int y2, const Color_4ub& color, int line_width)
{
	drawLine(x, y, x2, y2, color.red(), color.green(), color.blue(), color.alpha(), line_width);
}


/**
 * Draws a line from (x, y) - (x2, y2) on the primary surface.
 *
 * \param	x		X-Coordinate of the start of the line.
 * \param	y		Y-Coordinate of the start of the line.
 * \param	x2		X-Coordinate of the end of the line.
 * \param	y2		Y-Coordinate of the end of the line.
 * \param	r		Red Color Value. Must be between 0 - 255.
 * \param	g		Green Color Value. Must be between 0 - 255.
 * \param	b		Blue Color Value. Must be between 0 - 255.
 * \param	a		Alpha Value. Must be between 0 - 255. Defaults to 255.
 */
void Renderer::drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a, int line_width)
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
void Renderer::drawBox(int x, int y, int w, int h, int r, int g, int b, int a)
{}


/**
 * Fills a given area with a solid color on the primary surface.
 *
 * \param	x	X-Coordinate of the area to fill.
 * \param	y	Y-Coordinate of the area to fill.
 * \param	w	Width, in pixels, of the area to fill.
 * \param	h	Height, in pixels, of the area to fill.
 * \param	r	Red Color Value. Must be between 0 - 255.
 * \param	g	Green Color Value. Must be between 0 - 255.
 * \param	b	Blue Color Value. Must be between 0 - 255.
 * \param	a	Alpha value. Must be between 0 - 255.
 */
void Renderer::drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a)
{}


/**
 * Gets the RGBA color of a pixel at X, Y in an Image as an unsigned 32-Bit Integer.
 *
 * \param src	A reference to an Image.
 * \param x		X-Coordinate of the pixel to inspect.
 * \param y		Y-Coordinate of the pixel to inspect.
 */
unsigned int Renderer::getPixelColor(Image& src, int x, int y)
{
	return 0;
}


/**
 * Determines if a pixel at X, Y in an Image as completely transparent.
 *
 * \param src	A reference to an Image.
 * \param x		X-Coordinate of the pixel to inspect.
 * \param y		Y-Coordinate of the pixel to inspect.
 */
bool Renderer::isPixelTransparent(Image& src, int x, int y)
{
	return true;
}



/**
 * Returns the primary display's Width.
 */
int Renderer::getWidth()
{
	return 0;
}


/**
 * Returns the primary display's Height.
 */
int Renderer::getHeight()
{
	return 0;
}


/**
 * Gets the current screen resolution as a Point_2d.
 */
Point_2d Renderer::getScreenResolution()
{
	return Point_2d(getWidth(), getHeight());
}


/**
 * Gets the center X-Coordinate of the screen.
 */
int Renderer::getScreenCenterX()
{
	return getWidth() >> 1;
}


/**
 * Gets the center Y-Coordinate of the screen.
 */
int  Renderer::getScreenCenterY()
{
	return getHeight() >> 1;
}


/**
 * Builds a list of available display modes.
 */
void Renderer::buildDisplayModeList()
{}


/**
 * Returns the name of the Renderer.
 */
const string& Renderer::getName()
{
	return mRendererName;
}


/**
 * Returns the name of the driver as named by the operating system.
 */
const string& Renderer::getDriverName()
{
	return mDriverName;
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
void Renderer::drawText(Font& font, const string& text, int x, int y, int r, int g, int b, int a)
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
void Renderer::drawTextClamped(Font& font, const string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a)
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
void Renderer::drawTextShadow(Font& font, const string& text, int x, int y, int distance, int r, int g, int b, int sr, int sg, int sb, int a )
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
void Renderer::drawTextShadowClamped(Font& font, const string& text, int rasterX, int rasterY, int x, int y, int w, int h, int distance, int r, int g, int b, int sr, int sg, int sb, int a)
{
	drawTextClamped(font, text, rasterX + distance, rasterY + distance, x, y, w, h, sr, sg, sb, a);
	drawTextClamped(font, text, rasterX, rasterY, x, y, w, h, r, g, b, a);
}


/**
 * Permanently desaturates a given Image.
 * 
 * \param	image	A reference to an Image Resource.
 */
void Renderer::imageDesaturate(Image& image)
{}


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
	mLastTick = mCurrentTick;
	mCurrentTick = mTimer.ms();

	if(mFade != Renderer::FADE_NONE)
	{
		float fade = (mCurrentTick - mLastTick) * mFadeStep;

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
		drawBoxFilled(0, 0, getWidth(), getHeight(), mFadeColor.red(), mFadeColor.green(), mFadeColor.blue(), (int)mCurrentFade);
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