// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "RendererOpenGL.h"

#include "../Trig.h"
#include "../Configuration.h"
#include "../EventHandler.h"
#include "../Exception.h"
#include "../Filesystem.h"
#include "../MathUtils.h"
#include "../Utility.h"

#include "../Resources/FontInfo.h"
#include "../Resources/ImageInfo.h"

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace NAS2D;
using namespace NAS2D::Exception;

// UGLY ASS HACK!
// This is required here in order to remove OpenGL implementation details from Image and Font.
extern std::map<std::string, ImageInfo> imageIdMap;
extern std::map<std::string, FontInfo> fontMap;

// UGLY ASS HACK!
// This is required for mouse grabbing in the EventHandler class.
SDL_Window* underlyingWindow = nullptr;


namespace {
	SDL_GLContext oglContext; /**< Primary OpenGL render context. */

	/** Vertex coordinate pairs. Default vertex coordinates used for initializing OpenGL and for debugging. */
	GLfloat defaultVertexCoords[8] = {0.0f, 0.0f, 0.0f, 32.0f, 32.0f, 32.0f, 32.0f, 0.0f};

	/** Texture coordinate pairs. Default coordinates encompassing the entire texture. */
	GLfloat defaultTextureCoords[12] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};

	GLfloat pointVertexArray[2] = {0.0f, 0.0f};

	/** Color value array for four verts. Defaults to white or normal color. */
	GLfloat colorVertexArray[24] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

	GLfloat vertexArray[12] = {}; /**< Vertex array for quad drawing functions (all blitter functions). */
	GLfloat textureCoordArray[12] = {}; /**< Texture coordinate array for quad drawing functions (all blitter functions). */


	std::string glString(GLenum name)
	{
		const auto apiResult = glGetString(name);
		return apiResult ? reinterpret_cast<const char*>(apiResult) : "";
	}


	RendererOpenGL::Options ReadConfigurationOptions()
	{
		const auto& configuration = Utility<Configuration>::get();
		return {
			{configuration.graphicsWidth(), configuration.graphicsHeight()},
			configuration.fullscreen(),
			configuration.vsync()
		};
	}
}


// MODULE LEVEL FUNCTIONS
void fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v);
void drawVertexArray(GLuint textureId, bool useDefaultTextureCoords = true);

void line(float x1, float y1, float x2, float y2, float w, float Cr, float Cg, float Cb, float Ca);
GLuint generate_fbo(Image& image);


/**
 * C'tor
 *
 * Instantiates an RendererOpenGL object with the title of the application window.
 *
 * \param title	Title of the application window.
 */
RendererOpenGL::RendererOpenGL(const std::string& title) : RendererOpenGL(title, ReadConfigurationOptions())
{
}


RendererOpenGL::RendererOpenGL(const std::string& title, const Options& options) : Renderer(title)
{
	std::cout << "Starting OpenGL Renderer:" << std::endl;

	initVideo(options.resolution, options.fullscreen, options.vsync);
}


/**
 * D'tor.
 */
RendererOpenGL::~RendererOpenGL()
{
	Utility<EventHandler>::get().windowResized().disconnect(this, &RendererOpenGL::onResize);

	SDL_GL_DeleteContext(oglContext);
	SDL_DestroyWindow(underlyingWindow);
	underlyingWindow = nullptr;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	std::cout << "OpenGL Renderer Terminated." << std::endl;
}


void RendererOpenGL::drawImage(Image& image, Point<float> position, float scale, Color color)
{
	glColor4ub(color.red, color.green, color.blue, color.alpha);

	const auto imageSize = image.size().to<float>() * scale;
	fillVertexArray(position.x, position.y, imageSize.x, imageSize.y);
	fillTextureArray(0.0, 0.0, 1.0, 1.0);
	drawVertexArray(imageIdMap[image.name()].texture_id);
}


void RendererOpenGL::drawSubImage(Image& image, Point<float> raster, Rectangle<float> subImageRect, Color color)
{
	glColor4ub(color.red, color.green, color.blue, color.alpha);

	fillVertexArray(raster.x, raster.y, subImageRect.width, subImageRect.height);

	const auto imageSize = image.size().to<float>();
	fillTextureArray(
		subImageRect.x / imageSize.x,
		subImageRect.y / imageSize.y,
		(subImageRect.x + subImageRect.width) / imageSize.x,
		(subImageRect.y + subImageRect.height) / imageSize.y
	);

	drawVertexArray(imageIdMap[image.name()].texture_id, false);
}


void RendererOpenGL::drawSubImageRotated(Image& image, Point<float> raster, Rectangle<float> subImageRect, float degrees, Color color)
{
	glPushMatrix();

	// Find center point of the image.
	float tX = subImageRect.width / 2.0f;
	float tY = subImageRect.height / 2.0f;

	// Adjust the translation so that images appear where expected.
	glTranslatef(raster.x + tX, raster.y + tY, 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	glColor4ub(color.red, color.green, color.blue, color.alpha);

	fillVertexArray(-tX, -tY, tX * 2, tY * 2);

	const auto imageSize = image.size().to<float>();
	fillTextureArray(
		subImageRect.x / imageSize.x,
		subImageRect.y / imageSize.y,
		(subImageRect.x + subImageRect.width) / imageSize.x,
		(subImageRect.y + subImageRect.height) / imageSize.y
	);

	drawVertexArray(imageIdMap[image.name()].texture_id, false);

	glPopMatrix();
}


void RendererOpenGL::drawImageRotated(Image& image, Point<float> position, float degrees, Color color, float scale)
{
	glPushMatrix();

	// Find center point of the image.
	const auto imageCenter = image.size().to<float>() / 2;
	const auto scaledImageCenter = imageCenter * scale;

	// Adjust the translation so that images appear where expected.
	glTranslatef(position.x + imageCenter.x, position.y + imageCenter.y, 0.0f);

	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	glColor4ub(color.red, color.green, color.blue, color.alpha);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	fillVertexArray(-scaledImageCenter.x, -scaledImageCenter.y, scaledImageCenter.x * 2, scaledImageCenter.y * 2);

	drawVertexArray(imageIdMap[image.name()].texture_id);
	glPopMatrix();
}


void RendererOpenGL::drawImageStretched(Image& image, Rectangle<float> rect, Color color)
{
	glColor4ub(color.red, color.green, color.blue, color.alpha);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	fillVertexArray(rect.x, rect.y, rect.width, rect.height);
	drawVertexArray(imageIdMap[image.name()].texture_id);
}


void RendererOpenGL::drawImageRepeated(Image& image, Rectangle<float> rect)
{
	glColor4ub(255, 255, 255, 255);

	glBindTexture(GL_TEXTURE_2D, imageIdMap[image.name()].texture_id);

	// Change texture mode to repeat at edges.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	fillVertexArray(rect.x, rect.y, rect.width, rect.height);
	const auto imageSize = image.size().to<float>();
	fillTextureArray(0.0f, 0.0f, rect.width / imageSize.x, rect.height / imageSize.y);

	glVertexPointer(2, GL_FLOAT, 0, vertexArray);

	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordArray);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


/**
 * Draws part of a larger texture repeated.
 * 
 * This is a brute force method of doing this. Unfortunately OpenGL doesn't do texture
 * wrapping for only part of a texture, it only does it if geometry area is larger than
 * an entire texture.
 * 
 * There are two possible ways to get much better performance out of this: Use a fragment
 * shader (probably the simplest) or have the Renderer save the texture portion as a new
 * texture and reference it that way (bit of overhead to do a texture lookup and would
 * get unmanagable very quickly.
 */
void RendererOpenGL::drawSubImageRepeated(Image& image, const Rectangle<float>& source, const Rectangle<float>& destination)
{
	float widthReach = source.width / (destination.width - destination.x);
	float heightReach = source.height / (destination.height - destination.y);

	glEnable(GL_SCISSOR_TEST);
	glScissor(static_cast<int>(source.x), static_cast<int>(RendererOpenGL::height() - source.y - source.height), static_cast<int>(source.width), static_cast<int>(source.height));


	for (std::size_t row = 0; row <= heightReach; ++row)
	{
		for (std::size_t col = 0; col <= widthReach; ++col)
		{
			drawSubImage(image, {source.x + (col * (destination.width - destination.x)), source.y + (row * (destination.height - destination.y))}, destination);
		}
	}

	glDisable(GL_SCISSOR_TEST);
}


void RendererOpenGL::drawImageToImage(Image& source, Image& destination, const Point<float>& dstPoint)
{
	const auto dstPointInt = dstPoint.to<int>();
	const auto sourceSize = source.size();

	const auto origin = Point<int>{0, 0};

	const auto sourceBoundsInDestination = Rectangle<int>::Create(dstPointInt, sourceSize);
	const auto destinationBounds = Rectangle<int>::Create(origin, destination.size());

	// Ignore the call if the detination point is outside the bounds of destination image.
	if (!sourceBoundsInDestination.overlaps(destinationBounds))
	{
		return;
	}

	const auto availableSize = destinationBounds.endPoint() - dstPointInt;
	const auto clipSize = Vector{
		availableSize.x < sourceSize.x ? availableSize.x : sourceSize.x,
		availableSize.y < sourceSize.y ? availableSize.y : sourceSize.y
	};

	glColor4ub(255, 255, 255, 255);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, imageIdMap[destination.name()].texture_id);

	GLuint fbo = imageIdMap[destination.name()].fbo_id;
	if (fbo == 0)
	{
		fbo = generate_fbo(destination);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imageIdMap[destination.name()].texture_id, 0);
	// Flip the Y axis to keep images drawing correctly.
	fillVertexArray(dstPoint.x, static_cast<float>(destination.height()) - dstPoint.y, static_cast<float>(clipSize.x), static_cast<float>(-clipSize.y));

	drawVertexArray(imageIdMap[source.name()].texture_id);
	glBindTexture(GL_TEXTURE_2D, imageIdMap[destination.name()].texture_id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RendererOpenGL::drawPoint(Point<float> position, Color color)
{
	glDisable(GL_TEXTURE_2D);

	glColor4ub(color.red, color.green, color.blue, color.alpha);

	pointVertexArray[0] = position.x + 0.5f;
	pointVertexArray[1] = position.y + 0.5f;

	glVertexPointer(2, GL_FLOAT, 0, pointVertexArray);
	glDrawArrays(GL_POINTS, 0, 1);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawLine(Point<float> startPosition, Point<float> endPosition, Color color, int line_width)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);

	line(startPosition.x, startPosition.y, endPosition.x, endPosition.y, static_cast<float>(line_width), color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
}


/*
 * The below code originally comes from http://slabode.exofire.net/circle_draw.shtml.
 *
 * Modified to support X/Y scaling to draw an ellipse.
 */
void RendererOpenGL::drawCircle(float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int num_segments, float scale_x, float scale_y)
{
	glDisable(GL_TEXTURE_2D);
	glColor4ub(r, g, b, a);

	float theta = PI_2 / static_cast<float>(num_segments);
	float c = cosf(theta);
	float s = sinf(theta);

	float x = radius;
	float y = 0;

	GLfloat* verts = new GLfloat[static_cast<std::size_t>(num_segments) * std::size_t{2}]; // Two coords per vertex

	// During each iteration of the for loop, two indecies are accessed
	// so we need to be sure that we step two index places for each loop.
	for (int i = 0; i < num_segments * 2; i += 2)
	{
		verts[i] = x * scale_x + cx;
		verts[i + 1] = y * scale_y + cy;

		// Apply the rotation matrix
		float t = x;
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
	delete[] verts;

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawGradient(float x, float y, float w, float h, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4)
{
	glEnableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);

	colorVertexArray[0] = r1 / 255.0f;
	colorVertexArray[1] = g1 / 255.0f;
	colorVertexArray[2] = b1 / 255.0f;
	colorVertexArray[3] = a1 / 255.0f;

	colorVertexArray[4] = r2 / 255.0f;
	colorVertexArray[5] = g2 / 255.0f;
	colorVertexArray[6] = b2 / 255.0f;
	colorVertexArray[7] = a2 / 255.0f;

	colorVertexArray[8] = r3 / 255.0f;
	colorVertexArray[9] = g3 / 255.0f;
	colorVertexArray[10] = b3 / 255.0f;
	colorVertexArray[11] = a3 / 255.0f;


	colorVertexArray[12] = r3 / 255.0f;
	colorVertexArray[13] = g3 / 255.0f;
	colorVertexArray[14] = b3 / 255.0f;
	colorVertexArray[15] = a3 / 255.0f;

	colorVertexArray[16] = r4 / 255.0f;
	colorVertexArray[17] = g4 / 255.0f;
	colorVertexArray[18] = b4 / 255.0f;
	colorVertexArray[19] = a4 / 255.0f;

	colorVertexArray[20] = r1 / 255.0f;
	colorVertexArray[21] = g1 / 255.0f;
	colorVertexArray[22] = b1 / 255.0f;
	colorVertexArray[23] = a1 / 255.0f;


	fillVertexArray(x, y, w, h);
	glColorPointer(4, GL_FLOAT, 0, colorVertexArray);
	drawVertexArray(0);

	glEnable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
}


void RendererOpenGL::drawBox(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
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


void RendererOpenGL::drawBoxFilled(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	glColor4ub(r, g, b, a);
	glDisable(GL_TEXTURE_2D);

	fillVertexArray(x, y, width, height);
	drawVertexArray(0);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawText(const Font& font, std::string_view text, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	if (!font.loaded() || text.empty()) { return; }

	glColor4ub(r, g, b, a);

	int offset = 0;

	GlyphMetricsList& gml = fontMap[font.name()].metrics;
	if (gml.empty()) { return; }

	for (auto character : text)
	{
		GlyphMetrics& gm = gml[std::clamp<std::size_t>(static_cast<uint8_t>(character), 0, 255)];

		fillVertexArray(x + offset, y, static_cast<float>(font.glyphCellWidth()), static_cast<float>(font.glyphCellHeight()));
		fillTextureArray(gm.uvX, gm.uvY, gm.uvW, gm.uvH);

		drawVertexArray(fontMap[font.name()].texture_id, false);
		offset += gm.advance + gm.minX;
	}
}


void RendererOpenGL::showSystemPointer(bool _b)
{
	SDL_ShowCursor(static_cast<int>(_b));
}


void RendererOpenGL::addCursor(const std::string& filePath, int cursorId, int offx, int offy)
{
	File imageFile = Utility<Filesystem>::get().open(filePath);
	if (imageFile.size() == 0)
	{
		std::cout << "RendererOpenGL::addCursor(): '" << filePath << "' is empty." << std::endl;
		return;
	}

	SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), static_cast<int>(imageFile.size())), 0);
	if (!surface)
	{
		std::cout << "RendererOpenGL::addCursor(): " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Cursor* cur = SDL_CreateColorCursor(surface, offx, offy);
	if (!cur)
	{
		std::cout << "RendererOpenGL::addCursor(): " << SDL_GetError() << std::endl;
		return;
	}

	if (cursors.count(cursorId))
	{
		SDL_FreeCursor(cursors[cursorId]);
	}

	cursors[cursorId] = cur;

	if (cursors.size() == 1)
	{
		setCursor(cursorId);
	}
}


void RendererOpenGL::setCursor(int cursorId)
{
	SDL_SetCursor(cursors[cursorId]);
}


void RendererOpenGL::clipRect(float x, float y, float width, float height)
{
	if (width == 0 || height == 0)
	{
		glDisable(GL_SCISSOR_TEST);
		return;
	}

	glScissor(static_cast<int>(x), static_cast<int>(RendererOpenGL::height() - y - height), static_cast<int>(width), static_cast<int>(height));

	glEnable(GL_SCISSOR_TEST);
}


void RendererOpenGL::clearScreen(uint8_t r, uint8_t g, uint8_t b)
{
	glClearColor(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void RendererOpenGL::update()
{
	Renderer::update();
	SDL_GL_SwapWindow(underlyingWindow);
}


float RendererOpenGL::width() const
{
	if ((SDL_GetWindowFlags(underlyingWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		return desktopResolution.x;
	}

	return mResolution.x;
}


float RendererOpenGL::height() const
{
	if ((SDL_GetWindowFlags(underlyingWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		return desktopResolution.y;
	}

	return mResolution.y;
}


void RendererOpenGL::size(int w, int h)
{
	SDL_SetWindowSize(underlyingWindow, w, h);
	onResize(w, h);
	SDL_SetWindowPosition(underlyingWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


void RendererOpenGL::minimum_size(int w, int h)
{
	SDL_SetWindowMinimumSize(underlyingWindow, w, h);
	onResize(w, h);
}


void RendererOpenGL::fullscreen(bool fs, bool maintain)
{
	if (fs)
	{
		if (!maintain) { SDL_SetWindowFullscreen(underlyingWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); }
		else { SDL_SetWindowFullscreen(underlyingWindow, SDL_WINDOW_FULLSCREEN); }
		SDL_SetWindowResizable(underlyingWindow, SDL_FALSE);
	}
	else
	{
		SDL_SetWindowFullscreen(underlyingWindow, 0);
		SDL_SetWindowSize(underlyingWindow, static_cast<int>(width()), static_cast<int>(height()));
		SDL_SetWindowPosition(underlyingWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}


bool RendererOpenGL::fullscreen() const
{
	return ((SDL_GetWindowFlags(underlyingWindow) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN) ||
		((SDL_GetWindowFlags(underlyingWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP);
}


void RendererOpenGL::resizeable(bool resizable)
{
	if (fullscreen())
	{
		return;
	}

	#if defined(_MSC_VER)
	#pragma warning(suppress: 26812) // C26812 Warns to use enum class (C++), but SDL is a C library
	#endif
	SDL_SetWindowResizable(underlyingWindow, resizable ? SDL_TRUE : SDL_FALSE);
}


bool RendererOpenGL::resizeable() const
{
	return (SDL_GetWindowFlags(underlyingWindow) & SDL_WINDOW_RESIZABLE) == SDL_WINDOW_RESIZABLE;
}


void RendererOpenGL::onResize(int w, int h)
{
	const auto dimensions = Vector{w, h}.to<float>();
	setViewport(Rectangle{0, 0, w, h});
	setOrthoProjection(Rectangle<float>::Create(Point{0.0f, 0.0f}, dimensions));
	setResolution(dimensions);

}

void RendererOpenGL::setViewport(const Rectangle<int>& viewport)
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}


void RendererOpenGL::setOrthoProjection(const Rectangle<float>& orthoBounds)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const auto bounds = orthoBounds.to<double>();
	glOrtho(bounds.startPoint().x, bounds.endPoint().x, bounds.endPoint().y, bounds.startPoint().y, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void RendererOpenGL::window_icon(const std::string& path)
{
	if (!Utility<Filesystem>::get().exists(path)) { return; }

	File f = Utility<Filesystem>::get().open(path);
	SDL_Surface* icon = IMG_Load_RW(SDL_RWFromConstMem(f.raw_bytes(), static_cast<int>(f.size())), 0);
	if (!icon)
	{
		std::cout << "RendererOpenGL::window_icon(): " << SDL_GetError() << std::endl;
		return;
	}

	SDL_SetWindowIcon(underlyingWindow, icon);
	SDL_FreeSurface(icon);
}


void RendererOpenGL::initGL()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	onResize(static_cast<int>(width()), static_cast<int>(height()));

	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// Spit out system graphics information.
	std::cout << "\t- OpenGL System Info -" << std::endl;

	driverName(glString(GL_RENDERER));

	std::cout << "\tVendor: " << glString(GL_VENDOR) << std::endl;
	std::cout << "\tRenderer: " << driverName() << std::endl;
	std::cout << "\tDriver Version: " << glString(GL_VERSION) << std::endl;
	auto glShadingLanguageVersion = glString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "\tGLSL Version: " << glShadingLanguageVersion << std::endl;

	if (glShadingLanguageVersion.empty())
	{
		//throw renderer_no_glsl();
	}

	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, defaultVertexCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, defaultTextureCoords);
}


void RendererOpenGL::initVideo(Vector<int> resolution, bool fullscreen, bool vsync)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw renderer_backend_init_failure(SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); /// \todo	Add checks to determine an appropriate depth buffer.
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 4);

	if (vsync) { SDL_GL_SetSwapInterval(1); }
	else { SDL_GL_SetSwapInterval(0); }

	Uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	if (fullscreen) { sdlFlags = sdlFlags | SDL_WINDOW_FULLSCREEN; }

	underlyingWindow = SDL_CreateWindow(title().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.x, resolution.y, sdlFlags);

	if (!underlyingWindow)
	{
		throw renderer_window_creation_failure();
	}

	mResolution = resolution.to<float>();

	oglContext = SDL_GL_CreateContext(underlyingWindow);
	if (!oglContext)
	{
		throw renderer_opengl_context_failure();
	}

	SDL_ShowCursor(true);
	glewInit();
	initGL();

	Utility<EventHandler>::get().windowResized().connect(this, &RendererOpenGL::onResize);

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		std::cout << "SDL_GetDesktopDisplayMode failed: " << SDL_GetError();
		throw std::runtime_error("Unable to get desktop dislay mode: " + std::string(SDL_GetError()));
	}

	desktopResolution = {static_cast<float>(dm.w), static_cast<float>(dm.h)};
}

std::vector<DisplayDesc> RendererOpenGL::getDisplayModes() const
{
	const auto displayIndex = SDL_GetWindowDisplayIndex(underlyingWindow);
	const auto numResolutions = SDL_GetNumDisplayModes(displayIndex);
	if (numResolutions < 0)
	{
		throw std::runtime_error("Error getting number of display modes for display index: " + std::to_string(displayIndex) + " : " + std::string{SDL_GetError()});
	}

	std::vector<DisplayDesc> result{};
	result.reserve(static_cast<std::size_t>(numResolutions));
	for (int i = 0; i < numResolutions; ++i)
	{
		SDL_DisplayMode currentMode{};
		SDL_GetDisplayMode(displayIndex, i, &currentMode);
		result.push_back({currentMode.w, currentMode.h, currentMode.refresh_rate});
	}
	return result;
}

DisplayDesc RendererOpenGL::getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const
{
	const auto displayIndex = SDL_GetWindowDisplayIndex(underlyingWindow);
	SDL_DisplayMode preferredMode{};
	preferredMode.w = preferredDisplayDesc.width;
	preferredMode.h = preferredDisplayDesc.height;
	preferredMode.refresh_rate = preferredDisplayDesc.refreshHz;

	SDL_DisplayMode closestMode{};
	if (SDL_GetClosestDisplayMode(displayIndex, &preferredMode, &closestMode))
	{
		return {closestMode.w, closestMode.h, closestMode.refresh_rate};
	}
	throw std::runtime_error("No matching display mode for " + std::string{preferredDisplayDesc});
}

Vector<int> RendererOpenGL::getWindowClientArea() const noexcept
{
	Vector<int> size;
	SDL_GetWindowSize(underlyingWindow, &size.x, &size.y);
	return size;
}

// ==================================================================================
// = NON PUBLIC IMPLEMENTATION
// ==================================================================================

/**
 * Generates an OpenGL Frame Buffer Object.
 */
GLuint generate_fbo(Image& image)
{
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	if (imageIdMap[image.name()].texture_id == 0)
	{
		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		const auto textureFormat = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGRA : GL_RGBA;

		const auto imageSize = image.size();
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, imageSize.x, imageSize.y, 0, textureFormat, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imageIdMap[image.name()].texture_id, 0);

	imageIdMap[image.name()].fbo_id = framebuffer;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return framebuffer;
}


/**
 * Draws a textured rectangle using a vertex and texture coordinate array
 */
void drawVertexArray(GLuint textureId, bool useDefaultTextureCoords)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glVertexPointer(2, GL_FLOAT, 0, vertexArray);

	// Choose from the default texture coordinates or from a custom set.
	if (useDefaultTextureCoords) { glTexCoordPointer(2, GL_FLOAT, 0, defaultTextureCoords); }
	else { glTexCoordPointer(2, GL_FLOAT, 0, textureCoordArray); }

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}


/**
 * Fills a vertex array with quad vertex information.
 */
void fillVertexArray(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	vertexArray[0] = static_cast<GLfloat>(x); vertexArray[1] = static_cast<GLfloat>(y);
	vertexArray[2] = static_cast<GLfloat>(x); vertexArray[3] = static_cast<GLfloat>(y + h);
	vertexArray[4] = static_cast<GLfloat>(x + w); vertexArray[5] = static_cast<GLfloat>(y + h);

	vertexArray[6] = static_cast<GLfloat>(x + w); vertexArray[7] = static_cast<GLfloat>(y + h);
	vertexArray[8] = static_cast<GLfloat>(x + w); vertexArray[9] = static_cast<GLfloat>(y);
	vertexArray[10] = static_cast<GLfloat>(x); vertexArray[11] = static_cast<GLfloat>(y);
}


/**
 * Fills a texture coordinate array with quad vertex information.
 */
void fillTextureArray(GLfloat x, GLfloat y, GLfloat u, GLfloat v)
{
	textureCoordArray[0] = static_cast<GLfloat>(x); textureCoordArray[1] = static_cast<GLfloat>(y);
	textureCoordArray[2] = static_cast<GLfloat>(x); textureCoordArray[3] = static_cast<GLfloat>(v);
	textureCoordArray[4] = static_cast<GLfloat>(u); textureCoordArray[5] = static_cast<GLfloat>(v);

	textureCoordArray[6] = static_cast<GLfloat>(u); textureCoordArray[7] = static_cast<GLfloat>(v);
	textureCoordArray[8] = static_cast<GLfloat>(u); textureCoordArray[9] = static_cast<GLfloat>(y);
	textureCoordArray[10] = static_cast<GLfloat>(x); textureCoordArray[11] = static_cast<GLfloat>(y);
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
void line(float x1, float y1, float x2, float y2, float w, float Cr, float Cg, float Cb, float Ca)
{
	// What are these values for?
	float t = 0.0f;
	float R = 0.0f;
	float f = w - static_cast<int>(w);

	// HOLY CRAP magic numbers!
	//determine parameters t, R
	if (w >= 0.0f && w < 1.0f)
	{
		t = 0.05f;
		R = 0.48f + 0.32f * f;
	}
	else if (w >= 1.0f && w < 2.0f)
	{
		t = 0.05f + f * 0.33f;
		R = 0.768f + 0.312f * f;
	}
	else if (w >= 2.0f && w < 3.0f)
	{
		t = 0.38f + f * 0.58f;
		R = 1.08f;
	}
	else if (w >= 3.0f && w < 4.0f)
	{
		t = 0.96f + f * 0.48f;
		R = 1.08f;
	}
	else if (w >= 4.0f && w < 5.0f)
	{
		t = 1.44f + f * 0.46f;
		R = 1.08f;
	}
	else if (w >= 5.0f && w < 6.0f)
	{
		t = 1.9f + f * 0.6f;
		R = 1.08f;
	}
	else if (w >= 6.0f)
	{
		float ff = w - 6.0f;
		t = 2.5f + ff * 0.50f;
		R = 1.08f;
	}
	//printf( "w=%f, f=%f, C=%.4f\n", w, f, C);

	//determine angle of the line to horizontal
	float tx = 0.0f, ty = 0.0f; //core thinkness of a line
	float Rx = 0.0f, Ry = 0.0f; //fading edge of a line
	float cx = 0.0f, cy = 0.0f; //cap of a line
	float ALW = 0.01f; // Dafuq is this?
	float dx = x2 - x1;
	float dy = y2 - y1;

	if (std::abs(dx) < ALW)
	{
		//vertical
		tx = t; ty = 0.0f;
		Rx = R; Ry = 0.0f;
		if (w > 0.0f && w <= 1.0f)
		{
			tx = 0.5f;
			Rx = 0.0f;
		}
	}
	else if (std::abs(dy) < ALW)
	{
		//horizontal
		tx = 0.0f; ty = t;
		Rx = 0.0f; Ry = R;
		if (w > 0.0f && w <= 1.0f)
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
	float line_vertex[] =
	{
		x1 - tx - Rx - cx, y1 - ty - Ry - cy, //fading edge1
		x2 - tx - Rx + cx, y2 - ty - Ry + cy,
		x1 - tx - cx, y1 - ty - cy,        //core
		x2 - tx + cx, y2 - ty + cy,
		x1 + tx - cx, y1 + ty - cy,
		x2 + tx + cx, y2 + ty + cy,
		x1 + tx + Rx - cx, y1 + ty + Ry - cy, //fading edge2
		x2 + tx + Rx + cx, y2 + ty + Ry + cy
	};

	float line_color[] =
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

	glVertexPointer(2, GL_FLOAT, 0, line_vertex);
	glColorPointer(4, GL_FLOAT, 0, line_color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	// Line End Caps
	if (w > 3.0f) // <<< Arbitrary number.
	{
		float line_vertex2[] =
		{
			x1 - tx - cx, y1 - ty - cy,
			x1 + tx + Rx, y1 + ty + Ry,
			x1 + tx - cx, y1 + ty - cy,
			x1 + tx + Rx - cx, y1 + ty + Ry - cy,
			x2 - tx - Rx + cx, y2 - ty - Ry + cy, //cap2
			x2 - tx - Rx, y2 - ty - Ry,
			x2 - tx + cx, y2 - ty + cy,
			x2 + tx + Rx, y2 + ty + Ry,
			x2 + tx + cx, y2 + ty + cy,
			x2 + tx + Rx + cx, y2 + ty + Ry + cy
		};

		float line_color2[] =
		{
			Cr, Cg, Cb, 0, //cap1
			Cr, Cg, Cb, 0,
			Cr, Cg, Cb, Ca,
			Cr, Cg, Cb, 0,
			Cr, Cg, Cb, Ca,
			Cr, Cg, Cb, 0,
			Cr, Cg, Cb, 0, //cap2
			Cr, Cg, Cb, 0,
			Cr, Cg, Cb, Ca,
			Cr, Cg, Cb, 0,
			Cr, Cg, Cb, Ca,
			Cr, Cg, Cb, 0
		};

		glVertexPointer(2, GL_FLOAT, 0, line_vertex2);
		glColorPointer(4, GL_FLOAT, 0, line_color2);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);
	}
}
