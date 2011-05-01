// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Trig.h"
#include "NAS2D/Renderer/OGL_Renderer.h"

#if defined(__APPLE__)
	#include "SDL_gfx/SDL_gfxPrimitives.h"
	#include "SDL_ttf/SDL_ttf.h"
	#include <OpenGL/OpenGL.h>
	#include <GLUT/glut.h>
#elif defined(WIN32)
	#include "GLee.h"
	#include "SDL/SDL_opengl.h"
	#include "SDL/SDL_gfxPrimitives.h"
#else
	#include "SDL/SDL_opengl.h"
#endif

#include <iostream>

using namespace std;

// Life of generated OpenGL Textures in Miliseconds
const unsigned int TEXTURE_EXPIRE_TIME = 30000;	// Roughly 30 seconds

GraphicsQuality TEXTURE_FILTER = GRAPHICS_GOOD;

OGL_Renderer::OGL_Renderer():	Renderer("OGL Renderer"),
								mScreen(0),
								mLastResourceCheck(0),
								//mTextureTarget(0),
								//mVertexBufferObject(0),
								mRequirementsMet(false)
{
	cout << "Starting " << mRendererName << "..." << endl;
	
	Configuration& cf = Singleton<Configuration>::get();

	TEXTURE_FILTER = cf.getGraphicsTextureQuality();

	initVideo(cf.getGraphicsWidth(), cf.getGraphicsHeight(), cf.getGraphicsColorDepth(), cf.isGraphicsFullscreen(), cf.isVsyncEnabled());

	// Set our LetterBox height to 15% of the screen's height.
	mLetterBoxHeight = (int)((mScreen->h) * 0.15);
	
}


OGL_Renderer::OGL_Renderer(unsigned int ResX, unsigned int ResY, unsigned int BPP, bool fullscreen, bool vsync):	Renderer("OGL Renderer"),
																							mLastResourceCheck(0),
																							//mTextureTarget(0),
																							//mVertexBufferObject(0),
																							mRequirementsMet(false)
{
	cout << "Starting " << mRendererName << "..." << endl;

	initVideo(ResX, ResY, BPP, fullscreen, vsync);

	// Set our LetterBox height to 15% of the screen's height.
	mLetterBoxHeight = (int)((mScreen->h) * 0.15);
	
	mVertexBufferObject = 0;
	
	glGenBuffers(1, &mVertexBufferObject);
	
}


OGL_Renderer::~OGL_Renderer()
{
	#if defined(_DEBUG)
	cout << "(DGB) OpenGL Renderer contained " << mTextureArray.size() << " textures during deconstruction." << endl;
	#endif

	// Clear up GL generated textures.
	if(!mTextureArray.empty())
	{
		glDeleteTextures(mTextureArray.size(), &mTextureArray.begin()->second.first);
		mTextureArray.clear();
	}
	
	glDeleteBuffers(1, &mVertexBufferObject);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


void OGL_Renderer::setApplicationTitle(const std::string& title)
{
	SDL_WM_SetCaption(title.c_str(), title.c_str());
}


bool OGL_Renderer::drawVertexArray(Image& image, GLfloat vertexArray[], GLfloat textureCoord[], float scale = 1.0f, bool repeat = false)
{
	GLfloat vertices[8];
	memcpy(vertices, vertexArray, sizeof(vertices));
	
	GLfloat texture[8];
	memcpy(texture, textureCoord, sizeof(texture));
	
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(texture), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texture), texture);
	
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(mTextureTarget);
	glBindTexture(mTextureTarget, getTextureId(image));
	
	if(repeat)
	{
		// the texture wraps over at the edges (repeat)
		glTexParameterf( mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT );
	}
	
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));
	
	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisable(mTextureTarget);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return true;
}


bool OGL_Renderer::drawVertexArrayText(GLuint texture, GLfloat vertexArray[], GLfloat textureCoord[], float scale = 1.0f)
{
	GLfloat vertices[8];
	memcpy(vertices, vertexArray, sizeof(vertices));
	
	GLfloat textureArray[8];
	memcpy(textureArray, textureCoord, sizeof(textureArray));
	
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureArray), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textureArray), textureArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(mTextureTarget);
	glBindTexture(mTextureTarget, texture);
	
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));
	
	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisable(mTextureTarget);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return true;
}


void OGL_Renderer::drawImage(Image& image, int x, int y, float scale = 1.0f)
{
	//std::vector<std::pair<int, int>> *vertex;
	GLfloat vertices[8] = {
		static_cast<GLfloat>(x), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + image.getWidth()), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + image.getWidth()), static_cast<GLfloat>(y + image.getHeight()),
		static_cast<GLfloat>(x), static_cast<GLfloat>(y + image.getHeight())
	};
	
	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	glColor4ub(255, 255, 255, 255);
	
	drawVertexArray(image, vertices, texture, scale);
}


/**
 * \todo	Currently this function doesn't actually render a sub image of a larger image. Instead,
 *			it just stretches the texture across the entire surface of the quad.
 */
void OGL_Renderer::drawSubImage(Image& image, int rasterX, int rasterY, int x, int y, int width, int height)
{
	GLfloat vertices[8] = {
		static_cast<GLfloat>(rasterX), static_cast<GLfloat>(rasterY),
		static_cast<GLfloat>(rasterX + width), static_cast<GLfloat>(rasterY),
		static_cast<GLfloat>(rasterX + width), static_cast<GLfloat>(rasterY + height),
		static_cast<GLfloat>(rasterX), static_cast<GLfloat>(rasterY + height)
	};

	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
    
    // Because OGL is complicated we took care of subImages in the Image class.
    Image *subImage = new Image(&image, x, y, width, height);
	
	glColor4ub(255, 255, 255, 255);
	
	drawVertexArray(*subImage, vertices, texture);
}


void OGL_Renderer::drawImageRotated(Image& image, int x, int y, float degrees, int r, int g, int b, int a, float scale)
{
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glPushMatrix();

	int imgHalfW = (image.getWidth() >> 1);
	int imgHalfH = (image.getHeight() >> 1);

	float tX = imgHalfW * scale;
	float tY = imgHalfH * scale;

	// Adjust the translation so that images appear where expected.
	glTranslatef((GLfloat)(x + imgHalfW), (GLfloat)(y + imgHalfH), 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLfloat vertices[8] = {
		-tX, -tY,
		tX, -tY,
		tX, tY,
		-tX, tY
	};
	
	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	drawVertexArray(image, vertices, texture, scale);
	glPopMatrix();
}


void OGL_Renderer::drawImageStretched(Image& image, int x, int y, int w, int h, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLfloat vertices[8] = {
		static_cast<GLfloat>(x), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + w), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + w), static_cast<GLfloat>(y + h),
		static_cast<GLfloat>(x), static_cast<GLfloat>(y + h)
	};
	
	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	drawVertexArray(image, vertices, texture);
}


void OGL_Renderer::drawImageRepeated(Image& image, int x, int y, int w, int h)
{
	glColor4ub(255, 255, 255, 255);
	GLfloat vertices[8] = {
		static_cast<GLfloat>(x), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + w), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + w), static_cast<GLfloat>(y + h),
		static_cast<GLfloat>(x), static_cast<GLfloat>(y + h)
	};
		
	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
		
	drawVertexArray(image, vertices, texture, 1.0f, true);
}


void OGL_Renderer::drawImageToImage(Image& source, const Rectangle_2d& srcRect, Image& destination, const Point_2d& dstPoint)
{
	// Ignore the call if the detination point is outside the bounds of destination image.
	if(dstPoint.x > destination.getWidth() || dstPoint.y > destination.getHeight())
		return;

	Image subImage(&source, srcRect.x, srcRect.y, srcRect.w, srcRect.h);
	
	glEnable(mTextureTarget);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	// Bind our destination texture.
	glBindTexture(mTextureTarget, getTextureId(destination));
	
	// Check for the need to clip the source texture.
	Rectangle_2d clipRect;
	
	(dstPoint.x + srcRect.w) > destination.getWidth() ? clipRect.w = srcRect.w - ((dstPoint.x + srcRect.w) - destination.getWidth()) : clipRect.w = srcRect.w;
	(dstPoint.y + srcRect.h) > destination.getHeight() ? clipRect.h = srcRect.h - ((dstPoint.y + srcRect.h) - destination.getHeight()) : clipRect.h = srcRect.h;

	// Ignore this call of the clipping rect is smaller than 1 pixel in any dimension.
	if(clipRect.w < 1 || clipRect.h < 1)
		return;
	
	// Create a framebuffer object
	GLuint myFBO;
	glGenFramebuffersEXT(1, &myFBO);
	
	// Bind the framebuffer object 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, myFBO);
	
	// Attach a texture to the FBO 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, mTextureTarget, getTextureId(destination), 0);
	
	// Position to draw our quad in the FBO
	// The Y-Position was getting reversed, to solve this, we take the texture
	// height (which puts us at the top of the screen) and subtract our y-position.
	glColor4ub(255, 255, 255, 255);
	GLfloat vertices[8] = {
		static_cast<GLfloat>(dstPoint.x), static_cast<GLfloat>((destination.getHeight() - dstPoint.y)),
		static_cast<GLfloat>(dstPoint.x), static_cast<GLfloat>((destination.getHeight() - dstPoint.y) - clipRect.h),
		static_cast<GLfloat>(dstPoint.x + clipRect.w), static_cast<GLfloat>((destination.getHeight() - dstPoint.y) - clipRect.h),
		static_cast<GLfloat>(dstPoint.x + clipRect.w), static_cast<GLfloat>((destination.getHeight() - dstPoint.y))
	};
	
	GLfloat texture[8] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};
	
	// Render our quad
	drawVertexArray(subImage, vertices, texture);
	
	// Bind our destination texture again
	glBindTexture(mTextureTarget, getTextureId(destination));
	
	// Reset viewport and unbind
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffersEXT(1, &myFBO);
}


void OGL_Renderer::imageDesaturate(Image& image)
{
	#if defined(_DEBUG)
		if(!image.isLoaded())
		{
			stringstream str;
			str << "Image '" << image.getName() << "' is not loaded and cannot be desaturated. " << image.getErrorMessage();
			pushMessage(str.str());
			return;
		}
	#endif

	SDL_Surface *surface = image.getPixels();
	for(int y = 0; y < image.getHeight(); y++)
	{
		for(int x = 0; x < image.getWidth(); x++)
		{
			Uint8 r, g, b, a;

			SDL_GetRGBA(getPixelColor(image, x, y), surface->format, &r, &g, &b, &a);

			Uint8 grey = (r + g + b) / 3;

			pixelRGBA(surface, x, y, grey, grey, grey, a);
		}
	}
}


void OGL_Renderer::drawPixel(int x, int y, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);

	glBegin(GL_POINTS);
		glVertex2f(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
	glEnd();
}


void OGL_Renderer::drawLine(int x, int y, int x2, int y2, int r, int g, int b, int a, int line_width = 1)
{	
	glColor4ub(r, g, b, a);

	glBegin(GL_LINES);
		glVertex2f(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
		glVertex2f(static_cast<float>(x2) + 0.5f, static_cast<float>(y2) + 0.5f);
	glEnd();
}


/*
 * The below code originally comes from http://slabode.exofire.net/circle_draw.shtml.
 *
 * Modified to support X/Y scaling to draw an ellipse.
 */
void OGL_Renderer::drawCircle(int cx, int cy, int radius, int r, int g, int b, int a, int num_segments, float scale_x, float scale_y)
{
	glColor4ub(r, g, b, a);

	float theta = PI_2 / static_cast<float>(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = static_cast<float>(radius);
	float y = 0;

	int segments = num_segments;
	if(segments < 4)	
		segments = static_cast<int>(10.0f * sqrtf(x)); // Note, I used X because it's already the Radius cast to a float.

	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < segments; ii++)
	{
		glVertex2f(x * scale_x + cx, y * scale_y + cy);

		// Apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}


void OGL_Renderer::drawBox(int x, int y, int width, int height, int r, int g, int b, int a)
{	
	glColor4ub(r, g, b, a);

	glBegin(GL_LINE_LOOP);
		glVertex2i(x /*+ 0.5f*/, y);
		glVertex2i((x + width), y);
		glVertex2i((x + width), (y + height));
		glVertex2i(x, (y + height));
	glEnd();
}


void OGL_Renderer::drawBoxFilled(int x, int y, int width, int height, int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);

	glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i((x + width), y);
		glVertex2i((x + width), (y + height));
		glVertex2i(x, (y + height));
	glEnd();
}


void OGL_Renderer::drawText(Font& font, const string& text, int x, int y, int r, int g, int b, int a)
{
	// Protect against a NULL font object being passed in.
	if(!font.isLoaded())
		return;
	else if(text.empty())
		return;

	SDL_Color Color = {r, g, b};
	SDL_Surface *textSurface = TTF_RenderText_Blended(font.getFont(), text.c_str(), Color);
	if(!textSurface)
	{
		stringstream str;
		str << "Renderer: Unable to render Font '" << font.getName() << "': " << TTF_GetError() << "." << endl;
		pushMessage(str.str());
		return;
	}

	// Create a texture from the text surface, render it, and free it.
	GLuint texId = generateTexture(textSurface);

	glColor4ub(255, 255, 255, a);
	GLfloat vertices[8] = {
		static_cast<GLfloat>(x), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + textSurface->w), static_cast<GLfloat>(y),
		static_cast<GLfloat>(x + textSurface->w), static_cast<GLfloat>(y + textSurface->h),
		static_cast<GLfloat>(x), static_cast<GLfloat>(y + textSurface->h)
	};
	
	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	if(drawVertexArrayText(texId, vertices, texture))
	{
		glDeleteTextures(1, &texId);
	}

	SDL_FreeSurface(textSurface);

	return;
}


void OGL_Renderer::drawTextClamped(Font& font, const string& text, int rasterX, int rasterY, int x, int y, int w, int h, int r, int g, int b, int a)
{
	// Protect against a NULL font object being passed in.
	if(!font.isLoaded())
		return;
	else if(text.empty())
		return;

	SDL_Color Color = {r, g, b};
	SDL_Surface *textSurface = TTF_RenderText_Blended(font.getFont(), text.c_str(), Color);
	if(!textSurface)
	{
		cout << "(ERR) Renderer Error: Unable to render Font '" << font.getName() << "': " << TTF_GetError() << "." << endl;
		glDisable(mTextureTarget);
		return;
	}

	// Create a new surface at the clamped size, blit to it and free
	// the originally generated text surface.
	SDL_Surface *clampedSurface = SDL_CreateRGBSurface(textSurface->flags, w, h, textSurface->format->BitsPerPixel, textSurface->format->Rmask, textSurface->format->Gmask, textSurface->format->Bmask, textSurface->format->Amask);
	SDL_SetAlpha(textSurface, 0, textSurface->format->alpha); 
	SDL_Rect grabRect = {x, y, w, h};
	SDL_BlitSurface(textSurface, &grabRect, clampedSurface, NULL);
	SDL_SetAlpha(clampedSurface, SDL_SRCALPHA, clampedSurface->format->alpha);
	SDL_FreeSurface(textSurface);
	textSurface = NULL;


	// Create a texture from the clamped surface, render it, and free it.
	GLuint texId = generateTexture(clampedSurface);

	glColor4ub(255, 255, 255, a);
	GLfloat vertices[8] = {
		static_cast<GLfloat>(rasterX), static_cast<GLfloat>(rasterY),
		static_cast<GLfloat>(rasterX + clampedSurface->w), static_cast<GLfloat>(rasterY),
		static_cast<GLfloat>(rasterX + clampedSurface->w), static_cast<GLfloat>(rasterY + clampedSurface->h),
		static_cast<GLfloat>(rasterX), static_cast<GLfloat>(rasterY + clampedSurface->h)
	};
	
	GLfloat texture[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	if(drawVertexArrayText(texId, vertices, texture))
	{
		glDeleteTextures(1, &texId);
	}

	SDL_FreeSurface(clampedSurface);
	return;
}


void OGL_Renderer::clearScreen(int r, int g, int b)
{
	glClearColor((GLclampf)r / 255, (GLclampf)g / 255, (GLclampf)b / 255, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT);
}


void OGL_Renderer::update()
{
	getError();

	updateTextures();

	Renderer::update();
	SDL_GL_SwapBuffers();
}


unsigned int OGL_Renderer::getPixelColor(Image& src, int x, int y)
{
	SDL_LockSurface(src.getPixels());
    int bpp = src.getPixels()->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8*)src.getPixels()->pixels + y * src.getPixels()->pitch + x * bpp;

	SDL_UnlockSurface(src.getPixels());

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
			return *(Uint32*)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}


bool OGL_Renderer::isPixelTransparent(Image& src, int x, int y)
{
	SDL_Surface *surface = src.getPixels();

	Uint8 r, g, b, a;

	SDL_GetRGBA(getPixelColor(src, x, y), surface->format, &r, &g, &b, &a);
	
	return (a == 0);
}


int OGL_Renderer::getWidth()
{
    return mScreen->w;
}


int OGL_Renderer::getHeight()
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
	pushMessage(str.str());
}


/**
 * Gets a TextureID from our array of GL Texture's.
 */
inline GLuint OGL_Renderer::getTextureId(Image& image)
{
	GLuint textureId = 0;

	// Here we use the address of an image as its identifier. The address should never
	// change during the lifetime of the image (at least that's the assumption).
	TextureArray::iterator it = mTextureArray.find(image.id());

	if(it == mTextureArray.end())
	{
		textureId = generateTexture(image.getPixels());
		mTextureArray[image.id()] = std::pair<GLuint, int>(textureId, mTimer.ms());
	}
	else
	{
		textureId = it->second.first;
		it->second.second = mTimer.ms();	// Set time stamp.
	}

	return textureId;
}


/**
 * Gets the Determines the TextureFormat from an Image.
 */
GLenum OGL_Renderer::getTextureFormat(SDL_Surface *src)
{
	// Detect which order the pixel data is in to properly feed OGL.
	GLint nColors = src->format->BytesPerPixel;
	
	/**
	 * \todo	When compiling in nightmare mode with Visual Studio, it indicated that \c textureFormat could be potentially
	 *			uninitialized should nColors be anything other than '3' or '4' (24 bit and 32 bit color modes, respectively).
	 *			The only other expected values would be '2' (16 bit color mode) or '1' (8 bit color mode). While in practice
	 *			neither of these cases is likely to show up, it's extremely important that textureFormat is initialized to a
	 *			good default value that will work for generally all modes or one that will at least not cause a crash.
	 */
	GLenum textureFormat;
	if(nColors == 4)
	{
		if(src->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
	}
	else if(nColors == 3)     // no alpha channel
	{
		if(src->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
	}
	else
		cout << "Image is not truecolor." << std::endl;
	
	return textureFormat;
}


unsigned int OGL_Renderer::generateTexture(SDL_Surface *src)
{
	GLuint texId = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glGenTextures(1, &texId);
	glBindTexture(mTextureTarget, texId);

	// Detect which order the pixel data is in to properly feed OGL.
	GLint nColors = src->format->BytesPerPixel;
	
	/**
	 * \todo	When compiling in nightmare mode with Visual Studio, it indicated that \c textureFormat could be potentially
	 *			uninitialized should nColors be anything other than '3' or '4' (24 bit and 32 bit color modes, respectively).
	 *			The only other expected values would be '2' (16 bit color mode) or '1' (8 bit color mode). While in practice
	 *			neither of these cases is likely to show up, it's extremely important that textureFormat is initialized to a
	 *			good default value that will work for generally all modes or one that will at least not cause a crash.
	 */
	GLenum textureFormat;
	if(nColors == 4)
	{
		if(src->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
	}
	else if(nColors == 3)     // no alpha channel
	{
		if(src->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
	}
	else
		cout << "Image is not truecolor." << std::endl;

	glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, TEXTURE_FILTER);
	glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, TEXTURE_FILTER);
	
//	if(!mNPOTSupported)
//	{
//		
//		glTexParameteri(mTextureTarget, GL_GENERATE_MIPMAP, GL_TRUE);
//		glTexImage2D(mTextureTarget, 0, nColors, src->w, src->h, 0, textureFormat, GL_UNSIGNED_BYTE, src->pixels);
//	}
//	else
//	{
		glTexImage2D(mTextureTarget, 0, nColors, src->w, src->h, 0, textureFormat, GL_UNSIGNED_BYTE, src->pixels);
//	}

	return texId;
}


/**
 * Checks through the list of textures and removes any textures
 * that haven't been accessed for more than TEXTURE_EXPIRE_TIME
 * minutes.
 */
inline void OGL_Renderer::updateTextures()
{
	int currentTick = mTimer.ms();
	if((currentTick - mLastResourceCheck) > TEXTURE_EXPIRE_TIME)
	{
		map<int, pair<GLuint, int> >::iterator i = mTextureArray.begin();
		while(i != mTextureArray.end())
		{
			if((currentTick - i->second.second) > TEXTURE_EXPIRE_TIME)
			{
				glDeleteTextures(1, &i->second.first);
				mTextureArray.erase(i++);
			}
			else
				++i;
		}
	}
}


/**
 * Checks for expected extensions and prints warning messages
 * if the video driver doesn't support the extension.
 */
bool OGL_Renderer::extensionExists(const string& extension)
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


void OGL_Renderer::checkExtensions()
{
	cout << "\tOpenGL Extensions:" << endl;
	
	// Check extension and set our texture target
	if(extensionExists("GL_ARB_texture_non_power_of_two"))
	{
		cout << "\t\t- GL_ARB_texture_non_power_of_two" << endl;
		OGL_Renderer::mTextureTarget = GL_TEXTURE_2D;
		mRequirementsMet = true;
	}
	else if(extensionExists("GL_ARB_texture_rectangle"))
	{
		cout << "\t\t- GL_ARB_texture_rectangle" << endl;
		OGL_Renderer::mTextureTarget = GL_TEXTURE_RECTANGLE_ARB;
		mRequirementsMet = true;
	}
	else
	{
		mRequirementsMet = false;
	}

}


void OGL_Renderer::initGL()
{
	
	// Finish setting up OGL state.
	glShadeModel(GL_SMOOTH);

	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, mScreen->w, mScreen->h);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT);
	//glDepthFunc(GL_NEVER);

	//glMatrixMode(GL_TEXTURE);
	//glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, (double)mScreen->w, (double)mScreen->h, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	// Spit out system graphics information.
	const SDL_VideoInfo* mVideoInfo = SDL_GetVideoInfo();
	cout << "\t- OpenGL System Info -" << endl;

	char videoDriverName[256];
	if(SDL_VideoDriverName(videoDriverName, 256) != NULL)
		mDriverName = videoDriverName;
	else
		mDriverName = "UNSPECIFIED";

	cout << "\tVideo Driver: " << mDriverName << endl;
	cout << "\tVideo Hardware Memory: " << mVideoInfo->video_mem << endl;
	checkExtensions();
}


void OGL_Renderer::initVideo(unsigned int resX, unsigned int resY, unsigned int bpp, bool fullscreen, bool vsync)
{
	SDL_putenv("SDL_VIDEO_CENTERED=center");

	// Set the app title to blank until the client sets it.
	setApplicationTitle("");

	// Initialize SDL's Video Subsystems.
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(701, "Error starting SDL Video Library", SDL_GetError());

	SDL_WM_SetCaption("Zombie Splash", "Zombie Splash");

	// Set up our Video Surface flags and build onto it with the
	// Config class. We should probably throw this into a private
	// function which builds the flags list and returns a unsigned int32.
	Uint32 sdlFlags = SDL_OPENGL;
	//sdlFlags = SDL_HWSURFACE|SDL_HWACCEL|SDL_OPENGL;

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

	mScreen = SDL_SetVideoMode(resX, resY, bpp, sdlFlags);
	if(mScreen == 0)
		throw Exception(702, "Error setting Video Mode", SDL_GetError());

	SDL_ShowCursor(false);

	if(TTF_Init() < 0)
		throw Exception(703, "Error starting TrueType Library", TTF_GetError());

	buildDisplayModeList();
	initGL();
}


/**
 * Gets wether or not the OpenGL Renderer is in a valid state
 * for rendering.
 * 
 * On most systems with halfway decent GPU hardware this check
 * is not necessary but it's good to check that the renderer will
 * be in a stable state and, if not, exit gracefully or find
 * another way to handle the problem.
 */
bool OGL_Renderer::valid() const
{
	// This feels like a hack... it's just not as clean as I think it could be.
	return mRequirementsMet;
}