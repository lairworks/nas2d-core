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
#include "NAS2D/Renderer/OGL_Core_Renderer.h"

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#elif defined(WIN32)
#include "SDL/SDL_opengl.h"
#else
#include "SDL/SDL_opengl.h"
#endif

#include <iostream>
#include <math.h>

using namespace std;


/**
 * Life of generated OpenGL Textures in Miliseconds. Roughly 30 seconds
 */
const unsigned int TEXTURE_EXPIRE_TIME = 30000;

/**
 * Vertex coordinate pairs. Default vertex coordinates used for initializing OpenGL and for debugging.
 */
GLfloat DEFAULT_VERTEX_COORDS[8] =	{ 0.0f, 0.0f,  0.0f, 32.0f,  32.0f, 32.0f,  32.0f, 0.0f };

/**
 * Texture coordinate pairs. Default coordinates encompassing the entire texture.
 */
GLfloat DEFAULT_TEXTURE_COORDS[8] =	{ 0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f };

GLfloat POINT_VERTEX_ARRAY[2] = { 0.0f, 0.0f };


GraphicsQuality TEXTURE_FILTER = GRAPHICS_GOOD;



// UGLY ASS HACK!
// Until I do this properly, for now I'm leaving this as a global
// so that we can handle mouse input grabbing.
SDL_Window* _window = NULL;


OGL_Core_Renderer::OGL_Core_Renderer(const std::string title):	Renderer("OpenGL Renderer", title),
mWindow(NULL),
mTextureTarget(0)
{
	cout << "Starting " << name() << ":" << endl;
	
	Configuration& cf = Utility<Configuration>::get();
	
	TEXTURE_FILTER = cf.graphicsTextureQuality();
	
	initVideo(cf.graphicsWidth(), cf.graphicsHeight(), cf.graphicsColorDepth(), cf.fullscreen(), cf.vsync());
	
	// Set our LetterBox height to 15% of the screen's height.
	mLetterBoxHeight = (int)((width()) * 0.15);
	
}


OGL_Core_Renderer::~OGL_Core_Renderer()
{
	//glDeleteBuffers(1, &mVertexBufferObject);
	
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	_window == NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	
	//delete mShaderManager;
}

void OGL_Core_Renderer::clearScreen(int r, int g, int b)
{
	glClearColor((GLfloat)r / 255, (GLfloat)g / 255, (GLfloat)b / 255, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT);
}

void OGL_Core_Renderer::drawVertexArray(GLuint textureId, bool defaultTextureCoords)
{
	clearScreen(0, 0, 0);
	
	/* We're going to create a simple diamond made from lines */
    const GLfloat vert[4][2] = {
		{  0.0,  1.0  }, /* Top point */
		{  1.0,  0.0  }, /* Right point */
		{  0.0, -1.0  }, /* Bottom point */
		{ -1.0,  0.0  } }; /* Left point */
	
	
	const GLfloat colors[4][3] = {
		{  1.0,  0.0,  0.0  }, /* Red */
		{  0.0,  1.0,  0.0  }, /* Green */
		{  0.0,  0.0,  1.0  }, /* Blue */
		{  1.0,  1.0,  1.0  } }; /* White */
	
	glUseProgram(mShaderManager->getShaderProgram());
	
	glGenVertexArrays(1, &mVertexArray[0]);
	glBindVertexArray(mVertexArray[0]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	
	glVertexAttribPointer(
						  0,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
						  0,
						  0
						  );
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

	
	glBindAttribLocation(mShaderManager->getShaderProgram(), 0, "in_Position");
    glBindAttribLocation(mShaderManager->getShaderProgram(), 1, "in_Color");
	
	
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2);
	glDisableVertexAttribArray(0);
}

/**
 * Used internally to fill the vertex array with quad vertex information.
 */
void OGL_Core_Renderer::fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	mVertexArray[0] = static_cast<GLfloat>(x),		mVertexArray[1] = static_cast<GLfloat>(y);
	mVertexArray[2] = static_cast<GLfloat>(x),		mVertexArray[3] = static_cast<GLfloat>(y + h);
	mVertexArray[4] = static_cast<GLfloat>(x + w),	mVertexArray[5] = static_cast<GLfloat>(y + h);
	mVertexArray[6] = static_cast<GLfloat>(x + w),	mVertexArray[7] = static_cast<GLfloat>(y);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArray), mVertexArray, GL_STATIC_DRAW);
}


/**
 * Used internally to fill the texture coordinate array with quad vertex information.
 */
void OGL_Core_Renderer::fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v)
{
	mTextureCoordArray[0] = static_cast<GLfloat>(x),	mTextureCoordArray[1] = static_cast<GLfloat>(y);
	mTextureCoordArray[2] = static_cast<GLfloat>(x),	mTextureCoordArray[3] = static_cast<GLfloat>(v);
	mTextureCoordArray[4] = static_cast<GLfloat>(u),	mTextureCoordArray[5] = static_cast<GLfloat>(v);
	mTextureCoordArray[6] = static_cast<GLfloat>(u),	mTextureCoordArray[7] = static_cast<GLfloat>(y);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(mTextureCoordArray), mTextureCoordArray, GL_STATIC_DRAW);
}


void OGL_Core_Renderer::drawImage(Image& image, float x, float y, float scale = 1.0f)
{
	fillVertexArray(x, y, static_cast<float>(image.width()), static_cast<float>(image.height()));
	drawVertexArray(image.texture_id(), false);
}


void OGL_Core_Renderer::drawSubImage(Image& image, float rasterX, float rasterY, float x, float y, float width, float height)
{
	fillVertexArray(rasterX, rasterY, width, height);
	
	fillTextureArray(	x / image.width(),
					 y / image.height(),
					 x / image.width() + width / image.width(),
					 y / image.height() + height / image.height()
					 );
	
	drawVertexArray(image.texture_id(), false);
}


void OGL_Core_Renderer::drawSubImageRotated(Image& image, float rasterX, float rasterY, float x, float y, float width, float height, float degrees)
{
	//glPushMatrix();
	
	// Find center point of the image.
	float tX = width / 2.0f;
	float tY = height / 2.0f;
	
	// Adjust the translation so that images appear where expected.
	//glTranslatef(rasterX + tX, rasterY + tY, 0.0f);
	//glRotatef(degrees, 0.0f, 0.0f, 1.0f);
	
	fillVertexArray(-tX, -tY, tX * 2, tY * 2);
	
	fillTextureArray(	x / image.width(),
					 y / image.height(),
					 x / image.width() + width / image.width(),
					 y / image.height() + height / image.height()
					 );
	
	drawVertexArray(image.texture_id(), false);
	
	//glPopMatrix();
}


void OGL_Core_Renderer::drawImageRotated(Image& image, float x, float y, float degrees, int r, int g, int b, int a, float scale)
{
	//glPushMatrix();
	
	// Find center point of the image.
	int imgHalfW = (image.width() / 2);
	int imgHalfH = (image.height() / 2);
	
	float tX = imgHalfW * scale;
	float tY = imgHalfH * scale;
	
	// Adjust the translation so that images appear where expected.
	//glTranslatef(x + imgHalfW, y + imgHalfH, 0.0f);
	//glRotatef(degrees, 0.0f, 0.0f, 1.0f);
	
	//glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	fillVertexArray(-tX, -tY, tX * 2, tY * 2);
	
	drawVertexArray(image.texture_id());
	//glPopMatrix();
	
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Core_Renderer::drawImageStretched(Image& image, float x, float y, float w, float h, int r, int g, int b, int a)
{
	fillVertexArray(x, y, w, h);	
	drawVertexArray(image.texture_id());
}


void OGL_Core_Renderer::drawImageRepeated(Image& image, float x, float y, float w, float h)
{
	fillVertexArray(x, y, w, h);
	
	// Change texture mode to repeat at edges.
//	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	drawVertexArray(image.texture_id(), true);
	
//	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void OGL_Core_Renderer::drawImageToImage(Image& source, Image& destination, const Point_2df& dstPoint)
{
	// Ignore the call if the detination point is outside the bounds of destination image.
	if(dstPoint.x > destination.width() || dstPoint.y > destination.height())
		return;
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glBindTexture(mTextureTarget, destination.texture_id());
	
	Rectangle_2d clipRect;
	
	(dstPoint.x + source.width()) > destination.width() ? clipRect.w = source.width() - ((dstPoint.x + source.width()) - destination.width()) : clipRect.w = source.width();
	(dstPoint.y + source.height()) > destination.height() ? clipRect.h = source.height() - ((dstPoint.y + source.height()) - destination.height()) : clipRect.h = source.height();
	
	// Ignore this call if the clipping rect is smaller than 1 pixel in any dimension.
	if(clipRect.w < 1 || clipRect.h < 1)
		return;
	
	
	unsigned int fbo = destination.fbo_id();
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, mTextureTarget, destination.texture_id(), 0);
	
	// Flip the Y axis to keep images drawing correctly.
	fillVertexArray(dstPoint.x, destination.height() - dstPoint.y, clipRect.w, -clipRect.h);
	
	drawVertexArray(source.texture_id());
	glBindTexture(mTextureTarget, destination.texture_id());
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
}


void OGL_Core_Renderer::drawPixel(float x, float y, int r, int g, int b, int a)
{
	//glDisable(mTextureTarget);
	
	//glColor4ub(r, g, b, a);
	
	POINT_VERTEX_ARRAY[0] = x + 0.5f; POINT_VERTEX_ARRAY[1] = y + 0.5f;
	
	glVertexPointer(2, GL_FLOAT, 0, POINT_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, 1);
	
	//glEnable(mTextureTarget);
	
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Core_Renderer::drawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, int line_width = 1)
{
	//glDisable(mTextureTarget);
	
	GLfloat verts[12] =	{
		x, y + 1.0f,
		x2, y2 + 1.0f,
		x - 0.5f, y + 0.5f,
		x2 - 0.5f, y2 + 0.5f,
		x, y,
		x2, y2
	};
	
	//glColor4ub(r, g, b, a);
	
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	
	//glEnable(mTextureTarget);
	
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


/*
 * The below code originally comes from http://slabode.exofire.net/circle_draw.shtml.
 *
 * Modified to support X/Y scaling to draw an ellipse.
 */
void OGL_Core_Renderer::drawCircle(float cx, float cy, float radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y)
{
	//glDisable(mTextureTarget);
	
	//glColor4ub(r, g, b, a);
	
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
	 We should consider a basic array lookup table approach which will
	 eliminate the alloc/dealloc overhead (at the cost of increased code
	 size).
	 */
	delete [] verts;
	verts = 0;
	
	//glEnable(mTextureTarget);
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Core_Renderer::drawBox(float x, float y, float width, float height, int r, int g, int b, int a)
{
	//glColor4ub(r, g, b, a);
	//glDisable(mTextureTarget);
	
	fillVertexArray(x, y, width, height);
	glVertexPointer(2, GL_FLOAT, 0, mVertexArray);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
	//glEnable(mTextureTarget);
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Core_Renderer::drawBoxFilled(float x, float y, float width, float height, int r, int g, int b, int a)
{
	//glColor4ub(r, g, b, a);
	//glDisable(mTextureTarget);
	
	fillVertexArray(x, y, width, height);
	drawVertexArray(0);
	
	//glEnable(mTextureTarget);
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Core_Renderer::drawText(Font& font, const std::string& text, float x, float y, int r, int g, int b, int a)
{
	// Ignore if font isn't loaded or string is empty
	if(!font.loaded() || text.empty())
		return;
	
	//glColor4ub(r, g, b, a);
	
	int offset = 0;
	int	cellSize = font.glyphCellSize();
	Font::GlyphMetrics gm;
	
	
	for(size_t i = 0; i < text.size(); i++)
	{
		gm = font.glyphMetrics(static_cast<int>(text[i]));
		
		fillVertexArray(x + offset, y, cellSize, cellSize);
		fillTextureArray(gm.uvX, gm.uvY, gm.uvW, gm.uvH);
		
		drawVertexArray(font.texture_id(), false);
		offset += gm.advance + gm.minX;
	}
	
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Core_Renderer::drawTextClamped(Font& font, const std::string& text, float rasterX, float rasterY, float x, float y, float w, float h, int r, int g, int b, int a)
{
	// Ignore if font isn't loaded or string is empty
	if(!font.loaded() || text.empty())
		return;
	
	//glColor4ub(r, g, b, a);
	
	int offset = 0;
	int	cellSize = font.glyphCellSize();
	Font::GlyphMetrics gm;
	
	for(int i = 0; i < text.size(); i++)
	{
		gm = font.glyphMetrics(static_cast<int>(text[i]));
		int temp = (w - (x + offset));
		
		if(x + offset + gm.advance <= w)
		{
			fillVertexArray(rasterX + x + offset, rasterY + y, cellSize, cellSize);
			fillTextureArray(gm.uvX, gm.uvY, gm.uvW, gm.uvH);
			cout << "Ins:\t" << x + offset + gm.advance << "\t" << text[i]<< "\t" << temp << "\t" << gm.advance << "\t" << temp - gm.advance << "\t" << x + offset + (temp - gm.advance) << endl;
		}
		else if(x + offset + (gm.advance - temp) <= w && temp > 0)
		{
			fillVertexArray(rasterX + x + offset, rasterY + y, cellSize, cellSize);
			fillTextureArray(gm.uvX, gm.uvY, static_cast<float>(temp)/static_cast<float>(cellSize), gm.uvH);
			cout << "Out:\t" << gm.uvX << "\t" << gm.uvW << "\t" << gm.uvY << "\t" << gm.uvH << "\t" << static_cast<float>(temp)/static_cast<float>(cellSize) << endl;
		}
		
		drawVertexArray(font.texture_id(), false);
		offset += gm.advance;
	}
	
	//glColor4ub(255, 255, 255, 255); // Reset color back to normal.
	//drawText(font, text, rasterX, rasterY, r, g, b, a); // replace with appropriate drawing code.
	
	// please finish me
}


void OGL_Core_Renderer::update()
{
	getError();
	Renderer::update();
	//SDL_GL_SwapBuffers();
	SDL_GL_SwapWindow(mWindow);
}


float OGL_Core_Renderer::width()
{
    //return static_cast<float>(mScreen->w);
	int n = 0;
	SDL_GetWindowSize(mWindow, &n, NULL);
	return static_cast<float>(n);
}


float OGL_Core_Renderer::height()
{
    //return static_cast<float>(mScreen->h);
	int n = 0;
	SDL_GetWindowSize(mWindow, NULL, &n);
	return static_cast<float>(n);
}


void OGL_Core_Renderer::buildDisplayModeList()
{
	mDisplayModes.push_back(DisplayMode(640, 480));
	mDisplayModes.push_back(DisplayMode(800, 600));
	mDisplayModes.push_back(DisplayMode(1024, 768));
	mDisplayModes.push_back(DisplayMode(1152, 864));
	mDisplayModes.push_back(DisplayMode(1280, 1024));
	mDisplayModes.push_back(DisplayMode(1440, 900));
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;
	
    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}
void OGL_Core_Renderer::getError()
{
	
	std::string errStr = "OpenGL Error: ";
	
	switch(glGetError())
	{
		case GL_NO_ERROR:
			return;
			break;
		case GL_INVALID_ENUM:
			errStr += "Invalid Enumerator.";
			break;
		case GL_INVALID_VALUE:
			errStr += "Invalid Value.";
			break;
		case GL_INVALID_OPERATION:
			errStr += "Invalid Operation.";
			break;
		case GL_OUT_OF_MEMORY:
			errStr += "Out of Memory.";
			break;
		default:
			errStr += "Unknown Error Code.";
			break;
	}
	
	stringstream str;
	str << errStr << endl;
	
	cout << str.str();
	
	pushMessage(str.str());
}


/**
 * Checks for expected extensions and prints warning messages
 * if the video driver doesn't support the extension.
 */
bool OGL_Core_Renderer::extensionExists(const std::string& extension)
{
	std::istringstream iss(std::string(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS))));
	
	StringList extensions;
	string token;
	while(getline(iss, token, ' '))
		extensions.push_back(token);
	
	if(extensions.empty())
		return false;
	
	for(size_t i = 0; i < extensions.size(); i++)
		if(extensions[i] == extension)
			return true;
	
	return false;
}


bool OGL_Core_Renderer::checkExtensions()
{
	cout << "\tOpenGL Extensions:" << endl;
	
	// Check extension and set our texture target
	if(extensionExists("GL_ARB_texture_non_power_of_two"))
	{
		cout << "\t\t- GL_ARB_texture_non_power_of_two" << endl;
		mTextureTarget = GL_TEXTURE_2D;
		return true;
	}
	else
		return false;
}


void OGL_Core_Renderer::initGL()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Spit out system graphics information.
	cout << "\t- OpenGL System Info -" << endl;
	
	driverName((char*)glGetString(GL_RENDERER));
	
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << driverName() << endl;
	cout << "\tDriver Version: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	
//	if(!checkExtensions())
//		throw Exception(0, "Graphics Requirements Not Met", "Your graphics driver does not meet the minimum requirements.");
	
	glGenBuffers(2, mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DEFAULT_VERTEX_COORDS), DEFAULT_VERTEX_COORDS, GL_STATIC_DRAW);
	
	glGenBuffers(1, &mTextureBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mTextureBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DEFAULT_TEXTURE_COORDS), DEFAULT_TEXTURE_COORDS, GL_STATIC_DRAW);
}


void OGL_Core_Renderer::initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync)
{
	// Initialize SDL's Video Subsystems.
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(701, "Error starting SDL Video Library", SDL_GetError());
	
	
	// Setup OpenGL parameters
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	/// \todo	Add checks to determine an appropriate depth buffer.
	
	if(vsync)
		SDL_GL_SetSwapInterval(1);
	
	// Anti-aliasing... need to test this.
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	Uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	
	if(fullscreen)
		sdlFlags = sdlFlags | SDL_WINDOW_FULLSCREEN;
	
	mWindow = SDL_CreateWindow(title().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, sdlFlags);
	
	if(!mWindow)
	{
		cout << "EXCEPTION: Could not create a Window: " << SDL_GetError() << endl;
		throw Exception(0, "Window Creation Failed", "Unable to create a window.");
	}
	
	_window = mWindow;
	
	mContext = SDL_GL_CreateContext(mWindow);
	
	if(!mContext)
	{
		cout << "EXCEPTION: Could not create an OpenGL Context: " << SDL_GetError() << endl;
		throw Exception(0, "Can't create OGL Context", "Unable to create an OpenGL Context.\n" + string(SDL_GetError()));
	}
	
	
	void* ptr = SDL_GL_GetProcAddress("glDrawArrays");
	
	
	SDL_ShowCursor(false);
	
	buildDisplayModeList();
	mShaderManager = new ShaderManager();
	initGL();
}