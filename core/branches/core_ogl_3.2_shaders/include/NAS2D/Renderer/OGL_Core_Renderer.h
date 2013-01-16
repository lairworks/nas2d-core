// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_OGL_CORE_RENDERER_
#define _NAS_OGL_CORE_RENDERER_

#ifdef _WIN32
#include "Glee.h"
#endif
#define NO_SDL_GLEXT
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include "SDL/SDL.h"
#endif

#include "Renderer.h"
#include "ShaderManager.h"

#include <map>

extern SDL_Window* _window;

/**
 * \class OGL_Core_Renderer
 * \brief OpenGL Renderer.
 *
 * Implements all Renderer functions with the SDL_opengl API.
 * Note: The use of glBegin()->glEnd() calls is now deprecated in OGL3. If we can switch
 * to a non-immediate mode methodollogy it may help us when splitting towards a pure 3D renderer.
 */
class OGL_Core_Renderer: public Renderer
{
public:
	OGL_Core_Renderer(const std::string title);
	
	//OGL_Core_Renderer(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);

	~OGL_Core_Renderer();
	
	void drawImage(Image& image, float x, float y, float scale);
	void drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height);
	
	void drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees);
	void drawImageRotated(Image& image, float x, float y, float degrees, int r, int g, int b, int a, float scale);
	void drawImageStretched(Image& image, float x, float y, float w, float h, int r, int g, int b, int a);
	
	void drawImageRepeated(Image& image, float x, float y, float w, float h);
	
	void drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint);
	
	void drawPixel(float x, float y, int r, int g, int b, int a);
	void drawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, int line_width);
	void drawBox(float x, float y, float width, float height, int r, int g, int b, int a);
	void drawBoxFilled(float x, float y, float width, float height, int r, int g, int b, int a);
	void drawCircle(float x, float y, float radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y);
	
	void drawText(Font& font, const std::string& text, float x, float y, int r, int g, int b, int a);
	void drawTextClamped(Font& font, const std::string& text, float rasterX, float rasterY, float x, float y, float w, float h, int r, int g, int b, int a);

	void clearScreen(int r, int g, int b);
    
	float width();
	float height();

	void update();

private:

	OGL_Core_Renderer(const OGL_Core_Renderer&);				// Intentionally left undefined;
	OGL_Core_Renderer& operator=(const OGL_Core_Renderer&);	// Intentionally left undefined;

	void initGL();
	void initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync);
	bool checkExtensions();
	bool extensionExists(const std::string& extension);
	
	void buildDisplayModeList();
	
	void fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	void fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v);

	void drawVertexArray(GLuint textureId, bool defaultTextureCoords = true);

	void getError();
	
	SDL_Window*			mWindow;					/**< Primary window. */
	SDL_GLContext		mContext;					/**< Primary OpenGL render context. */
	//SDL_Renderer*		mRenderer;					/**< SDL2 Renderer object associated with the OGL Context/Window. */

	GLuint				mVertexArray[12];			/**< Vertex array for quad drawing functions (all blitter functions). */
	GLfloat				mTextureCoordArray[8];		/**< Texture coordinate arrawy for quad drawing functions (all blitter functions). */
	
	GLuint				mTextureTarget;				/**< Target to bind textures to. Generally going to be GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB */
		GLuint				mVertexBufferObject[2];	/**< COMMENT ME! */
		GLuint				mTextureBufferObject;	/**< COMMENT ME! */
	
	unsigned int		mFontShaderFrag;
	unsigned int		mFontShaderVert;
	
	ShaderManager		*mShaderManager;
};	

#endif
