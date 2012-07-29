// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_OGL_RENDERER_
#define _NAS_OGL_RENDERER_


#include "Renderer.h"

#include "SDL/SDL.h"

#include "ShaderManager.h"

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
	
	//OGL_Renderer(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);

	~OGL_Renderer();

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
	bool checkExtensions();
	bool extensionExists(const std::string& extension);
	
	void buildDisplayModeList();
	
	void fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	void fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v);

	inline void fillVertexArray(int x, int y, int w, int h);
	inline void fillTextureArray(int x, int y, int u, int v);
	
	void drawVertexArray(GLuint textureId, bool defaultTextureCoords, bool repeat);

	void getError();
	
	SDL_Surface			*mScreen;					/**< Primary screen surface. */
	TextureArray		mTextureArray;				/**< Internal array of GL Texture ID and time stamp. */

	GLfloat				mVertexArray[8];			/**< Vertex array for quad drawing functions (all blitter functions). */
	GLfloat				mTextureCoordArray[8];		/**< Texture coordinate arrawy for quad drawing functions (all blitter functions). */
	
	GLuint				mTextureTarget;				/**< Target to bind textures to. Generally going to be GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB */
	//	GLfloat				mVertexBufferObject;	/**< COMMENT ME! */
	//	GLfloat				mTextureBufferObject;	/**< COMMENT ME! */
	
	ShaderManager		*mShaderManager;
};	

#endif
