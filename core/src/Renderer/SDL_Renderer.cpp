// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Trig.h"
#include "NAS2D/Renderer/SDL_Renderer.h"

#ifdef __APPLE__
#include "SDL_gfx/SDL_gfxPrimitives.h"
#include "SDL_gfx/SDL_rotozoom.h"
#else
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#endif

int TEXTURE_FILTER_TYPE = 1;

SDL_Renderer::SDL_Renderer():	Renderer("SDL Renderer"),
								mScreen(NULL)
{
	cout << "Starting " << mRendererName << "..." << endl;

	Configuration& cf = Utility<Configuration>::get();
	initVideo(cf.graphicsWidth(), cf.graphicsHeight(), cf.graphicsColorDepth(), cf.fullscreen(), cf.vsync());

	if(cf.graphicsTextureQuality() == GRAPHICS_FAST)
		TEXTURE_FILTER_TYPE = 0;
	else
		TEXTURE_FILTER_TYPE = 1;
}


SDL_Renderer::SDL_Renderer(unsigned int ResX, unsigned int ResY, unsigned int BPP, bool fullscreen, bool vsync):	Renderer("SDL Renderer"),
																													mScreen(NULL)
{
	cout << "Starting " << mRendererName << "..." << endl;

	initVideo(ResX, ResY, BPP, fullscreen, vsync);
}


SDL_Renderer::~SDL_Renderer()
{
	if(mScreen)
	{
		SDL_FreeSurface(mScreen);
		mScreen = NULL;
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


void SDL_Renderer::setApplicationTitle(const std::string& title)
{
	SDL_WM_SetCaption(title.c_str(), title.c_str());
}


unsigned int SDL_Renderer::pixelColor(Image& src, int x, int y)
{
	SDL_LockSurface(src.pixels());
    int bpp = src.pixels()->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)src.pixels()->pixels + y * src.pixels()->pitch + x * bpp;

	SDL_UnlockSurface(src.pixels());

	switch(bpp)
	{
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			// We should never ever get here. If we do, then we've got
			// a real problem.
			assert(bpp == 1 || bpp == 2 || bpp == 3 || bpp == 4);
			return 0;
	}
}


bool SDL_Renderer::pixelTransparent(Image& src, int x, int y)
{
	Uint8 r, g, b, a;
	SDL_GetRGBA(pixelColor(src, x, y), src.pixels()->format, &r, &g, &b, &a);

	return (a == 0);
}


int SDL_Renderer::width()
{
	return mScreen->w;
}


int SDL_Renderer::height()
{
	return mScreen->h;
}


void SDL_Renderer::buildDisplayModeList()
{
	mDisplayModes.push_back(DisplayMode(640, 480));
	mDisplayModes.push_back(DisplayMode(800, 600));
	mDisplayModes.push_back(DisplayMode(1024, 768));
	mDisplayModes.push_back(DisplayMode(1152, 864));
	mDisplayModes.push_back(DisplayMode(1280, 1024));
	mDisplayModes.push_back(DisplayMode(1440, 900));
}


void SDL_Renderer::drawImage(Image& image, int x, int y, float scale = 1.0f)
{
	if(scale != 1.0f)
		return drawImageStretched(image, x, y, static_cast<int>(image.width() * scale), static_cast<int>(image.height() * scale), 255, 255, 255, 255);

	SDL_Rect blitRect = {x, y, image.width(), image.height()};
	SDL_BlitSurface(image.pixels(), NULL, mScreen, &blitRect);
}


void SDL_Renderer::drawSubImage(Image& image, int rasterX, int rasterY, int imgX, int imgY, int imgWidth, int imgHeight)
{
	SDL_Rect blitRect = {rasterX, rasterY, imgWidth, imgHeight};
	SDL_Rect grabRect = {imgX, imgY, imgWidth, imgHeight};

	SDL_BlitSurface(image.pixels(), &grabRect, mScreen, &blitRect);
}

/**
 * Note that this version of the rotate function is <I>slow</I>
 */
void SDL_Renderer::drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale)
{
	SDL_Surface* rotated = rotozoomSurface(image.pixels(), -degrees, scale, TEXTURE_FILTER_TYPE);

	if(rotated)
	{
		int offsetX = (rotated->w >> 1) - (image.width() >> 1);
		int offsetY = (rotated->h >> 1) - (image.height() >> 1);

		SDL_Rect blitRect = {x - offsetX, y - offsetY, rotated->w, rotated->h};
		SDL_BlitSurface(rotated, NULL, mScreen, &blitRect);

		SDL_FreeSurface(rotated);
	}
}


void SDL_Renderer::drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a)
{
	double zoomedWidth = (static_cast<double>(w) / static_cast<double>(image.width()));
	double zoomedHeight = (static_cast<double>(h) / static_cast<double>(image.height()));

	SDL_Surface* zoomed = zoomSurface(image.pixels(), zoomedWidth, zoomedHeight, TEXTURE_FILTER_TYPE);
	if(zoomed)
	{
		SDL_Rect blitRect = {x, y, zoomed->w, zoomed->h};
		SDL_BlitSurface(zoomed, NULL, mScreen, &blitRect);

		SDL_FreeSurface(zoomed);
	}
}


void SDL_Renderer::drawImageRepeated(Image& image, int x, int y, int w, int h)
{
	int imageWidth = image.width();
	int imageHeight = image.height();

	if(imageWidth == 0 || imageHeight == 0)
		return;

	int dh = 0, dw = 0;
	for (int positionY = 0; positionY < h; positionY += imageHeight)     // Y position on pattern plane
	{
		if(positionY + imageHeight > h)
			dh = h - positionY;
		else
			dh = imageHeight;

		int srcY = 0;
		int dstY = y + positionY;

		for (int positionX = 0; positionX < w; positionX += imageWidth) // X position on pattern plane
		{
			if(positionX + imageWidth > w)
				dw = w - positionX;
			else
				dw = imageWidth;

			int srcX = 0;
			int dstX = x + positionX;

			SDL_Rect dstRect;
			SDL_Rect srcRect;
			dstRect.x = dstX; dstRect.y = dstY;
			srcRect.x = srcX; srcRect.y = srcY;
			srcRect.w = dw;   srcRect.h = dh;

			SDL_BlitSurface(image.pixels(), &srcRect, mScreen, &dstRect);
		}
	}
}


void SDL_Renderer::drawImageToImage(Image& source, const Rectangle_2d& srcRect, Image& destination, const Point_2d& dstPoint)
{
	// Blit the source surface to the destination surface.
	SDL_Rect sRect = { srcRect.x, srcRect.y, srcRect.w, srcRect.h };
	SDL_Rect dRect = { dstPoint.x, dstPoint.y, 0, 0 };

	SDL_BlitSurface(source.pixels(), &sRect, destination.pixels(), &dRect);
}


void SDL_Renderer::update()
{
	// If Letterbox mode is toggled, render the letterboxes.
	if(mLetterbox)
	{
		SDL_Rect blitRect = {0, 0, mScreen->w, mLetterBoxHeight};
		SDL_FillRect(SDL_GetVideoSurface(), &blitRect, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));

		blitRect.x = 0;
		blitRect.y = mScreen->h - mLetterBoxHeight;
		blitRect.w = mScreen->w;
		blitRect.h = mLetterBoxHeight;
		SDL_FillRect(SDL_GetVideoSurface(), &blitRect, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));
	}

	Renderer::update();
	SDL_Flip(mScreen);
}


void SDL_Renderer::clearScreen(int r, int g, int b)
{
	SDL_FillRect(SDL_GetVideoSurface(), NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b));
}


void SDL_Renderer::drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a)
{
	SDL_Rect fillRect = {x, y, width, height};

	if(a < 255)
	{
		SDL_Surface *mPrimarySurface = SDL_GetVideoSurface();
		SDL_Surface *fillSurface = SDL_CreateRGBSurface(mPrimarySurface->flags, width, height, mPrimarySurface->format->BitsPerPixel, mPrimarySurface->format->Rmask, mPrimarySurface->format->Gmask, mPrimarySurface->format->Bmask, mPrimarySurface->format->Amask);

		SDL_FillRect(fillSurface, NULL, SDL_MapRGBA(SDL_GetVideoSurface()->format, r, g, b, a));
		SDL_SetAlpha(fillSurface, SDL_SRCALPHA, a);

		SDL_BlitSurface(fillSurface, NULL, mScreen, &fillRect);

		SDL_FreeSurface(fillSurface);
	}
	else
		SDL_FillRect(mScreen, &fillRect, SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b));
}


void SDL_Renderer::drawText(Font& font, const string& text, int x, int y, int r, int g, int b, int a)
{
	if(!font.loaded() || text.empty())
		return;

	SDL_Color color = {r, g, b, 0};
	SDL_Rect blitRect = {x, y, 0, 0};

	SDL_Surface *text_surface;
	if(!(text_surface = TTF_RenderText_Blended(font.font(), text.c_str(), color)))
	{
		pushMessage(SDL_GetError());
	}
	else
	{
		SDL_BlitSurface(text_surface, NULL, mScreen, &blitRect);
		SDL_FreeSurface(text_surface);
	}
}


void SDL_Renderer::drawTextClamped(Font& font, const string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a)
{
	if(!font.loaded() || text.empty())
		return;

	SDL_Color color = {r, g, b, 0};

	SDL_Rect blitRect = {rasterX, rasterY, w, h};
	SDL_Rect clipRect = {x, y, w, h};

	SDL_Surface *text_surface;
	if(!(text_surface = TTF_RenderText_Blended(font.font(), text.c_str(), color)))
	{
		pushMessage(SDL_GetError());
	}
	else
	{
		SDL_BlitSurface(text_surface, &clipRect, mScreen, &blitRect);
		SDL_FreeSurface(text_surface);
	}
}


void SDL_Renderer::drawPixel(int x, int y, int r, int g, int b, int a)
{
	pixelRGBA(mScreen, x, y, r, g, b, a);
}


void SDL_Renderer::drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a, int line_width = 1)
{
	lineRGBA(mScreen, x, y, x2, y2, r, g, b, a);
}


void SDL_Renderer::drawBox(int x, int y, int width, int height, int r, int g, int b, int a)
{
	hlineRGBA(mScreen, x, x + width, y, r, g, b, a);
	vlineRGBA(mScreen, x, y, y + height, r, g, b, a);
	hlineRGBA(mScreen, x, x + width, y + height, r, g, b, a);
	vlineRGBA(mScreen, x + width, y, y + height, r, g, b, a);
}


void SDL_Renderer::drawCircle(int cx, int cy, int radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y)
{
	float theta = PI_2 / static_cast<float>(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = static_cast<float>(radius);
	float y = 0;

	float prvX = 0, prvY = 0;
	float plotX = x * scale_x + cx, plotY = y * scale_y + cy;

	for(int ii = 0; ii <= num_segments; ii++)
	{
		prvX = plotX ; prvY = plotY;

		// Apply the rotation matrix
		t = x;
		x = (c * x) - (s * y);
		y = (s * t) + (c * y);

		plotX = x * scale_x + cx;
		plotY = y * scale_y + cy;

		drawLine(static_cast<int>(plotX) , static_cast<int>(plotY), static_cast<int>(prvX), static_cast<int>(prvY), r, g, b, a);
	}
}


void SDL_Renderer::desaturate(Image& image)
{
	#if defined(_DEBUG)
		if(!image.loaded())
		{
			stringstream str;
			str << "Image '" << image.name() << "' is not loaded and cannot be desaturated. " << image.errorMessage();
			pushMessage(str.str());
			return;
		}
	#endif

	SDL_Surface *surface = image.pixels();
	for(int y = 0; y < image.height(); y++)
	{
		for(int x = 0; x < image.width(); x++)
		{
			Uint8 r, g, b, a;

			SDL_GetRGBA(pixelColor(image, x, y), surface->format, &r, &g, &b, &a);

			Uint8 grey = (r + g + b) / 3;

			pixelRGBA(surface, x, y, grey, grey, grey, a);
		}
	}
}


void SDL_Renderer::initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync)
{
	SDL_putenv("SDL_VIDEO_CENTERED=center");

	// Set the app title to blank until the client sets it.
	setApplicationTitle(" ");

	// Initialize SDL's Video Subsystems.
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(1321, "Error starting SDL Video Library", SDL_GetError());


    char videoDriverName[256];
	if(SDL_VideoDriverName(videoDriverName, 256) != NULL)
		mDriverName = videoDriverName;
	else
		mDriverName = "UNSPECIFIED";

	cout << "\tVideo Driver: " << mDriverName << endl;

	// Set up our Video Surface flags and build onto it with the
	// Config class. We should probably throw this into a private
	// function which builds the flags list and returns a Uint32.
	Uint32 sdlFlags = 0;
	sdlFlags = SDL_HWSURFACE|SDL_HWACCEL;
	//sdlFlags = SDL_SWSURFACE;
	if(fullscreen)
		sdlFlags = sdlFlags|SDL_FULLSCREEN;

	if(vsync)
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );

	mScreen = SDL_SetVideoMode(resX, resY, bpp, sdlFlags);
	if(mScreen == 0)
		throw Exception(1322, "Error setting Video Mode", SDL_GetError());

	SDL_ShowCursor(false);

	// Set our LetterBox height to 15% of the screen's height.
	mLetterBoxHeight = (int)((mScreen->h) * 0.15);
	
	buildDisplayModeList();

	if (TTF_Init() < 0)
		throw Exception(1323, "Error starting TrueType Library", TTF_GetError());
}
