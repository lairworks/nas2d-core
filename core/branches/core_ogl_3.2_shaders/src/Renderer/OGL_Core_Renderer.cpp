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
	#include <OpenGL/OpenGL.h>
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
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );
	
	cout << m_viewport[0] << 'x' << m_viewport[1] << endl;
	mDisplayModes.push_back(DisplayMode(640, 480));
	mDisplayModes.push_back(DisplayMode(800, 600));
	mDisplayModes.push_back(DisplayMode(1024, 768));
	mDisplayModes.push_back(DisplayMode(1152, 864));
	mDisplayModes.push_back(DisplayMode(1280, 1024));
	mDisplayModes.push_back(DisplayMode(1440, 900));
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
	else if(extensionExists("GL_ARB_texture_rectangle"))
	{
		cout << "\t\t- GL_ARB_texture_rectangle" << endl;
		mTextureTarget = GL_TEXTURE_RECTANGLE_ARB;
		return true;
	}
	else
		return false;
}


void OGL_Core_Renderer::initGL()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	//glViewport(0, 0, mScreen->w, mScreen->h);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Spit out system graphics information.
	cout << "\t- OpenGL System Info -" << endl;

	driverName((char*)glGetString(GL_RENDERER));

	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << driverName() << endl;
	cout << "\tDriver Version: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	if(!checkExtensions())
		throw Exception(0, "Graphics Requirements Not Met", "Your graphics driver does not meet the minimum requirements.");
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

	//if(TTF_Init() < 0)
	//	throw Exception(703, "Error starting TrueType Library", TTF_GetError());

	buildDisplayModeList();
	mShaderManager = new ShaderManager();
	initGL();
}
