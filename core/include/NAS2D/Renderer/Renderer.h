// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_RENDERER_
#define _NAS_RENDERER_

#include "NAS2D/Common.h"
#include "NAS2D/Configuration.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Utility.h"
#include "NAS2D/Timer.h"

#include "NAS2D/Renderer/Primitives.h"

#include "NAS2D/Resources/Font.h"
#include "NAS2D/Resources/Image.h"

#include <sstream>

// Color Presets
extern const Color_4ub COLOR_BLACK;
extern const Color_4ub COLOR_BLUE;
extern const Color_4ub COLOR_BRIGHT_GREEN;
extern const Color_4ub COLOR_CYAN;
extern const Color_4ub COLOR_GREEN;
extern const Color_4ub COLOR_GREY;
extern const Color_4ub COLOR_MAGENTA;
extern const Color_4ub COLOR_NAVY;
extern const Color_4ub COLOR_ORANGE;
extern const Color_4ub COLOR_RED;
extern const Color_4ub COLOR_SILVER;
extern const Color_4ub COLOR_WHITE;
extern const Color_4ub COLOR_YELLOW;

extern const Color_4ub COLOR_NORMAL;


/**
 * \class Renderer
 * \brief Renderer base class.
 *
 * Provides a standard Renderer interface. The base Renderer can be used
 * but will act as a NULL interface.
 * 
 * \todo	Add support for color-keyed images.
 */
class Renderer
{
public:

	/**
	 * \class DisplayMode
	 * \brief Represents a display mode.
	 */
	class DisplayMode
	{
	public:
		/** C'tor */
		DisplayMode(int x, int y): screenWidth(x), screenHeight(y)
		{
			std::stringstream str;
			str << screenWidth << "x" << screenHeight;
			resolution = str.str();
		}

		std::string resolution;
		int screenWidth;
		int screenHeight;
	};

	/**
	 * \typedef	DisplayModes
	 * 
	 * List of display modes.
	 */
	typedef std::vector<DisplayMode> DisplayModes;
	
	Renderer();
	
	virtual ~Renderer();

	const std::string& getName();
	const std::string& getDriverName();
	const std::string& getLastError();

	virtual void setApplicationTitle(const std::string& title);

	virtual void drawImage(Image& image, int x, int y, float scale = 1.0f);
	virtual void drawSubImage(Image& image, int rasterX, int rasterY, int imgX, int imgY, int imgWidth, int imgHeight);

	void drawImageRotated(Image& image, int x, int y, float degrees, Color_4ub color = COLOR_NORMAL, float scale = 1.0f);
	virtual void drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale = 1.0f);

	void drawImageStretched(Image& image, int x, int y, int w, int h, Color_4ub color = COLOR_NORMAL);
	virtual void drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a);

	virtual void drawImageRepeated(Image& image, int x, int y, int w, int h);
	void drawImageRect(int x, int y, int w, int h, Image& topLeft, Image& top, Image& topRight, Image& left, Image& center, Image& right, Image& bottomLeft, Image& bottom, Image& bottomRight);
	void drawImageRect(int x, int y, int w, int h, ImageList& images);

	virtual void drawImageToImage(Image& source, const Rectangle_2d& srcRect, Image& destination, const Point_2d& dstPoint);

	void drawPixel(int x, int y, const Color_4ub& color = COLOR_WHITE);
	virtual void drawPixel(int x, int y, int r, int g, int b, int a = 255);

	void drawLine(int x, int y, int x2, int y2, const Color_4ub& color = COLOR_WHITE, int line_width = 1);
	virtual void drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a = 255, int line_width = 1);
	
	void drawBox(const Rectangle_2d& rect, int r, int g, int b, int a = 255);
	virtual void drawBox(int x, int y, int w, int h, int r, int g, int b, int a = 255);
	virtual void drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a = 255);
	
	virtual void drawCircle(int cx, int cy, int radius, int r, int g, int b, int a, int num_segments = 10, float scale_x = 1.0f, float scale_y = 1.0f) {}

	virtual void drawText(Font& font, const std::string& text, int x, int y, int r, int g, int b, int a = 255);
	void drawTextShadow(Font& font, const std::string& text, int x, int y, int sDistance, int r, int g, int b, int sr, int sg, int sb, int a = 255);
	virtual void drawTextClamped(Font& font, const std::string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a = 255);
	void drawTextShadowClamped(Font& font, const std::string& text, int rasterX, int rasterY, int x, int y, int w, int h, int distance, int r, int g, int b, int sr, int sg, int sb, int a = 255);

	virtual void desaturate(Image& image);

	void setFadeColor(const Color_4ub& color);
	void fadeIn(int delayTime);
	void fadeOut(int delayTime);
	bool isFading() const;
	bool isFaded() const;

	void toggleLetterBox();
	void toggleCinematicMode();

	void clearScreen(const Color_4ub& color);
	virtual void clearScreen(int r, int g, int b);
    
	virtual unsigned int pixelColor(Image& src, int x, int y);
	virtual bool pixelTransparent(Image& src, int x, int y);

	virtual int width();
	virtual int height();

	Point_2d getScreenResolution();

	int screenCenterX();
	int screenCenterY();
	
	virtual void buildDisplayModeList();
	const DisplayModes& getDisplayModes() const { return mDisplayModes; }

	virtual void update();

	void debug();


protected:
	Renderer(const std::string& rendererName);
	
	virtual void initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync) {}

	void pushMessage(const std::string& str);

	Timer				mTimer;				/**< Internal Timer. */

	std::string			mRendererName;		/**< Internal name of the Renderer. */
	std::string			mDriverName;		/**< OS Driver name */
	StringList			mMessages;			/**< List of messages. */
	DisplayModes		mDisplayModes;		/**< List of Screen resolutions. */

	int					mLetterBoxHeight;	/**< Height of Letter Box's */

	bool				mLetterbox;			/**< Toggles Letterbox Mode */
	bool				mCinematic;			/**< Toggles Cinematic Mode */

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

	enum FadeType
	{
		FADE_NONE,
		FADE_IN,
		FADE_OUT
	};

	Color_4ub			mFadeColor;			/**< Fade Color. */
	FadeType			mFade;				/**< Fade State. */
	float				mFadeStep;			/**< Amount of fading to do per milisecond. */
	float				mCurrentFade;		/**< Current fade amount. */

	int					mCurrentTick;		/**< Current tick in miliseconds. */
	int					mLastTick;			/**< Last tick in miliseconds. */
};

#endif