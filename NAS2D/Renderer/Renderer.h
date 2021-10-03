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
#include "DisplayDesc.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"
#include "../Timer.h"
#include "../Signal/Signal.h"

#include <chrono>
#include <string_view>
#include <string>
#include <vector>


namespace NAS2D
{

	class Font;
	class Image;

	template <typename BaseType>
	struct Rectangle;


	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const Renderer& rhs) = default;
		Renderer& operator=(const Renderer& rhs) = default;
		Renderer(Renderer&& rhs) = default;
		Renderer& operator=(Renderer&& rhs) = default;
		virtual ~Renderer();

		const std::string& driverName() const;
		virtual std::vector<DisplayDesc> getDisplayModes() const = 0;
		virtual DisplayDesc getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const = 0;

		const std::string& title() const;
		void title(const std::string& title);

		virtual void window_icon(const std::string& path) = 0;

		virtual void showSystemPointer(bool) = 0;
		virtual void addCursor(const std::string& filePath, int cursorId, int offx, int offy) = 0;
		virtual void setCursor(int cursorId) = 0;

		virtual void fullscreen(bool fs, bool maintain = false) = 0;
		virtual bool fullscreen() const = 0;

		virtual void resizeable(bool _r) = 0;
		virtual bool resizeable() const = 0;

		virtual void minimumSize(Vector<int> newSize) = 0;

		virtual Vector<int> size() const = 0;
		virtual void size(Vector<int> newSize) = 0;
		void setResolution(Vector<int> newResolution);

		virtual Vector<int> getWindowClientArea() const noexcept = 0;

		virtual void drawImage(const Image& image, Point<float> position, float scale = 1.0, Color color = Color::Normal) = 0;
		virtual void drawSubImage(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, Color color = Color::Normal) = 0;
		virtual void drawSubImageRotated(const Image& image, Point<float> raster, const Rectangle<float>& subImageRect, float degrees, Color color = Color::Normal) = 0;
		virtual void drawImageRotated(const Image& image, Point<float> position, float degrees, Color color = Color::Normal, float scale = 1.0f) = 0;
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

		void fadeColor(Color color);
		void fadeIn(std::chrono::milliseconds fadeInTime);
		void fadeOut(std::chrono::milliseconds fadeOutTime);
		bool isFading() const;
		bool isFaded() const;
		SignalSource<>& fadeComplete();

		virtual void clearScreen(Color color = Color::Black) = 0;

		Point<int> center() const;

		virtual void clipRect(const Rectangle<float>& rect) = 0;
		virtual void clipRectClear() = 0;

		virtual void update();

		virtual void setViewport(const Rectangle<int>& viewport) = 0;
		virtual void setOrthoProjection(const Rectangle<float>& orthoBounds) = 0;

	protected:
		Renderer(const std::string& appTitle);

		void driverName(const std::string& name);

		Vector<int> mResolution{1600, 900};

	private:
		enum class FadeType
		{
			In = -1,
			None = 0,
			Out = 1
		};

		std::string mDriverName{"NULL Renderer"};
		std::string mTitle{"Default Application"};

		Color mFadeColor{Color::Black};
		float mFadeStep{0.0f};
		float mCurrentFade{0.0f};

		FadeType mCurrentFadeType{FadeType::None};
		Timer fadeTimer;
		Signal<> fadeCompleteSignal;
	};

} // namespace
