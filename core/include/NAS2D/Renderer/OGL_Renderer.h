// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_OGL_RENDERER_
#define _NAS_OGL_RENDERER_


#include "Renderer.h"

#include "SDL/SDL.h"

#include <map>


/**
 * \class OGL_Renderer
 * \brief OpenGL Renderer.
 *
 * Implements all Renderer functions with the SDL_opengl API.
 * Note: The use of glBegin()->glEnd() calls is now deprecated in OGL3. If we can switch
 * to a non-immediate mode methodollogy it may help us when splitting towards a pure 3D renderer.
 */
class OGL_Renderer: public Renderer
{
public:
	OGL_Renderer();
	OGL_Renderer(unsigned int ResX, unsigned int ResY, unsigned int BPP, bool fullscreen, bool vsync);

	~OGL_Renderer();

	void setApplicationTitle(const std::string& title);
	
	void drawImage(Image& image, int x, int y, float scale);
	void drawSubImage(Image& image, int rasterX, int rasterY, int imgX, int imgY, int imgWidth, int imgHeight);

	void drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale);
	void drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a);

	void drawImageRepeated(Image& image, int x, int y, int w, int h);

	void drawImageToImage(Image& source, const Rectangle_2d& srcRect, Image& destination, const Point_2d& dstPoint);

	void drawPixel(int x, int y, int r, int g, int b, int a);
	void drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a, int line_width);
	void drawBox(int x, int y, int width, int height, int r, int g, int b, int a);
	void drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a);
	void drawCircle(int cx, int cy, int radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y);

	void drawText(Font& font, const std::string& text, int x, int y, int r, int g, int b, int a);
	void drawTextClamped(Font& font, const std::string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a);

	void imageDesaturate(Image& image);

	void clearScreen(int r, int g, int b);

    unsigned int getPixelColor(Image& src, int x, int y);
	bool isPixelTransparent(Image& src, int x, int y);
    
	int getWidth();
	int getHeight();

	bool valid() const;

	void update();

private:
	/**
	 * \typedef	TextureArray
	 * 
	 * Implemented as a map with an int as a key and a pair as a value.
	 * 
	 * The key is a numeric identifier. In practice we use the address of an image
	 * object as the identifier. The assumption is that no two Image objects will
	 * ever have the same address. This could prove buggy when switching between scenes.
	 * 
	 * The value is an integer pair with the OpenGL Texture ID as the first value and
	 * a tick count since last use as the second value.
	 */
	typedef std::map<int, pair<unsigned int, int> > TextureArray;

	OGL_Renderer(const OGL_Renderer&);				// Intentionally left undefined;
	OGL_Renderer& operator=(const OGL_Renderer&);	// Intentionally left undefined;

	void initGL();
	void initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);
	void checkExtensions();
	bool extensionExists(const std::string& extension);
	
	void buildDisplayModeList();
	
	bool drawVertexArray(Image& image, GLfloat vertexArray[], GLfloat textureCoord[], float scale, bool repeat);
	bool drawVertexArrayText(GLuint texture, GLfloat vertexArray[], GLfloat textureCoord[], float scale);

	inline GLuint getTextureId(Image& image);
	unsigned int generateTexture(SDL_Surface *src);
	inline void updateTextures();
	
	GLenum getTextureFormat(SDL_Surface *src);

	void getError();
	
	SDL_Surface			*mScreen;				/**< Primary screen surface. */
	unsigned int		mLastResourceCheck;		/**< Tick count from last resource check. */
	TextureArray		mTextureArray;			/**< Internal array of GL Texture ID and time stamp. */
	
	unsigned int		mTextureTarget;			/**< COMMENT ME! */
	//	GLfloat				mVertexBufferObject;	/**< COMMENT ME! */
	//	GLfloat				mTextureBufferObject;

	bool				mRequirementsMet;		/**< Flag indicating that all extensions requirements have been met. */
};	

#endif
