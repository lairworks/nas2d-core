// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_SDL_RENDERER_
#define _NAS_SDL_RENDERER_

#include "NAS2D/Renderer/Renderer.h"

#include "SDL/SDL.h"

/**
 * \class SDL_Renderer
 * \brief SDL Renderer.
 *
 * Implements all Renderer functions with the SDL API.
 */
class SDL_Renderer : public Renderer
{
public:
	SDL_Renderer();
	//SDL_Renderer(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);

	~SDL_Renderer();

	void setApplicationTitle(const std::string& title);
	
	void drawImage(Image& image, int x, int y, float scale);
	void drawSubImage(Image& image, int rasterX, int rasterY, int x, int y, int width, int height);

	void drawSubImageRotated(Image& image, int rasterX, int rasterY, int x, int y, int width, int height, float degrees);
	void drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale);
	void drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a);
	void drawImageRepeated(Image& image, int x, int y, int w, int h);

	void drawImageToImage(Image& source, Image& destination, const Point_2d& dstPoint);

	void drawPixel(int x, int y, int r, int g, int b, int a);
	void drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a, int line_width);
	void drawBox(int x, int y, int width, int height, int r, int g, int b, int a);
	void drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a);
	void drawCircle(int cx, int cy, int radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y);

	void drawText(Font& font, const std::string& text, int x, int y, int r, int g, int b, int a);
	void drawTextClamped(Font& font, const std::string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a);

	void clearScreen(int r, int g, int b);
    
	int width();
	int height();

	bool valid() const;

	void update();
private:
	SDL_Renderer(const SDL_Renderer&);				// Intentionally left undefined;
	SDL_Renderer& operator=(const SDL_Renderer&);	// Intentionally left undefined;

	void initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);

	void buildDisplayModeList();

	SDL_Surface *mScreen;		/**< Primary screen surface. */
};

#endif
