// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Trig.h"
#include "NAS2D/Renderer/OGL_Renderer.h"

#include "NAS2D/Configuration.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Resources/FontInfo.h"
#include "NAS2D/Resources/ImageInfo.h"
#include "NAS2D/Utility.h"

#ifdef WINDOWS
#define NO_SDL_GLEXT
#include "GL/glew.h"
#include "SDL.h"
#elif __APPLE__
#include <SDL2/SDL.h>
#elif __linux__
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#else
#include "SDL2.h"
#endif


#include <iostream>
#include <math.h>

using namespace NAS2D;
using namespace NAS2D::exception;

/** Vertex coordinate pairs. Default vertex coordinates used for initializing OpenGL and for debugging. */
GLfloat DEFAULT_VERTEX_COORDS[8] =	{ 0.0f, 0.0f,  0.0f, 32.0f,  32.0f, 32.0f,  32.0f, 0.0f };

/** Texture coordinate pairs. Default coordinates encompassing the entire texture. */
GLfloat DEFAULT_TEXTURE_COORDS[12] = { 0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f };

GLfloat POINT_VERTEX_ARRAY[2] = { 0.0f, 0.0f };

/** Color value array for four verts. Defaults to white or normal color. */
GLfloat COLOR_VERTEX_ARRAY[16] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };


GLfloat		VERTEX_ARRAY[12]		= {};	/**< Vertex array for quad drawing functions (all blitter functions). */
GLfloat		TEXTURE_COORD_ARRAY[12]	= {};	/**< Texture coordinate array for quad drawing functions (all blitter functions). */

GraphicsQuality TEXTURE_FILTER = GRAPHICS_GOOD;

// UGLY ASS HACK!
// This is required here in order to remove OpenGL implementation details from Image and Font.
extern std::map<std::string, ImageInfo>	IMAGE_ID_MAP;
extern std::map<std::string, FontInfo> FONTMAP;

// UGLY ASS HACK!
// This is required for mouse grabbing in the EventHandler class.
SDL_Window*			_WINDOW = nullptr;

SDL_GLContext		CONTEXT;					/**< Primary OpenGL render context. */

void fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v);
void drawVertexArray(GLuint textureId, bool defaultTextureCoords = true);

void line(float x1, float y1, float x2, float y2, float w, float Cr, float Cg, float Cb, float Ca);
GLuint generate_fbo();


OGL_Renderer::OGL_Renderer(const std::string title) : Renderer("OpenGL Renderer", title)
{
	std::cout << "Starting " << name() << ":" << std::endl;
	
	Configuration& cf = Utility<Configuration>::get();
	TEXTURE_FILTER = cf.graphicsTextureQuality();
	initVideo(cf.graphicsWidth(), cf.graphicsHeight(), cf.graphicsColorDepth(), cf.fullscreen(), cf.vsync());
}


OGL_Renderer::~OGL_Renderer()
{
	SDL_GL_DeleteContext(CONTEXT);
	SDL_DestroyWindow(_WINDOW);
	_WINDOW = nullptr;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	std::cout << "OpenGL Renderer Terminated." << std::endl;
}


void OGL_Renderer::drawImage(Image& image, float x, float y, float scale, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);

	fillVertexArray(x, y, static_cast<float>(image.width()), static_cast<float>(image.height()));
	fillTextureArray(0.0, 0.0, 1.0, 1.0);
	drawVertexArray(IMAGE_ID_MAP[image.name()].texture_id);
}


void OGL_Renderer::drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);

	fillVertexArray(rasterX, rasterY, width, height);

	fillTextureArray(	x / image.width(),
						y / image.height(),
						x / image.width() + width / image.width(),
						y / image.height() + height / image.height()
					);

	drawVertexArray(IMAGE_ID_MAP[image.name()].texture_id, false);
}


void OGL_Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees, int r, int g, int b, int a)
{
	glPushMatrix();

	// Find center point of the image.
	float tX = width / 2.0f;
	float tY = height / 2.0f;

	// Adjust the translation so that images appear where expected.
	glTranslatef(rasterX + tX, rasterY + tY, 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	glColor4ub(r, g, b, a);

	fillVertexArray(-tX, -tY, tX * 2, tY * 2);

	fillTextureArray(	x / image.width(),
						y / image.height(),
						x / image.width() + width / image.width(),
						y / image.height() + height / image.height()
					);

	drawVertexArray(IMAGE_ID_MAP[image.name()].texture_id, false);

	glPopMatrix();
}


void OGL_Renderer::drawImageRotated(Image& image, float x, float y, float degrees, int r, int g, int b, int a, float scale)
{
	glPushMatrix();

	// Find center point of the image.
	int imgHalfW = (image.width() / 2);
	int imgHalfH = (image.height() / 2);

	float tX = imgHalfW * scale;
	float tY = imgHalfH * scale;

	// Adjust the translation so that images appear where expected.
	glTranslatef(x + imgHalfW, y + imgHalfH, 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	fillVertexArray(-tX, -tY, tX * 2, tY * 2);
	
	drawVertexArray(IMAGE_ID_MAP[image.name()].texture_id);
	glPopMatrix();
}


void OGL_Renderer::drawImageStretched(Image& image, float x, float y, float w, float h, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	fillVertexArray(x, y, w, h);
	drawVertexArray(IMAGE_ID_MAP[image.name()].texture_id);
}


void OGL_Renderer::drawImageRepeated(Image& image, float x, float y, float w, float h)
{
	glColor4ub(255, 255, 255, 255);
	fillVertexArray(x, y, w, h);

	// Change texture mode to repeat at edges.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	drawVertexArray(IMAGE_ID_MAP[image.name()].texture_id, true);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void OGL_Renderer::drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint)
{
	glColor4ub(255, 255, 255, 255);

	// Ignore the call if the detination point is outside the bounds of destination image.
	if(dstPoint.x() > destination.width() || dstPoint.y() > destination.height())
		return;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, IMAGE_ID_MAP[destination.name()].texture_id);

	Rectangle_2d clipRect;

	(static_cast<int>(dstPoint.x()) + source.width()) > destination.width() ? clipRect.width(source.width() - ((static_cast<int>(dstPoint.x()) + source.width()) - destination.width())) : clipRect.width(source.width());
	(static_cast<int>(dstPoint.y()) + source.height()) > destination.height() ? clipRect.height(source.height() - ((static_cast<int>(dstPoint.y()) + source.height()) - destination.height())) : clipRect.height(source.height());

	// Ignore this call if the clipping rect is smaller than 1 pixel in any dimension.
	if(clipRect.width() < 1 || clipRect.height() < 1)
		return;

	GLuint fbo = IMAGE_ID_MAP[destination.name()].fbo_id;
	if (fbo == 0)
	{
		fbo = generate_fbo();
		IMAGE_ID_MAP[destination.name()].fbo_id = fbo;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, IMAGE_ID_MAP[destination.name()].texture_id, 0);

	// Flip the Y axis to keep images drawing correctly.
	fillVertexArray(dstPoint.x(), static_cast<float>(destination.height()) - dstPoint.y(), static_cast<float>(clipRect.width()), static_cast<float>(-clipRect.height()));

	drawVertexArray(IMAGE_ID_MAP[source.name()].texture_id);
	glBindTexture(GL_TEXTURE_2D, IMAGE_ID_MAP[destination.name()].texture_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}


void OGL_Renderer::drawPoint(float x, float y, int r, int g, int b, int a)
{
	glDisable(GL_TEXTURE_2D);

	glColor4ub(r, g, b, a);

	POINT_VERTEX_ARRAY[0] = x + 0.5f; POINT_VERTEX_ARRAY[1] = y + 0.5f;

	glVertexPointer(2, GL_FLOAT, 0, POINT_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, 1);

	glEnable(GL_TEXTURE_2D);
}


void OGL_Renderer::drawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, int line_width = 1)
{	
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	
	line(x, y, x2, y2, (float)line_width, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
}


/*
 * The below code originally comes from http://slabode.exofire.net/circle_draw.shtml.
 *
 * Modified to support X/Y scaling to draw an ellipse.
 */
void OGL_Renderer::drawCircle(float cx, float cy, float radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y)
{
	glDisable(GL_TEXTURE_2D);
	glColor4ub(r, g, b, a);

	float theta = PI_2 / static_cast<float>(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = radius;
	float y = 0;

	GLfloat* verts = new GLfloat[num_segments * 2]; // Two coords per vertex

	// During each iteration of the for loop, two indecies are accessed
	// so we need to be sure that we step two index places for each loop.
	for(int i = 0; i < num_segments * 2; i += 2)
	{
		verts[i]		= x * scale_x + cx;
		verts[i + 1]	= y * scale_y + cy;

		// Apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}

	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_LINE_LOOP, 0, num_segments);


	/**
	 * \todo	I really hate the alloc's/dealloc's that are done in this function.
	 * 			We should consider a basic array lookup table approach which will
	 * 			eliminate the alloc/dealloc overhead (at the cost of increased code
	 * 			size).
	 */
	delete [] verts;
	verts = 0;

	glEnable(GL_TEXTURE_2D);
}


void OGL_Renderer::drawGradient(float x, float y, float w, float h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int r3, int g3, int b3, int a3, int r4, int g4, int b4, int a4)
{
	glEnableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);

	COLOR_VERTEX_ARRAY[0] = r1 / 255.0f;
	COLOR_VERTEX_ARRAY[1] = g1 / 255.0f;
	COLOR_VERTEX_ARRAY[2] = b1 / 255.0f;
	COLOR_VERTEX_ARRAY[3] = a1 / 255.0f;

	COLOR_VERTEX_ARRAY[4] = r2 / 255.0f;
	COLOR_VERTEX_ARRAY[5] = g2 / 255.0f;
	COLOR_VERTEX_ARRAY[6] = b2 / 255.0f;
	COLOR_VERTEX_ARRAY[7] = a2 / 255.0f;

	COLOR_VERTEX_ARRAY[8] = r3 / 255.0f;
	COLOR_VERTEX_ARRAY[9] = g3 / 255.0f;
	COLOR_VERTEX_ARRAY[10] = b3 / 255.0f;
	COLOR_VERTEX_ARRAY[11] = a3 / 255.0f;

	COLOR_VERTEX_ARRAY[12] = r4 / 255.0f;
	COLOR_VERTEX_ARRAY[13] = g4 / 255.0f;
	COLOR_VERTEX_ARRAY[14] = b4 / 255.0f;
	COLOR_VERTEX_ARRAY[15] = a4 / 255.0f;


	fillVertexArray(x, y, w, h);
	glColorPointer(4, GL_FLOAT, 0, COLOR_VERTEX_ARRAY);
	drawVertexArray(0);

	glEnable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
}


void OGL_Renderer::drawBox(float x, float y, float width, float height, int r, int g, int b, int a)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);

	line(x, y, x + width, y, 1.0f, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	line(x, y, x, y + height + 0.5f, 1.0f, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	line(x, y + height + 0.5f, x + width, y + height + 0.5f, 1.0f, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	line(x + width, y, x + width, y + height + 0.5f, 1.0f, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::drawBoxFilled(float x, float y, float width, float height, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);
	glDisable(GL_TEXTURE_2D);

	fillVertexArray(x, y, width, height);
	drawVertexArray(0);

	glEnable(GL_TEXTURE_2D);
}


void OGL_Renderer::drawText(NAS2D::Font& font, const std::string& text, float x, float y, int r, int g, int b, int a)
{
	// Ignore if font isn't loaded or string is empty
	if(!font.loaded() || text.empty())
		return;

	glColor4ub(r, g, b, a);

	int offset = 0;
    NAS2D::Font::GlyphMetrics gm;
	for (size_t i = 0; i < text.size(); i++)
	{
		gm = font.glyphMetrics(static_cast<int>(text[i]));

		fillVertexArray(x + offset, y, (float)font.glyphCellWidth(), (float)font.glyphCellHeight());
		fillTextureArray(gm.uvX, gm.uvY, gm.uvW, gm.uvH);
		
		drawVertexArray(FONTMAP[font.name()].textureId, false);
		offset += gm.advance + gm.minX;
	}
}


void OGL_Renderer::clearScreen(int r, int g, int b)
{
	glClearColor((GLfloat)r / 255, (GLfloat)g / 255, (GLfloat)b / 255, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT);
}


void OGL_Renderer::update()
{
	Renderer::update();
	SDL_GL_SwapWindow(_WINDOW);
}


float OGL_Renderer::width()
{
	int n = 0;
	SDL_GetWindowSize(_WINDOW, &n, nullptr);
	return static_cast<float>(n);
}


float OGL_Renderer::height()
{
	int n = 0;
	SDL_GetWindowSize(_WINDOW, nullptr, &n);
	return static_cast<float>(n);
}


void OGL_Renderer::initGL()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)width(), (GLdouble)height(), 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Spit out system graphics information.
	std::cout << "\t- OpenGL System Info -" << std::endl;

	driverName((char*)glGetString(GL_RENDERER));

	std::cout << "\tVendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "\tRenderer: " << driverName() << std::endl;
	std::cout << "\tDriver Version: " << glGetString(GL_VERSION) << std::endl;

	std::string glsl_v = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (glsl_v.empty())
		throw renderer_no_glsl();

	std::cout << "\tGLSL Version: " << glsl_v << std::endl;

	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, DEFAULT_VERTEX_COORDS);
	glTexCoordPointer(2, GL_FLOAT, 0, DEFAULT_TEXTURE_COORDS);
}


void OGL_Renderer::initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw renderer_backend_init_failure(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	/// \todo	Add checks to determine an appropriate depth buffer.
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 4);

	if(vsync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);

	Uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	if(fullscreen)
		sdlFlags = sdlFlags | SDL_WINDOW_FULLSCREEN;

	_WINDOW = SDL_CreateWindow(title().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, sdlFlags);

	if (!_WINDOW)
		throw renderer_window_creation_failure();

	CONTEXT = SDL_GL_CreateContext(_WINDOW);
	if (!CONTEXT)
		throw renderer_opengl_context_failure();

	#ifdef __APPLE__
	if (vsync)
	{
		// FIXME: The SDL 2.0 method above may work fine which would allow us to pull this code.
		vsync = false;
		const GLint VBL = 1;
		CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &VBL);
	}
	#endif

	SDL_ShowCursor(false);
	initGL();
}



// ==================================================================================
// = NON PUBLIC IMPLEMENTATION
// ==================================================================================

/**
 * Generates an OpenGL Frame Buffer Object.
 */
GLuint generate_fbo()
{
	GLuint fbo = 0;
	glGenBuffers(1, &fbo);
	return fbo;
}


/**
 * Draws a textured rectangle using a vertex and texture coordinate array
 */
void drawVertexArray(GLuint textureId, bool defaultTextureCoords)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glVertexPointer(2, GL_FLOAT, 0, VERTEX_ARRAY);

	// Choose from the default texture coordinates or from a custom set.
	if (defaultTextureCoords) glTexCoordPointer(2, GL_FLOAT, 0, DEFAULT_TEXTURE_COORDS);
	else glTexCoordPointer(2, GL_FLOAT, 0, TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}


/**
 * Fills a vertex array with quad vertex information.
 */
void fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	VERTEX_ARRAY[0] = static_cast<GLfloat>(x), VERTEX_ARRAY[1] = static_cast<GLfloat>(y);
	VERTEX_ARRAY[2] = static_cast<GLfloat>(x), VERTEX_ARRAY[3] = static_cast<GLfloat>(y + h);
	VERTEX_ARRAY[4] = static_cast<GLfloat>(x + w), VERTEX_ARRAY[5] = static_cast<GLfloat>(y + h);

	VERTEX_ARRAY[6] = static_cast<GLfloat>(x + w), VERTEX_ARRAY[7] = static_cast<GLfloat>(y + h);
	VERTEX_ARRAY[8] = static_cast<GLfloat>(x + w), VERTEX_ARRAY[9] = static_cast<GLfloat>(y);
	VERTEX_ARRAY[10] = static_cast<GLfloat>(x), VERTEX_ARRAY[11] = static_cast<GLfloat>(y);
}


/**
 * Fills a texture coordinate array with quad vertex information.
 */
void fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v)
{
	TEXTURE_COORD_ARRAY[0] = static_cast<GLfloat>(x), TEXTURE_COORD_ARRAY[1] = static_cast<GLfloat>(y);
	TEXTURE_COORD_ARRAY[2] = static_cast<GLfloat>(x), TEXTURE_COORD_ARRAY[3] = static_cast<GLfloat>(v);
	TEXTURE_COORD_ARRAY[4] = static_cast<GLfloat>(u), TEXTURE_COORD_ARRAY[5] = static_cast<GLfloat>(v);

	TEXTURE_COORD_ARRAY[6] = static_cast<GLfloat>(u), TEXTURE_COORD_ARRAY[7] = static_cast<GLfloat>(v);
	TEXTURE_COORD_ARRAY[8] = static_cast<GLfloat>(u), TEXTURE_COORD_ARRAY[9] = static_cast<GLfloat>(y);
	TEXTURE_COORD_ARRAY[10] = static_cast<GLfloat>(x), TEXTURE_COORD_ARRAY[11] = static_cast<GLfloat>(y);
}


/**
 * The following code was developed by Chris Tsang and lifted from:
 * 
 * http://www.codeproject.com/KB/openGL/gllinedraw.aspx
 * 
 * Modified: Removed option for non-alpha blending and general code cleanup.
 * 
 * This is drop-in code that may be replaced in the future.
 */
static inline float _ABS(float x) { return x > 0 ? x : -x; }

void line(float x1, float y1, float x2, float y2, float w, float Cr, float Cg, float Cb, float Ca)
{
	// What are these values for?
	float t = 0.0f;
	float R = 0.0f;
	float f = w - static_cast<int>(w);
	
	// Alpha component?
	float A = Ca;

	// HOLY CRAP magic numbers!
	//determine parameters t,R
	if(w >= 0.0f && w < 1.0f)
	{
		t = 0.05f;
		R = 0.48f + 0.32f * f;
		
		A *= f;
	}
	else if(w >= 1.0f && w < 2.0f)
	{
		t = 0.05f + f * 0.33f;
		R = 0.768f + 0.312f * f;
	}
	else if(w >= 2.0f && w < 3.0f)
	{
		t = 0.38f + f * 0.58f;
		R = 1.08f;
	}
	else if(w >= 3.0f && w < 4.0f)
	{
		t = 0.96f + f * 0.48f;
		R = 1.08f;
	}
	else if(w >= 4.0f && w < 5.0f)
	{
		t = 1.44f + f * 0.46f;
		R = 1.08f;
	}
	else if(w >= 5.0f && w < 6.0f)
	{
		t = 1.9f + f * 0.6f;
		R = 1.08f;
	}
	else if(w >= 6.0f)
	{
		float ff = w - 6.0f;
		t = 2.5f + ff * 0.50f;
		R = 1.08f;
	}
	//printf( "w=%f, f=%f, C=%.4f\n", w,f,C);
	
	//determine angle of the line to horizontal
	float tx = 0.0f, ty = 0.0f; //core thinkness of a line
	float Rx = 0.0f, Ry = 0.0f; //fading edge of a line
	float cx = 0.0f, cy = 0.0f; //cap of a line
	float ALW = 0.01f;			// Dafuq is this?
	float dx = x2 - x1;
	float dy = y2 - y1;
	
	if(_ABS(dx) < ALW)
	{
		//vertical
		tx = t; ty = 0.0f;
		Rx = R; Ry = 0.0f;
		if(w > 0.0f && w <= 1.0f)
		{
			tx = 0.5f;
			Rx = 0.0f;
		}
	}
	else if(_ABS(dy) < ALW)
	{
		//horizontal
		tx = 0.0f; ty = t;
		Rx = 0.0f; Ry = R;
		if(w > 0.0f && w <= 1.0f)
		{
			ty = 0.5f;
			Ry = 0.0f;
		}
	}
	else
	{
		dx = y1 - y2;
		dy = x2 - x1;

		float L = sqrt(dx * dx + dy * dy);

		dx /= L;
		dy /= L;

		cx = -dy;
		cy = dx;
		
		tx = t * dx;
		ty = t * dy;
		
		Rx = R * dx;
		Ry = R * dy;
	}

	x1 += cx * 0.5f;
	y1 += cy * 0.5f;
	
	x2 -= cx * 0.5f;
	y2 -= cy * 0.5f;

	//draw the line by triangle strip
	float line_vertex[]=
	{
		x1-tx-Rx-cx, y1-ty-Ry-cy, //fading edge1
		x2-tx-Rx+cx, y2-ty-Ry+cy,
		x1-tx-cx,y1-ty-cy,	  //core
		x2-tx+cx,y2-ty+cy,
		x1+tx-cx,y1+ty-cy,
		x2+tx+cx,y2+ty+cy,
		x1+tx+Rx-cx, y1+ty+Ry-cy, //fading edge2
		x2+tx+Rx+cx, y2+ty+Ry+cy
	};

	glVertexPointer(2, GL_FLOAT, 0, line_vertex);

	float line_color[]=
	{
		Cr, Cg, Cb, 0,
		Cr, Cg, Cb, 0,
		Cr, Cg, Cb, Ca,
		Cr, Cg, Cb, Ca,
		Cr, Cg, Cb, Ca,
		Cr, Cg, Cb, Ca,
		Cr, Cg, Cb, 0,
		Cr, Cg, Cb, 0
	};

	glColorPointer(4, GL_FLOAT, 0, line_color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	// Line End Caps
	if(w > 3.0f) // <<< Arbitrary number.
	{
		float line_vertex[]=
		{
			x1-tx-Rx-cx, y1-ty-Ry-cy, //cap1
			x1-tx-Rx, y1-ty-Ry,
			x1-tx-cx, y1-ty-cy,
			x1+tx+Rx, y1+ty+Ry,
			x1+tx-cx, y1+ty-cy,
			x1+tx+Rx-cx, y1+ty+Ry-cy,
			x2-tx-Rx+cx, y2-ty-Ry+cy, //cap2
			x2-tx-Rx, y2-ty-Ry,
			x2-tx+cx, y2-ty+cy,
			x2+tx+Rx, y2+ty+Ry,
			x2+tx+cx, y2+ty+cy,
			x2+tx+Rx+cx, y2+ty+Ry+cy
		};
		glVertexPointer(2, GL_FLOAT, 0, line_vertex);

		float line_color[]=
		{
			Cr,Cg,Cb, 0, //cap1
			Cr,Cg,Cb, 0,
			Cr,Cg,Cb, Ca,
			Cr,Cg,Cb, 0,
			Cr,Cg,Cb, Ca,
			Cr,Cg,Cb, 0,		
			Cr,Cg,Cb, 0, //cap2
			Cr,Cg,Cb, 0,
			Cr,Cg,Cb, Ca,
			Cr,Cg,Cb, 0,
			Cr,Cg,Cb, Ca,
			Cr,Cg,Cb, 0
		};
		glColorPointer(4, GL_FLOAT, 0, line_color);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);
	}
}
