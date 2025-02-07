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

#include "Color.h"
#include "Window.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"
#include "../Signal/Signal.h"

#include <chrono>
#include <string_view>
#include <string>
#include <vector>


namespace NAS2D
{

	class Font;
	class Image;
	class Angle;

	template <typename BaseType>
	struct Rectangle;


	class Renderer : public Window
	{
	public:
		Renderer() = default;
		Renderer(const Renderer& rhs) = default;
		Renderer& operator=(const Renderer& rhs) = default;
		Renderer(Renderer&& rhs) = default;
		Renderer& operator=(Renderer&& rhs) = default;

		virtual void drawImage(const Image& image, Point<float> position, float scale = 1.0, Color color = Color::Normal) = 0;
		virtual void drawSubImage(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, Color color = Color::Normal) = 0;
		virtual void drawSubImageRotated(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, Angle angle, Color color = Color::Normal) = 0;
		virtual void drawImageRotated(const Image& image, Point<float> position, Angle angle, Color color = Color::Normal, float scale = 1.0f) = 0;
		virtual void drawImageStretched(const Image& image, const Rectangle<float>& rect, Color color = Color::Normal) = 0;
		virtual void drawImageRepeated(const Image& image, const Rectangle<float>& rect) = 0;
		virtual void drawSubImageRepeated(const Image& image, const Rectangle<float>& destination, const Rectangle<float>& source) = 0;

		virtual void drawImageToImage(const Image& source, const Image& destination, Point<float> dstPoint) = 0;

		virtual void drawPoint(Point<float> position, Color color = Color::White) = 0;
		virtual void drawLine(Point<float> startPosition, Point<float> endPosition, Color color = Color::White, int line_width = 1) = 0;
		virtual void drawBox(const Rectangle<float>& rect, Color color = Color::White) = 0;
		virtual void drawBoxFilled(const Rectangle<float>& rect, Color color = Color::White) = 0;
		virtual void drawCircle(Point<float> position, float radius, Color color, int num_segments = 10, Vector<float> scale = Vector{1.0f, 1.0f}) = 0;

		virtual void drawGradient(const Rectangle<float>& rect, Color colorUpperLeft, Color colorLowerLeft, Color colorLowerRight, Color colorUpperRight) = 0;

		virtual void drawText(const Font& font, std::string_view text, Point<float> position, Color color = Color::White) = 0;
		void drawTextShadow(const Font& font, std::string_view text, Point<float> position, Vector<float> shadowOffset, Color textColor, Color shadowColor);

		virtual void clearScreen(Color color = Color::Black) = 0;

		Point<int> center() const;

		virtual void clipRect(const Rectangle<float>& rect) = 0;
		virtual void clipRectClear() = 0;

		virtual void update() = 0;

		virtual void setViewport(const Rectangle<int>& viewport) = 0;
		virtual void setOrthoProjection(const Rectangle<float>& orthoBounds) = 0;

	protected:
		Renderer(const std::string& appTitle);
	};

} // namespace
