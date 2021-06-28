// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "Renderer.h"

#include <map>


using SDL_GLContext = void*;
struct SDL_Cursor;


namespace NAS2D {

class RendererOpenGL : public Renderer
{
public:
	struct Options
	{
		Vector<int> resolution;
		bool fullscreen;
		bool vsync;
	};

	static Options ReadConfigurationOptions();
	static void WriteConfigurationOptions(const Options& options);

	RendererOpenGL() = delete;
	explicit RendererOpenGL(const std::string& title);
	RendererOpenGL(const std::string& title, const Options& options);
	RendererOpenGL(const RendererOpenGL& other) = delete;
	RendererOpenGL(RendererOpenGL&& other) = delete;
	RendererOpenGL& operator=(const RendererOpenGL& rhs) = delete;
	RendererOpenGL& operator=(RendererOpenGL&& rhs) = delete;
	virtual ~RendererOpenGL() override;

	std::vector<DisplayDesc> getDisplayModes() const override;
	DisplayDesc getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const override;
	Vector<int> getWindowClientArea() const noexcept override;

	void drawImage(const Image& image, Point<float> position, float scale = 1.0, Color color = Color::Normal) override;

	void drawSubImage(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, Color color = Color::Normal) override;
	void drawSubImageRotated(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, float degrees, Color color = Color::Normal) override;

	void drawImageRotated(const Image& image, Point<float> position, float degrees, Color color = Color::Normal, float scale = 1.0f) override;
	void drawImageStretched(const Image& image, const Rectangle<float>& rect, Color color = Color::Normal) override;

	void drawImageRepeated(const Image& image, const Rectangle<float>& rect) override;
	void drawSubImageRepeated(const Image& image, const Rectangle<float>& destination, const Rectangle<float>& source) override;

	void drawImageToImage(const Image& source, const Image& destination, Point<float> dstPoint) override;

	void drawPoint(Point<float> position, Color color = Color::White) override;
	void drawLine(Point<float> startPosition, Point<float> endPosition, Color color = Color::White, int line_width = 1) override;
	void drawBox(const Rectangle<float>& rect, Color color = Color::White) override;
	void drawBoxFilled(const Rectangle<float>& rect, Color color = Color::White) override;
	void drawCircle(Point<float> position, float radius, Color color, int num_segments = 10, Vector<float> scale = Vector{1.0f, 1.0f}) override;

	void drawGradient(const Rectangle<float>& rect, Color c1, Color c2, Color c3, Color c4) override;

	void drawText(const Font& font, std::string_view text, Point<float> position, Color color = Color::White) override;

	void showSystemPointer(bool) override;
	void addCursor(const std::string& filePath, int cursorId, int offx, int offy) override;
	void setCursor(int cursorId) override;

	void clearScreen(Color color = Color::Black) override;

	Vector<int> size() const override;
	void size(Vector<int> newSize) override;
	void minimumSize(Vector<int> newSize) override;

	void fullscreen(bool fs, bool maintain = false) override;
	bool fullscreen() const override;

	void resizeable(bool resizable) override;
	bool resizeable() const override;

	void clipRect(const Rectangle<float>& rect) override;
	void clipRectClear() override;

	void window_icon(const std::string& path) override;

	void update() override;

	void setViewport(const Rectangle<int>& viewport) override;
	void setOrthoProjection(const Rectangle<float>& orthoBounds) override;

private:

	void initGL();
	void initVideo(Vector<int> resolution, bool fullscreen, bool vsync);

	void onResize(Vector<int> newSize);


	SDL_GLContext sdlOglContext; /**< Primary OpenGL render context. */
	Vector<int> desktopResolution;
	std::map<int, SDL_Cursor*> cursors;
};

} // namespace
