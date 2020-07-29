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

#include "VectorSizeRange.h"
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
#include <array>
#include <vector>


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
	/**
	 * Fills a vertex array with quad vertex information.
	 */
	constexpr std::array<GLfloat, 12> rectToQuad(Rectangle<GLfloat> rect)
	{
		const auto p1 = rect.startPoint();
		const auto p2 = rect.endPoint();

		return {
			p1.x, p1.y,
			p1.x, p2.y,
			p2.x, p2.y,

			p2.x, p2.y,
			p2.x, p1.y,
			p1.x, p1.y
		};
	}


	/** Texture coordinate pairs. Default coordinates encompassing the entire texture. */
	constexpr std::array<GLfloat, 12> DefaultTextureCoords = rectToQuad({0, 0, 1, 1});


	GLuint generate_fbo(const Image& image);
	void drawTexturedQuad(GLuint textureId, const std::array<GLfloat, 12>& verticies, const std::array<GLfloat, 12>& textureCoords = DefaultTextureCoords);
	void line(Point<float> p1, Point<float> p2, float lineWidth, Color color);

	void setColor(Color color)
	{
		glColor4ub(color.red, color.green, color.blue, color.alpha);
	}

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

	SDL_GL_DeleteContext(sdlOglContext);
	SDL_DestroyWindow(underlyingWindow);
	underlyingWindow = nullptr;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	std::cout << "OpenGL Renderer Terminated." << std::endl;
}


void RendererOpenGL::drawImage(const Image& image, Point<float> position, float scale, Color color)
{
	setColor(color);

	const auto imageSize = image.size().to<float>() * scale;
	const auto vertexArray = rectToQuad({position.x, position.y, imageSize.x, imageSize.y});
	drawTexturedQuad(imageIdMap[image.name()].textureId, vertexArray);
}


void RendererOpenGL::drawSubImage(const Image& image, Point<float> raster, Rectangle<float> subImageRect, Color color)
{
	setColor(color);

	const auto vertexArray = rectToQuad({raster.x, raster.y, subImageRect.width, subImageRect.height});
	const auto imageSize = image.size().to<float>();
	const auto textureCoordArray = rectToQuad(subImageRect.skewInverseBy(imageSize));

	drawTexturedQuad(imageIdMap[image.name()].textureId, vertexArray, textureCoordArray);
}


void RendererOpenGL::drawSubImageRotated(const Image& image, Point<float> raster, Rectangle<float> subImageRect, float degrees, Color color)
{
	glPushMatrix();

	// Find center point of the image.
	const auto translate = subImageRect.size().to<float>() / 2;
	const auto center = raster + translate;

	// Adjust the translation so that images appear where expected.
	glTranslatef(center.x, center.y, 0.0f);
	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	setColor(color);

	const auto vertexArray = rectToQuad({-translate.x, -translate.y, translate.x * 2, translate.y * 2});
	const auto imageSize = image.size().to<float>();
	const auto textureCoordArray = rectToQuad(subImageRect.skewInverseBy(imageSize));

	drawTexturedQuad(imageIdMap[image.name()].textureId, vertexArray, textureCoordArray);

	glPopMatrix();
}


void RendererOpenGL::drawImageRotated(const Image& image, Point<float> position, float degrees, Color color, float scale)
{
	glPushMatrix();

	// Find center point of the image.
	const auto halfSize = image.size().to<float>() / 2;
	const auto scaledHalfSize = halfSize * scale;
	const auto center = position + halfSize;

	// Adjust the translation so that images appear where expected.
	glTranslatef(center.x, center.y, 0.0f);

	glRotatef(degrees, 0.0f, 0.0f, 1.0f);

	setColor(color);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const auto vertexArray = rectToQuad({-scaledHalfSize.x, -scaledHalfSize.y, scaledHalfSize.x * 2, scaledHalfSize.y * 2});

	drawTexturedQuad(imageIdMap[image.name()].textureId, vertexArray);
	glPopMatrix();
}


void RendererOpenGL::drawImageStretched(const Image& image, Rectangle<float> rect, Color color)
{
	setColor(color);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const auto vertexArray = rectToQuad(rect);
	drawTexturedQuad(imageIdMap[image.name()].textureId, vertexArray);
}


void RendererOpenGL::drawImageRepeated(const Image& image, Rectangle<float> rect)
{
	setColor(Color::White);

	glBindTexture(GL_TEXTURE_2D, imageIdMap[image.name()].textureId);

	// Change texture mode to repeat at edges.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	const auto vertexArray = rectToQuad(rect);
	const auto imageSize = image.size().to<float>();
	const auto textureCoordArray = rectToQuad(Rectangle<float>::Create({0.0f, 0.0f}, rect.size().skewInverseBy(imageSize)));

	glVertexPointer(2, GL_FLOAT, 0, vertexArray.data());

	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordArray.data());
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
void RendererOpenGL::drawSubImageRepeated(const Image& image, const Rectangle<float>& destination, const Rectangle<float>& source)
{
	clipRect(destination);

	const auto tileCountSize = destination.size().skewInverseBy(source.size()).to<int>() + Vector{1, 1};
	for (const auto tileOffset : VectorSizeRange(tileCountSize))
	{
		drawSubImage(image, destination.startPoint() + tileOffset.to<float>().skewBy(source.size()), source);
	}

	clipRectClear();
}


void RendererOpenGL::drawImageToImage(const Image& source, const Image& destination, const Point<float>& dstPoint)
{
	const auto dstPointInt = dstPoint.to<int>();
	const auto sourceSize = source.size();

	const auto sourceBoundsInDestination = Rectangle<int>::Create(dstPointInt, sourceSize);
	const auto destinationBounds = Rectangle<int>::Create(Point{0, 0}, destination.size());

	// Ignore the call if the detination point is outside the bounds of destination image.
	if (!sourceBoundsInDestination.overlaps(destinationBounds))
	{
		return;
	}

	const auto availableSize = destinationBounds.endPoint() - dstPointInt;
	const auto clipSize = Vector{
		std::min(sourceSize.x, availableSize.x),
		std::min(sourceSize.y, availableSize.y)
	}.to<float>();

	setColor(Color::White);

	const auto& destinationImageInfo = imageIdMap[destination.name()];
	glBindTexture(GL_TEXTURE_2D, destinationImageInfo.textureId);

	GLuint fbo = destinationImageInfo.frameBufferObjectId;
	if (fbo == 0)
	{
		fbo = generate_fbo(destination);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, destinationImageInfo.textureId, 0);
	// Flip the Y axis to keep images drawing correctly.
	const auto vertexArray = rectToQuad({dstPoint.x, static_cast<float>(destination.size().y) - dstPoint.y, clipSize.x, -clipSize.y});

	drawTexturedQuad(imageIdMap[source.name()].textureId, vertexArray);
	glBindTexture(GL_TEXTURE_2D, destinationImageInfo.textureId);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RendererOpenGL::drawPoint(Point<float> position, Color color)
{
	glDisable(GL_TEXTURE_2D);

	setColor(color);

	GLfloat pointVertexArray[2] = {position.x + 0.5f, position.y + 0.5f};

	glVertexPointer(2, GL_FLOAT, 0, pointVertexArray);
	glDrawArrays(GL_POINTS, 0, 1);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawLine(Point<float> startPosition, Point<float> endPosition, Color color, int line_width)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);

	line(startPosition, endPosition, static_cast<float>(line_width), color);

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
}


/*
 * The below code originally comes from http://slabode.exofire.net/circle_draw.shtml.
 *
 * Modified to support X/Y scaling to draw an ellipse.
 */
void RendererOpenGL::drawCircle(Point<float> position, float radius, Color color, int num_segments, Vector<float> scale)
{
	glDisable(GL_TEXTURE_2D);
	setColor(color);

	auto theta = PI_2 / static_cast<float>(num_segments);
	auto cosTheta = std::cos(theta);
	auto sinTheta = std::sin(theta);

	auto offset = Vector<float>{radius, 0};

	std::vector<GLfloat> verts;
	verts.resize(static_cast<std::size_t>(num_segments) * std::size_t{2}); // Two coords per vertex

	// During each iteration of the for loop, two indecies are accessed
	// so we need to be sure that we step two index places for each loop.
	for (std::size_t i = 0; i < verts.size(); i += 2)
	{
		const auto point = position + offset.skewBy(scale);
		verts[i] = point.x;
		verts[i + 1] = point.y;

		// Apply the rotation matrix
		offset = {cosTheta * offset.x - sinTheta * offset.y, sinTheta * offset.x + cosTheta * offset.y};
	}

	glVertexPointer(2, GL_FLOAT, 0, verts.data());
	glDrawArrays(GL_LINE_LOOP, 0, num_segments);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawGradient(Rectangle<float> rect, Color c1, Color c2, Color c3, Color c4)
{
	glEnableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);

	GLfloat colorVertexArray[24];

	colorVertexArray[0] = c1.red / 255.0f;
	colorVertexArray[1] = c1.green / 255.0f;
	colorVertexArray[2] = c1.blue / 255.0f;
	colorVertexArray[3] = c1.alpha / 255.0f;

	colorVertexArray[4] = c2.red / 255.0f;
	colorVertexArray[5] = c2.green / 255.0f;
	colorVertexArray[6] = c2.blue / 255.0f;
	colorVertexArray[7] = c2.alpha / 255.0f;

	colorVertexArray[8] = c3.red / 255.0f;
	colorVertexArray[9] = c3.green / 255.0f;
	colorVertexArray[10] = c3.blue / 255.0f;
	colorVertexArray[11] = c3.alpha / 255.0f;


	colorVertexArray[12] = c3.red / 255.0f;
	colorVertexArray[13] = c3.green / 255.0f;
	colorVertexArray[14] = c3.blue / 255.0f;
	colorVertexArray[15] = c3.alpha / 255.0f;

	colorVertexArray[16] = c4.red / 255.0f;
	colorVertexArray[17] = c4.green / 255.0f;
	colorVertexArray[18] = c4.blue / 255.0f;
	colorVertexArray[19] = c4.alpha / 255.0f;

	colorVertexArray[20] = c1.red / 255.0f;
	colorVertexArray[21] = c1.green / 255.0f;
	colorVertexArray[22] = c1.blue / 255.0f;
	colorVertexArray[23] = c1.alpha / 255.0f;


	const auto vertexArray = rectToQuad(rect);
	glColorPointer(4, GL_FLOAT, 0, colorVertexArray);
	drawTexturedQuad(0, vertexArray);

	glEnable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
}


void RendererOpenGL::drawBox(const Rectangle<float>& rect, Color color)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);

	const auto adjustedCrossYPoint = rect.crossYPoint() + Vector{0.0f, 0.5f};
	const auto adjustedEndPoint = rect.endPoint() + Vector{0.0f, 0.5f};

	line(rect.startPoint(), rect.crossXPoint(), 1.0f, color);
	line(rect.startPoint(), adjustedCrossYPoint, 1.0f, color);
	line(adjustedCrossYPoint, adjustedEndPoint, 1.0f, color);
	line(rect.crossXPoint(), adjustedEndPoint, 1.0f, color);

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawBoxFilled(const Rectangle<float>& rect, Color color)
{
	setColor(color);
	glDisable(GL_TEXTURE_2D);

	const auto vertexArray = rectToQuad(rect);
	drawTexturedQuad(0, vertexArray);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawText(const Font& font, std::string_view text, Point<float> position, Color color)
{
	if (!font.loaded() || text.empty()) { return; }

	setColor(color);

	GlyphMetricsList& gml = fontMap[font.name()].metrics;
	if (gml.empty()) { return; }

	int offset = 0;
	for (auto character : text)
	{
		GlyphMetrics& gm = gml[std::clamp<std::size_t>(static_cast<uint8_t>(character), 0, 255)];

		const auto glyphCellSize = font.glyphCellSize().to<float>();
		const auto vertexArray = rectToQuad({position.x + offset, position.y, glyphCellSize.x, glyphCellSize.y});
		const auto textureCoordArray = rectToQuad(gm.uvRect);

		drawTexturedQuad(fontMap[font.name()].textureId, vertexArray, textureCoordArray);
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


void RendererOpenGL::clipRect(const Rectangle<float>& rect)
{
	const auto intRect = rect.to<int>();
	glScissor(intRect.x, size().y - (intRect.y + intRect.height), intRect.width, intRect.height);

	glEnable(GL_SCISSOR_TEST);
}


void RendererOpenGL::clipRectClear()
{
	glDisable(GL_SCISSOR_TEST);
}


void RendererOpenGL::clearScreen(Color color)
{
	glClearColor(static_cast<float>(color.red) / 255.0f, static_cast<float>(color.green) / 255.0f, static_cast<float>(color.blue) / 255.0f, static_cast<float>(color.alpha) / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void RendererOpenGL::update()
{
	Renderer::update();
	SDL_GL_SwapWindow(underlyingWindow);
}


Vector<int> RendererOpenGL::size() const
{
	if ((SDL_GetWindowFlags(underlyingWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		return desktopResolution;
	}

	return mResolution;
}


void RendererOpenGL::size(Vector<int> newSize)
{
	SDL_SetWindowSize(underlyingWindow, newSize.x, newSize.y);
	onResize(newSize.x, newSize.y);
	SDL_SetWindowPosition(underlyingWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


void RendererOpenGL::minimumSize(Vector<int> newSize)
{
	SDL_SetWindowMinimumSize(underlyingWindow, newSize.x, newSize.y);
	onResize(newSize.x, newSize.y);
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
		SDL_SetWindowSize(underlyingWindow, size().x, size().y);
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
	const auto dimensions = Vector{w, h};
	setViewport(Rectangle{0, 0, w, h});
	setOrthoProjection(Rectangle<float>::Create(Point{0.0f, 0.0f}, dimensions.to<float>()));
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

	onResize(size().x, size().y);

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

	mResolution = resolution;

	sdlOglContext = SDL_GL_CreateContext(underlyingWindow);
	if (!sdlOglContext)
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

	desktopResolution = {dm.w, dm.h};
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

namespace {
/**
 * Generates an OpenGL Frame Buffer Object.
 */
GLuint generate_fbo(const Image& image)
{
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	if (imageIdMap[image.name()].textureId == 0)
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

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imageIdMap[image.name()].textureId, 0);

	imageIdMap[image.name()].frameBufferObjectId = framebuffer;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return framebuffer;
}


/**
 * Draws a textured rectangle using a vertex and texture coordinate array
 */
void drawTexturedQuad(GLuint textureId, const std::array<GLfloat, 12>& verticies, const std::array<GLfloat, 12>& textureCoords)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glVertexPointer(2, GL_FLOAT, 0, verticies.data());
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoords.data());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
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
void line(Point<float> p1, Point<float> p2, float lineWidth, Color color)
{
	float Cr = color.red / 255.0f;
	float Cg = color.green / 255.0f;
	float Cb = color.blue / 255.0f;
	float Ca = color.alpha / 255.0f;

	// What are these values for?
	float t = 0.0f;
	float R = 0.0f;
	float f = lineWidth - static_cast<int>(lineWidth);

	// HOLY CRAP magic numbers!
	//determine parameters t, R
	if (lineWidth >= 0.0f && lineWidth < 1.0f)
	{
		t = 0.05f;
		R = 0.48f + 0.32f * f;
	}
	else if (lineWidth >= 1.0f && lineWidth < 2.0f)
	{
		t = 0.05f + f * 0.33f;
		R = 0.768f + 0.312f * f;
	}
	else if (lineWidth >= 2.0f && lineWidth < 3.0f)
	{
		t = 0.38f + f * 0.58f;
		R = 1.08f;
	}
	else if (lineWidth >= 3.0f && lineWidth < 4.0f)
	{
		t = 0.96f + f * 0.48f;
		R = 1.08f;
	}
	else if (lineWidth >= 4.0f && lineWidth < 5.0f)
	{
		t = 1.44f + f * 0.46f;
		R = 1.08f;
	}
	else if (lineWidth >= 5.0f && lineWidth < 6.0f)
	{
		t = 1.9f + f * 0.6f;
		R = 1.08f;
	}
	else if (lineWidth >= 6.0f)
	{
		float ff = lineWidth - 6.0f;
		t = 2.5f + ff * 0.50f;
		R = 1.08f;
	}
	//printf( "lineWidth=%f, f=%f, C=%.4f\n", lineWidth, f, C);

	//determine angle of the line to horizontal
	float tx = 0.0f, ty = 0.0f; //core thinkness of a line
	float Rx = 0.0f, Ry = 0.0f; //fading edge of a line
	float cx = 0.0f, cy = 0.0f; //cap of a line
	float ALW = 0.01f; // Dafuq is this?
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	if (std::abs(dx) < ALW)
	{
		//vertical
		tx = t; ty = 0.0f;
		Rx = R; Ry = 0.0f;
		if (lineWidth > 0.0f && lineWidth <= 1.0f)
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
		if (lineWidth > 0.0f && lineWidth <= 1.0f)
		{
			ty = 0.5f;
			Ry = 0.0f;
		}
	}
	else
	{
		dx = p1.y - p2.y;
		dy = p2.x - p1.x;

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

	p1.x += cx * 0.5f;
	p1.y += cy * 0.5f;

	p2.x -= cx * 0.5f;
	p2.y -= cy * 0.5f;

	//draw the line by triangle strip
	float line_vertex[] =
	{
		p1.x - tx - Rx - cx, p1.y - ty - Ry - cy, //fading edge1
		p2.x - tx - Rx + cx, p2.y - ty - Ry + cy,
		p1.x - tx - cx, p1.y - ty - cy,        //core
		p2.x - tx + cx, p2.y - ty + cy,
		p1.x + tx - cx, p1.y + ty - cy,
		p2.x + tx + cx, p2.y + ty + cy,
		p1.x + tx + Rx - cx, p1.y + ty + Ry - cy, //fading edge2
		p2.x + tx + Rx + cx, p2.y + ty + Ry + cy
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
	if (lineWidth > 3.0f) // <<< Arbitrary number.
	{
		float line_vertex2[] =
		{
			p1.x - tx - cx, p1.y - ty - cy,
			p1.x + tx + Rx, p1.y + ty + Ry,
			p1.x + tx - cx, p1.y + ty - cy,
			p1.x + tx + Rx - cx, p1.y + ty + Ry - cy,
			p2.x - tx - Rx + cx, p2.y - ty - Ry + cy, //cap2
			p2.x - tx - Rx, p2.y - ty - Ry,
			p2.x - tx + cx, p2.y - ty + cy,
			p2.x + tx + Rx, p2.y + ty + Ry,
			p2.x + tx + cx, p2.y + ty + cy,
			p2.x + tx + Rx + cx, p2.y + ty + Ry + cy
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

}
