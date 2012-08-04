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
#include "NAS2D/Renderer/OGL_Renderer.h"

#if defined(__APPLE__)
	#include <OpenGL/OpenGL.h>
#elif defined(WIN32)
	#include "GLee.h"
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
 * Texture coordinate pairs. Default coordinates encompassing the entire texture.
 */
 GLfloat DEFAULT_TEXTURE_COORDS[8] =	{ 0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f };

GLfloat POINT_VERTEX_ARRAY[2] = { 0 };


GraphicsQuality TEXTURE_FILTER = GRAPHICS_GOOD;


OGL_Renderer::OGL_Renderer():	Renderer("OpenGL Renderer"),
								mScreen(0),
								mTextureTarget(0)
{
	cout << "Starting " << mRendererName << ":" << endl;
	
	Configuration& cf = Utility<Configuration>::get();

	TEXTURE_FILTER = cf.graphicsTextureQuality();

	initVideo(cf.graphicsWidth(), cf.graphicsHeight(), cf.graphicsColorDepth(), cf.fullscreen(), cf.vsync());

	// Set our LetterBox height to 15% of the screen's height.
	mLetterBoxHeight = (int)((mScreen->h) * 0.15);
	
}


OGL_Renderer::~OGL_Renderer()
{
	//glDeleteBuffers(1, &mVertexBufferObject);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	//delete mShaderManager;
}


void OGL_Renderer::drawVertexArray(GLuint textureId, bool defaultTextureCoords = true)
{ 	
	glBindTexture(mTextureTarget, textureId);

	glVertexPointer(2, GL_FLOAT, 0, mVertexArray);

	// Choose from the default texture coordinates or from a custom set.
	if(defaultTextureCoords)
		glTexCoordPointer(2, GL_FLOAT, 0, DEFAULT_TEXTURE_COORDS);
	else
		glTexCoordPointer(2, GL_FLOAT, 0, mTextureCoordArray);
	
	glDrawArrays(GL_QUADS, 0, 4);
}


/**
 * Used internally to fill the vertex array with quad vertex information.
 */
void OGL_Renderer::fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	mVertexArray[0] = static_cast<GLfloat>(x),		mVertexArray[1] = static_cast<GLfloat>(y);
	mVertexArray[2] = static_cast<GLfloat>(x),		mVertexArray[3] = static_cast<GLfloat>(y + h);
	mVertexArray[4] = static_cast<GLfloat>(x + w),	mVertexArray[5] = static_cast<GLfloat>(y + h);
	mVertexArray[6] = static_cast<GLfloat>(x + w),	mVertexArray[7] = static_cast<GLfloat>(y);
}

/**
 * Convenience interface function to aid in code readability by
 * performing inline type casting.
 */
inline void OGL_Renderer::fillVertexArray(int x, int y, int w, int h)
{
	fillVertexArray(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(w), static_cast<GLfloat>(h));
}



/**
 * Used internally to fill the texture coordinate array with quad vertex information.
 */
void OGL_Renderer::fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v)
{
	mTextureCoordArray[0] = static_cast<GLfloat>(x),	mTextureCoordArray[1] = static_cast<GLfloat>(y);
	mTextureCoordArray[2] = static_cast<GLfloat>(x),	mTextureCoordArray[3] = static_cast<GLfloat>(v);
	mTextureCoordArray[4] = static_cast<GLfloat>(u),	mTextureCoordArray[5] = static_cast<GLfloat>(v);
	mTextureCoordArray[6] = static_cast<GLfloat>(u),	mTextureCoordArray[7] = static_cast<GLfloat>(y);
}


/**
 * Convenience interface function to aid in code readability by
 * performing inline type casting.
 */
inline void OGL_Renderer::fillTextureArray(int x, int y, int u, int v)
{
	fillTextureArray(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(u), static_cast<GLfloat>(v));
}


void OGL_Renderer::drawImage(Image& image, int x, int y, float scale = 1.0f)
{
	fillVertexArray(x, y, image.width(), image.height());
	drawVertexArray(image.texture_id());
}


void OGL_Renderer::drawSubImage(Image& image, int rasterX, int rasterY, int x, int y, int width, int height)
{
	fillVertexArray(rasterX, rasterY, width, height);

	fillTextureArray(	static_cast<GLfloat>(x) / static_cast<GLfloat>(image.width()),
						static_cast<GLfloat>(y) / static_cast<GLfloat>(image.height()),
						static_cast<GLfloat>(x) / static_cast<GLfloat>(image.width()) + static_cast<GLfloat>(width) / static_cast<GLfloat>(image.width()),
						static_cast<GLfloat>(y) / static_cast<GLfloat>(image.height()) + static_cast<GLfloat>(height) / static_cast<GLfloat>(image.height())
					);

	drawVertexArray(image.texture_id(), false);
}


void OGL_Renderer::drawSubImageRotated(Image& image, int rasterX, int rasterY, int x, int y, int width, int height, float degrees)
{
	glPushMatrix();

	// Find center point of the image.
	float tX = static_cast<float>(width / 2);
	float tY = static_cast<float>(height / 2);

	// Adjust the translation so that images appear where expected.
	glTranslatef(static_cast<float>(rasterX + tX), static_cast<float>(rasterY + tY), 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	fillVertexArray(-tX, -tY, tX * 2, tY * 2);

	fillTextureArray(	static_cast<GLfloat>(x) / static_cast<GLfloat>(image.width()),
						static_cast<GLfloat>(y) / static_cast<GLfloat>(image.height()),
						static_cast<GLfloat>(x) / static_cast<GLfloat>(image.width()) + static_cast<GLfloat>(width) / static_cast<GLfloat>(image.width()),
						static_cast<GLfloat>(y) / static_cast<GLfloat>(image.height()) + static_cast<GLfloat>(height) / static_cast<GLfloat>(image.height())
					);

	drawVertexArray(image.texture_id(), false);

	glPopMatrix();
}


void OGL_Renderer::drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale)
{
	glPushMatrix();

	// Find center point of the image.
	int imgHalfW = (image.width() / 2);
	int imgHalfH = (image.height() / 2);

	float tX = imgHalfW * scale;
	float tY = imgHalfH * scale;

	// Adjust the translation so that images appear where expected.
	glTranslatef(static_cast<float>(x + imgHalfW), static_cast<float>(y + imgHalfH), 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	fillVertexArray(-tX, -tY, tX * 2, tY * 2);
	
	drawVertexArray(image.texture_id());
	glPopMatrix();

	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	fillVertexArray(x, y, w, h);
	
	drawVertexArray(image.texture_id());
}


void OGL_Renderer::drawImageRepeated(Image& image, int x, int y, int w, int h)
{
	fillVertexArray(x, y, w, h);

	// Change texture mode to repeat at edges.
	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	drawVertexArray(image.texture_id(), true);

	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(mTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void OGL_Renderer::drawImageToImage(Image& source, Image& destination, const Point_2d& dstPoint)
{
	// Ignore the call if the detination point is outside the bounds of destination image.
	if(dstPoint.x > destination.width() || dstPoint.y > destination.height())
		return;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Bind our destination texture.
	glBindTexture(mTextureTarget, destination.texture_id());

	// Check for the need to clip the source texture.
	Rectangle_2d clipRect;

	(dstPoint.x + source.width()) > destination.width() ? clipRect.w = source.width() - ((dstPoint.x + source.width()) - destination.width()) : clipRect.w = source.width();
	(dstPoint.y + source.height()) > destination.height() ? clipRect.h = source.height() - ((dstPoint.y + source.height()) - destination.height()) : clipRect.h = source.height();

	// Ignore this call if the clipping rect is smaller than 1 pixel in any dimension.
	if(clipRect.w < 1 || clipRect.h < 1)
		return;


	unsigned int fbo = destination.fbo_id();

	// Bind the framebuffer object 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	// Attach a texture to the FBO 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, mTextureTarget, destination.texture_id(), 0);

	// Flip the Y axis to keep images drawing correctly.
	fillVertexArray(dstPoint.x, destination.height() - dstPoint.y, clipRect.w, -clipRect.h);

	drawVertexArray(source.texture_id());

	// Bind our destination texture again
	glBindTexture(mTextureTarget, destination.texture_id());

	// Reset viewport and unbind
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}


void OGL_Renderer::drawPixel(int x, int y, int r, int g, int b, int a)
{
	glDisable(mTextureTarget);

	glColor4ub(r, g, b, a);

	POINT_VERTEX_ARRAY[0] = static_cast<float>(x) + 0.5f; POINT_VERTEX_ARRAY[1] = static_cast<float>(y) + 0.5f;

	//glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, POINT_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, 1);
	//glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(mTextureTarget);

	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a, int line_width = 1)
{	
	glDisable(mTextureTarget);
	
	GLfloat verts[12] =	{
							static_cast<float>(x), static_cast<float>(y) + 1.0f,
							static_cast<float>(x2), static_cast<float>(y2) + 1.0f,
							static_cast<float>(x) - 0.5f, static_cast<float>(y) + 0.5f,
							static_cast<float>(x2) - 0.5f, static_cast<float>(y2) + 0.5f,
							static_cast<float>(x), static_cast<float>(y),
							static_cast<float>(x2), static_cast<float>(y2)
						};

	glColor4ub(r, g, b, a);

	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	
	glEnable(mTextureTarget);

	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


/*
 * The below code originally comes from http://slabode.exofire.net/circle_draw.shtml.
 *
 * Modified to support X/Y scaling to draw an ellipse.
 */
void OGL_Renderer::drawCircle(int cx, int cy, int radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y)
{
	glDisable(mTextureTarget);

	glColor4ub(r, g, b, a);

	float theta = PI_2 / static_cast<float>(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = static_cast<float>(radius);
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
				We may want to consider forcing circles to be within a certain range
				of num_segments and provide arrays for them with the correct number
				of elements. Will increase code length but will eliminate the memory
				fudging.
	 */
	delete [] verts;
	verts = 0;

	glEnable(mTextureTarget);
	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::drawBox(int x, int y, int width, int height, int r, int g, int b, int a)
{	
	glColor4ub(r, g, b, a);
	glDisable(mTextureTarget);

	fillVertexArray(x, y, width, height);
	glVertexPointer(2, GL_FLOAT, 0, mVertexArray);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glEnable(mTextureTarget);
	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);
	glDisable(mTextureTarget);

	fillVertexArray(x, y, width, height);
	drawVertexArray(0);

	glEnable(mTextureTarget);
	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::drawText(Font& font, const std::string& text, int x, int y, int r, int g, int b, int a)
{
	// Protect against a NULL font object being passed in.
	if(!font.loaded())
		return;
	else if(text.empty())
		return;

//	SDL_Color Color = {r, g, b};
//	SDL_Surface *textSurface = TTF_RenderText_Blended(font.font(), text.c_str(), Color);
//	if(!textSurface)
//	{
//		stringstream str;
//		str << "Renderer: Unable to render Font '" << font.name() << "': " << TTF_GetError() << "." << endl;
//		pushMessage(str.str());
//		return;
//	}
//
//
//	// Detect which order the pixel data is in to properly feed OGL.
//	GLint nColors = textSurface->format->BytesPerPixel;
//	
//	GLenum textureFormat = 0;
//	if(nColors == 4)
//	{
//		if(textSurface->format->Rmask == 0x000000ff)
//			textureFormat = GL_RGBA;
//		else
//			textureFormat = GL_BGRA;
//	}
//	else if(nColors == 3)     // no alpha channel
//	{
//		if(textSurface->format->Rmask == 0x000000ff)
//			textureFormat = GL_RGB;
//		else
//			textureFormat = GL_BGR;
//	}
//	else
//	{
//		cout << "Image must be 16-, 24- or 32-bit." << std::endl;
//		return;
//	}
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
//											// or can we set it once in the Renderer?
//
//	// Create a texture from the text surface, render it, and free it.
//	GLuint texId = 0;
//	glGenTextures(1, &texId);
//	glBindTexture(GL_TEXTURE_2D, texId);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, nColors, textSurface->w, textSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, textSurface->pixels);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


//	// =================
//	glColor4ub(r, g, b, a);
//	
//	fillVertexArray(x, y, textSurface->w, textSurface->h);
//
//	drawVertexArray(texId);
//
//	glDeleteTextures(1, &texId);
//	SDL_FreeSurface(textSurface);
//
//	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
	
	// =================
	glColor4ub(r, g, b, a);
	int advX = x;
	for(string::size_type i = 0; i < text.size(); ++i)
    {
        int ch = int(text[i]);
		fillVertexArray(advX, y, font.getGlyphWidth(ch), font.height());
	
		drawVertexArray(font.texture(ch));
		advX += font.getGlyphWidth(ch);
	}
	
	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
	
}


void OGL_Renderer::drawTextClamped(Font& font, const std::string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a)
{
	// Protect against a NULL font object being passed in.
	if(!font.loaded())
		return;
	else if(text.empty())
		return;

//	SDL_Color Color = {r, g, b};
//	SDL_Surface *textSurface = TTF_RenderText_Blended(font.font(), text.c_str(), Color);
//	if(!textSurface)
//	{
//		cout << "(ERR) Renderer Error: Unable to render Font '" << font.name() << "': " << TTF_GetError() << "." << endl;
//		//glDisable(mTextureTarget);
//		return;
//	}
//
//	// Create a new surface at the clamped size, blit to it and free
//	// the originally generated text surface.
//	SDL_Surface *clampedSurface = SDL_CreateRGBSurface(textSurface->flags, w, h, textSurface->format->BitsPerPixel, textSurface->format->Rmask, textSurface->format->Gmask, textSurface->format->Bmask, textSurface->format->Amask);
//	SDL_SetAlpha(textSurface, 0, textSurface->format->alpha); 
//	SDL_Rect grabRect = {x, y, w, h};
//	SDL_BlitSurface(textSurface, &grabRect, clampedSurface, NULL);
//	SDL_SetAlpha(clampedSurface, SDL_SRCALPHA, clampedSurface->format->alpha);
//	SDL_FreeSurface(textSurface);
//	textSurface = NULL;
//
//
//	// Detect which order the pixel data is in to properly feed OGL.
//	GLint nColors = textSurface->format->BytesPerPixel;
//	
//	GLenum textureFormat = 0;
//	if(nColors == 4)
//	{
//		if(textSurface->format->Rmask == 0x000000ff)
//			textureFormat = GL_RGBA;
//		else
//			textureFormat = GL_BGRA;
//	}
//	else if(nColors == 3)     // no alpha channel
//	{
//		if(textSurface->format->Rmask == 0x000000ff)
//			textureFormat = GL_RGB;
//		else
//			textureFormat = GL_BGR;
//	}
//	else
//	{
//		cout << "Image must be 16-, 24- or 32-bit." << std::endl;
//		return;
//	}
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
//											// or can we set it once in the Renderer?
//
//	// Create a texture from the text surface, render it, and free it.
//	GLuint texId = 0;
//	glGenTextures(1, &texId);
//	glBindTexture(GL_TEXTURE_2D, texId);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, nColors, textSurface->w, textSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, textSurface->pixels);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//
//	glColor4ub(r, g, b, a);
//
//	fillVertexArray(rasterX, rasterY, clampedSurface->w, clampedSurface->h);
//	
//	drawVertexArray(texId);
//	
//	glDeleteTextures(1, &texId);
//	SDL_FreeSurface(clampedSurface);
//
//	glColor4ub(255, 255, 255, 255); // Reset color back to normal.
}


void OGL_Renderer::clearScreen(int r, int g, int b)
{
	glClearColor((GLfloat)r / 255, (GLfloat)g / 255, (GLfloat)b / 255, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT);
}


void OGL_Renderer::update()
{
	getError();

	Renderer::update();
	SDL_GL_SwapBuffers();
}


int OGL_Renderer::width()
{
    return mScreen->w;
}


int OGL_Renderer::height()
{
    return mScreen->h;
}


void OGL_Renderer::buildDisplayModeList()
{
/*
	// Get available fullscreen/hardware modes
	SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_OPENGL);

	// No video modes available.
	if(modes == NULL)
	{
		Logger::log << Logger::error() << "No available video modes." << endl;
		throw Exception(705, "Cannot Initialize Video", "No video modes are available.");
	}
	// Indicates that ANY resolution is okay.
	else if(modes == (SDL_Rect **)-1)
	{
		// This is an extremely unlikely case, in fact so unlikely
		// that I can comfortably say that we'll never hit this so
		// we can safely throw an exception in the event it does occur.
		throw Exception(706, "Cannot Initialize Video", "No video modes are available.");
	}
	for(int i = 0; modes[i]; ++i)
	{
		DisplayMode tmpMode;
		tmpMode.screenWidth = modes[i]->w;
		tmpMode.screenHeight = modes[i]->h;
		
		stringstream str;
		str << tmpMode.screenWidth << "x" << tmpMode.screenHeight;
		tmpMode.resolution = str.str();
		
		mDisplayModes.push_back(tmpMode);
	}
	*/

	mDisplayModes.push_back(DisplayMode(640, 480));
	mDisplayModes.push_back(DisplayMode(800, 600));
	mDisplayModes.push_back(DisplayMode(1024, 768));
	mDisplayModes.push_back(DisplayMode(1152, 864));
	mDisplayModes.push_back(DisplayMode(1280, 1024));
	mDisplayModes.push_back(DisplayMode(1440, 900));
}


void OGL_Renderer::getError()
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
		case GL_INVALID_OPERATION:
			errStr += "Invalid Operation.";
			break;
		case GL_STACK_OVERFLOW:
			errStr += "Stack Overflow.";
			break;
		case GL_STACK_UNDERFLOW:
			errStr += "Stack Underflow.";
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
bool OGL_Renderer::extensionExists(const std::string& extension)
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


bool OGL_Renderer::checkExtensions()
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


void OGL_Renderer::initGL()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	glViewport(0, 0, mScreen->w, mScreen->h);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)mScreen->w, (GLdouble)mScreen->h, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	// Spit out system graphics information.
	const SDL_VideoInfo* mVideoInfo = SDL_GetVideoInfo();
	cout << "\t- OpenGL System Info -" << endl;

	mDriverName = (char*)glGetString(GL_RENDERER);

	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << mDriverName << endl;
	cout << "\tDriver Version: " << glGetString(GL_VERSION) << endl;

	if(!checkExtensions())
		throw Exception(0, "Graphics Requirements Not Met", "Your graphics driver does not meet the minimum requirements.");

	glEnable(mTextureTarget);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//		mShaderManager->loadShader("shaders/font/font.frag", mFontShaderFrag);
	//	mShaderManager->loadShader("shaders/font/font.vert", mFontShaderVert);
}


void OGL_Renderer::initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync)
{
	SDL_putenv("SDL_VIDEO_CENTERED=center");

	// Set the app title to blank until the client sets it.
	//setApplicationTitle(" ");

	// Initialize SDL's Video Subsystems.
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(701, "Error starting SDL Video Library", SDL_GetError());

	Uint32 sdlFlags = SDL_OPENGL;

	if(fullscreen)
		sdlFlags = sdlFlags|SDL_FULLSCREEN;

#ifdef __APPLE__
	
	if (vsync)
	{
		vsync = false;
		//const GLint VBL = 1;
		//CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &VBL);
	}
#endif
	if(vsync)
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	mScreen = 0;
	mScreen = SDL_SetVideoMode(resX, resY, bpp, sdlFlags);
	if(mScreen == 0)
		throw Exception(702, "Error setting Video Mode", SDL_GetError());

	SDL_ShowCursor(false);

	//if(TTF_Init() < 0)
	//	throw Exception(703, "Error starting TrueType Library", TTF_GetError());

	buildDisplayModeList();
	//mShaderManager = new ShaderManager();
	initGL();
}
