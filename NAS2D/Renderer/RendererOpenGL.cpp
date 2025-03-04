// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "RendererOpenGL.h"

#include "../Math/VectorSizeRange.h"
#include "../Resource/Image.h"
#include "../Resource/Font.h"
#include "../Math/Angle.h"
#include "../Math/Trig.h"
#include "../Configuration.h"
#include "../EventHandler.h"
#include "../Filesystem.h"
#include "../Utility.h"

#include <SDL2/SDL.h>

#if defined(__XCODE_BUILD__)
#include <GLEW/GLEW.h>
#include <SDL2_image/SDL_image.h>
#else
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#endif

#include <algorithm>
#include <cmath>
#include <array>
#include <vector>
#include <stdexcept>
#include <string>


using namespace NAS2D;


namespace
{
	constexpr std::array<GLfloat, 12> rectToQuad(Rectangle<GLfloat> rect)
	{
		const auto p1 = rect.position;
		const auto p2 = rect.endPoint();

		return {
			p1.x,
			p1.y,

			p1.x,
			p2.y,

			p2.x,
			p2.y,


			p2.x,
			p2.y,

			p2.x,
			p1.y,

			p1.x,
			p1.y,
		};
	}


	constexpr auto DefaultTextureCoords = rectToQuad({{0, 0}, {1, 1}});


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
}


RendererOpenGL::Options RendererOpenGL::ReadConfigurationOptions()
{
	const auto& configuration = Utility<Configuration>::get();
	const auto& graphics = configuration["graphics"];
	return {
		{graphics.get<int>("screenwidth"), graphics.get<int>("screenheight")},
		graphics.get<bool>("fullscreen"),
		graphics.get<bool>("vsync"),
	};
}


void RendererOpenGL::WriteConfigurationOptions(const Options& options)
{
	auto& configuration = Utility<Configuration>::get();
	auto& graphics = configuration["graphics"];
	graphics.set("screenwidth", options.resolution.x);
	graphics.set("screenheight", options.resolution.y);
	graphics.set("fullscreen", options.fullscreen);
	graphics.set("vsync", options.vsync);
}


RendererOpenGL::RendererOpenGL(const std::string& title) :
	RendererOpenGL(title, ReadConfigurationOptions())
{
}


RendererOpenGL::RendererOpenGL(const std::string& title, const Options& options) :
	Renderer(title)
{
	initVideo(options.resolution, options.fullscreen, options.vsync);
}


RendererOpenGL::~RendererOpenGL()
{
	Utility<EventHandler>::get().windowResized().disconnect({this, &RendererOpenGL::onResize});

	SDL_GL_DeleteContext(sdlOglContext);
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


std::string RendererOpenGL::getVendor()
{
	return glString(GL_VENDOR);
}


std::string RendererOpenGL::getRenderer()
{
	return glString(GL_RENDERER);
}


std::string RendererOpenGL::getDriverVersion()
{
	return glString(GL_VERSION);
}


std::string RendererOpenGL::getShaderVersion()
{
	return glString(GL_SHADING_LANGUAGE_VERSION);
}


void RendererOpenGL::drawImage(const Image& image, Point<float> position, float scale, Color color)
{
	setColor(color);

	const auto imageSize = image.size().to<float>() * scale;
	const auto vertexArray = rectToQuad({position, imageSize});
	drawTexturedQuad(image.textureId(), vertexArray);
}


void RendererOpenGL::drawSubImage(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, Color color)
{
	setColor(color);

	const auto& subImageSize = subImageRect.size;
	const auto vertexArray = rectToQuad({raster, subImageSize});
	const auto imageSize = image.size().to<float>();
	const auto textureCoordArray = rectToQuad(subImageRect.skewInverseBy(imageSize));

	drawTexturedQuad(image.textureId(), vertexArray, textureCoordArray);
}


void RendererOpenGL::drawSubImageRotated(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, Angle angle, Color color)
{
	glPushMatrix();

	const auto translate = subImageRect.size.to<float>() / 2;
	const auto center = raster + translate;

	glTranslatef(center.x, center.y, 0.0f);
	glRotatef(angle.degrees(), 0.0f, 0.0f, 1.0f);

	setColor(color);

	const auto vertexArray = rectToQuad({{-translate.x, -translate.y}, translate * 2});
	const auto imageSize = image.size().to<float>();
	const auto textureCoordArray = rectToQuad(subImageRect.skewInverseBy(imageSize));

	drawTexturedQuad(image.textureId(), vertexArray, textureCoordArray);

	glPopMatrix();
}


void RendererOpenGL::drawImageRotated(const Image& image, Point<float> position, Angle angle, Color color, float scale)
{
	glPushMatrix();

	const auto halfSize = image.size().to<float>() / 2;
	const auto scaledHalfSize = halfSize * scale;
	const auto center = position + halfSize;

	glTranslatef(center.x, center.y, 0.0f);

	glRotatef(angle.degrees(), 0.0f, 0.0f, 1.0f);

	setColor(color);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const auto vertexArray = rectToQuad({{-scaledHalfSize.x, -scaledHalfSize.y}, scaledHalfSize * 2});

	drawTexturedQuad(image.textureId(), vertexArray);
	glPopMatrix();
}


void RendererOpenGL::drawImageStretched(const Image& image, const Rectangle<float>& rect, Color color)
{
	setColor(color);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const auto vertexArray = rectToQuad(rect);
	drawTexturedQuad(image.textureId(), vertexArray);
}


void RendererOpenGL::drawImageRepeated(const Image& image, const Rectangle<float>& rect)
{
	setColor(Color::White);

	glBindTexture(GL_TEXTURE_2D, image.textureId());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	const auto vertexArray = rectToQuad(rect);
	const auto imageSize = image.size().to<float>();
	const auto textureCoordArray = rectToQuad(Rectangle{{0.0f, 0.0f}, rect.size.skewInverseBy(imageSize)});

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
 * get unmanageable very quickly.
 */
void RendererOpenGL::drawSubImageRepeated(const Image& image, const Rectangle<float>& destination, const Rectangle<float>& source)
{
	clipRect(destination);

	const auto tileCountSize = destination.size.skewInverseBy(source.size).to<int>() + Vector{1, 1};
	for (const auto tileOffset : VectorSizeRange(tileCountSize))
	{
		drawSubImage(image, destination.position + tileOffset.to<float>().skewBy(source.size), source);
	}

	clipRectClear();
}


void RendererOpenGL::drawImageToImage(const Image& source, const Image& destination, Point<float> dstPoint)
{
	const auto dstPointInt = dstPoint.to<int>();
	const auto sourceSize = source.size();

	const auto sourceBoundsInDestination = Rectangle{dstPointInt, sourceSize};
	const auto destinationBounds = Rectangle{Point{0, 0}, destination.size()};

	if (!sourceBoundsInDestination.overlaps(destinationBounds))
	{
		return;
	}

	const auto availableSize = destinationBounds.endPoint() - dstPointInt;
	const auto clipSize = Vector{std::min(sourceSize.x, availableSize.x), std::min(sourceSize.y, availableSize.y)}.to<float>();

	setColor(Color::White);

	glBindTexture(GL_TEXTURE_2D, destination.textureId());

	GLuint fbo = destination.frameBufferObjectId();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, destination.textureId(), 0);
	// OpenGL expects UV texture coordinates to start at the lower left.
	const auto vertexArray = rectToQuad({{dstPoint.x, static_cast<float>(destination.size().y) - dstPoint.y}, {clipSize.x, -clipSize.y}});

	drawTexturedQuad(source.textureId(), vertexArray);
	glBindTexture(GL_TEXTURE_2D, destination.textureId());
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


void RendererOpenGL::drawLine(Point<float> startPosition, Point<float> endPosition, Color color, int lineWidth)
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);

	line(startPosition, endPosition, static_cast<float>(lineWidth), color);

	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawCircle(Point<float> position, float radius, Color color, int numSegments, Vector<float> scale)
{

	/*
	* See: http://slabode.exofire.net/circle_draw.shtml.
	*
	* Modified to support X/Y scaling to draw an ellipse.
	*/


	glDisable(GL_TEXTURE_2D);
	setColor(color);

	const auto theta = Angle::degrees(360) / static_cast<float>(numSegments);
	const auto direction = getDirectionVector(theta);
	const auto cosTheta = direction.x;
	const auto sinTheta = direction.y;

	auto offset = Vector<float>{radius, 0};

	std::vector<GLfloat> verts;
	verts.resize(static_cast<std::size_t>(numSegments) * std::size_t{2});

	for (std::size_t i = 0; i < verts.size(); i += 2)
	{
		const auto point = position + offset.skewBy(scale);
		verts[i] = point.x;
		verts[i + 1] = point.y;

		offset = {cosTheta * offset.x - sinTheta * offset.y, sinTheta * offset.x + cosTheta * offset.y};
	}

	glVertexPointer(2, GL_FLOAT, 0, verts.data());
	glDrawArrays(GL_LINE_LOOP, 0, numSegments);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawGradient(const Rectangle<float>& rect, Color c1, Color c2, Color c3, Color c4)
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
	if (rect.empty())
	{
		return;
	}

	setColor(color);
	glDisable(GL_TEXTURE_2D);

	const auto p1 = rect.position + Vector{0.5, 0.5}; // OpenGL centers pixels between integer values
	const auto p2 = rect.endPoint(); // No adjustment here so as to exclude the bottom right sides
	const GLfloat corners[] = { p1.x, p1.y, p2.x, p1.y, p2.x, p2.y, p1.x, p2.y };

	glVertexPointer(2, GL_FLOAT, 0, &corners);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawBoxFilled(const Rectangle<float>& rect, Color color)
{
	if (rect.empty())
	{
		return;
	}

	setColor(color);
	glDisable(GL_TEXTURE_2D);

	const auto vertexArray = rectToQuad(rect);
	drawTexturedQuad(0, vertexArray);

	glEnable(GL_TEXTURE_2D);
}


void RendererOpenGL::drawText(const Font& font, std::string_view text, Point<float> position, Color color)
{
	if (text.empty()) { return; }

	setColor(color);

	const auto& gml = font.metrics();
	if (gml.empty()) { return; }

	Vector<int> offset{0, 0};
	for (auto character : text)
	{
		if (character == '\n')
		{
			offset.y += font.height();
			offset.x = 0;
			continue;
		}

		const auto& gm = gml[std::clamp<std::size_t>(static_cast<uint8_t>(character), 0, 255)];

		const auto glyphCellSize = font.glyphCellSize().to<float>();
		const auto adjustX = (gm.minX < 0) ? gm.minX : 0;
		const auto vertexArray = rectToQuad({{position.x + offset.x + adjustX, position.y + offset.y}, glyphCellSize});
		const auto textureCoordArray = rectToQuad(gm.uvRect);

		drawTexturedQuad(font.textureId(), vertexArray, textureCoordArray);
		offset.x += gm.advance;
	}
}


void RendererOpenGL::clipRect(const Rectangle<float>& rect)
{
	const auto intRect = rect.to<int>();
	const auto& position = intRect.position;
	const auto& clipSize = intRect.size;
	glScissor(position.x, size().y - (position.y + clipSize.y), clipSize.x, clipSize.y);

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
	SDL_GL_SwapWindow(window);
}


void RendererOpenGL::onResize(Vector<int> newSize)
{
	const auto viewportRect = Rectangle{{0, 0}, newSize};
	setViewport(viewportRect);
	setOrthoProjection(viewportRect.to<float>());
	setResolution(newSize);
}

void RendererOpenGL::setViewport(const Rectangle<int>& viewport)
{
	const auto& position = viewport.position;
	const auto& size = viewport.size;
	glViewport(position.x, position.y, size.x, size.y);
}


void RendererOpenGL::setOrthoProjection(const Rectangle<float>& orthoBounds)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const auto bounds = orthoBounds.to<double>();
	glOrtho(bounds.position.x, bounds.endPoint().x, bounds.endPoint().y, bounds.position.y, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}


void RendererOpenGL::initGL()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, DefaultTextureCoords.data());

	onResize(size());
}


void RendererOpenGL::initSdl(Vector<int> resolution, bool fullscreen)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("SDL video initialization failed: " + std::string{SDL_GetError()});
	}

	const Uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	window = SDL_CreateWindow(title().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.x, resolution.y, sdlFlags);

	if (!window)
	{
		throw std::runtime_error("Failed to create SDL window");
	}

	mResolution = resolution;

	SDL_ShowCursor(true);
}


void RendererOpenGL::initSdlGL(bool vsync)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 4);

	SDL_GL_SetSwapInterval(vsync ? 1 : 0);

	sdlOglContext = SDL_GL_CreateContext(window);
	if (!sdlOglContext)
	{
		throw std::runtime_error("Failed to create SDL OpenGL context");
	}
}


void RendererOpenGL::initVideo(Vector<int> resolution, bool fullscreen, bool vsync)
{
	initSdl(resolution, fullscreen);
	initSdlGL(vsync);
	glewInit();
	initGL();

	Utility<EventHandler>::get().windowResized().connect({this, &RendererOpenGL::onResize});
}

// ==================================================================================
// = NON PUBLIC IMPLEMENTATION
// ==================================================================================

namespace
{
	void drawTexturedQuad(GLuint textureId, const std::array<GLfloat, 12>& verticies, const std::array<GLfloat, 12>& textureCoords)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
		glVertexPointer(2, GL_FLOAT, 0, verticies.data());
		glTexCoordPointer(2, GL_FLOAT, 0, textureCoords.data());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	}

	void line(Point<float> p1, Point<float> p2, float lineWidth, Color color)
	{

		/**
		 * The following code was developed by Chris Tsang and lifted from:
		 *
		 * http://www.codeproject.com/KB/openGL/gllinedraw.aspx
		 *
		 * Modified: Removed option for non-alpha blending and general code cleanup.
		 *
		 * This is drop-in code that may be replaced in the future.
		 */


		float Cr = color.red / 255.0f;
		float Cg = color.green / 255.0f;
		float Cb = color.blue / 255.0f;
		float Ca = color.alpha / 255.0f;

		float t = 0.0f;
		float R = 0.0f;
		float f = lineWidth - static_cast<float>(static_cast<int>(lineWidth));

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

		// Determine angle of the line to horizontal
		float tx = 0.0f, ty = 0.0f; // Core thickness of a line
		float Rx = 0.0f, Ry = 0.0f; // Fading edge of a line
		float cx = 0.0f, cy = 0.0f; // Cap of a line
		float ALW = 0.01f;
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;

		if (std::abs(dx) < ALW)
		{
			// Vertical
			tx = t;
			ty = 0.0f;

			Rx = R;
			Ry = 0.0f;

			if (lineWidth > 0.0f && lineWidth <= 1.0f)
			{
				tx = 0.5f;
				Rx = 0.0f;
			}
		}
		else if (std::abs(dy) < ALW)
		{
			// Horizontal
			tx = 0.0f;
			ty = t;

			Rx = 0.0f;
			Ry = R;

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

		// Draw the line by triangle strip
		float lineVertex[] = {
			p1.x - tx - Rx - cx,
			p1.y - ty - Ry - cy, // Fading edge1

			p2.x - tx - Rx + cx,
			p2.y - ty - Ry + cy,

			p1.x - tx - cx,
			p1.y - ty - cy, // Core

			p2.x - tx + cx,
			p2.y - ty + cy,

			p1.x + tx - cx,
			p1.y + ty - cy,

			p2.x + tx + cx,
			p2.y + ty + cy,

			p1.x + tx + Rx - cx,
			p1.y + ty + Ry - cy, // Fading edge2

			p2.x + tx + Rx + cx,
			p2.y + ty + Ry + cy,
		};

		float lineColor[] = {
			Cr,
			Cg,
			Cb,
			0,

			Cr,
			Cg,
			Cb,
			0,

			Cr,
			Cg,
			Cb,
			Ca,

			Cr,
			Cg,
			Cb,
			Ca,

			Cr,
			Cg,
			Cb,
			Ca,

			Cr,
			Cg,
			Cb,
			Ca,

			Cr,
			Cg,
			Cb,
			0,

			Cr,
			Cg,
			Cb,
			0,
		};

		glVertexPointer(2, GL_FLOAT, 0, lineVertex);
		glColorPointer(4, GL_FLOAT, 0, lineColor);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

		// Line End Caps
		if (lineWidth > 3.0f)
		{
			float lineVertex2[] = {
				p1.x - tx - cx,
				p1.y - ty - cy,

				p1.x + tx + Rx,
				p1.y + ty + Ry,

				p1.x + tx - cx,
				p1.y + ty - cy,

				p1.x + tx + Rx - cx,
				p1.y + ty + Ry - cy,

				p2.x - tx - Rx + cx,
				p2.y - ty - Ry + cy, // Cap2

				p2.x - tx - Rx,
				p2.y - ty - Ry,

				p2.x - tx + cx,
				p2.y - ty + cy,

				p2.x + tx + Rx,
				p2.y + ty + Ry,

				p2.x + tx + cx,
				p2.y + ty + cy,

				p2.x + tx + Rx + cx,
				p2.y + ty + Ry + cy,
			};

			float lineColor2[] = {
				Cr,
				Cg,
				Cb,
				0, // Cap1

				Cr,
				Cg,
				Cb,
				0,

				Cr,
				Cg,
				Cb,
				Ca,

				Cr,
				Cg,
				Cb,
				0,

				Cr,
				Cg,
				Cb,
				Ca,

				Cr,
				Cg,
				Cb,
				0,

				Cr,
				Cg,
				Cb,
				0, // Cap2

				Cr,
				Cg,
				Cb,
				0,

				Cr,
				Cg,
				Cb,
				Ca,

				Cr,
				Cg,
				Cb,
				0,

				Cr,
				Cg,
				Cb,
				Ca,

				Cr,
				Cg,
				Cb,
				0,
			};

			glVertexPointer(2, GL_FLOAT, 0, lineVertex2);
			glColorPointer(4, GL_FLOAT, 0, lineColor2);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);
		}
	}

}
