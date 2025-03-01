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


namespace NAS2D
{
	class Angle;


	class RendererNull : public Renderer
	{
	public:
		RendererNull() = default;

		~RendererNull() override;

		void drawImage(const Image&, Point<float>, float = 1.0, Color = Color::Normal) override;

		void drawSubImage(const Image&, Point<float>, const Rectangle<float>&, Color = Color::Normal) override;
		void drawSubImageRotated(const Image&, Point<float>, const Rectangle<float>&, Angle, Color = Color::Normal) override;

		void drawImageRotated(const Image&, Point<float>, Angle, Color = Color::Normal, float = 1.0f) override;
		void drawImageStretched(const Image&, const Rectangle<float>&, Color = Color::Normal) override;

		void drawImageRepeated(const Image&, const Rectangle<float>&) override;
		void drawSubImageRepeated(const Image&, const Rectangle<float>&, const Rectangle<float>&) override;

		void drawImageToImage(const Image&, const Image&, Point<float>) override;

		void drawPoint(Point<float>, Color = Color::White) override;
		void drawLine(Point<float>, Point<float>, Color = Color::White, int = 1) override;
		void drawBox(const Rectangle<float>&, Color = Color::White) override;
		void drawBoxFilled(const Rectangle<float>&, Color = Color::White) override;
		void drawCircle(Point<float>, float, Color, int = 10, Vector<float> = Vector{1.0f, 1.0f}) override;

		void drawGradient(const Rectangle<float>&, Color, Color, Color, Color) override;

		void drawText(const Font&, std::string_view, Point<float>, Color = Color::White) override;

		void clearScreen(Color = Color::Black) override;

		void clipRect(const Rectangle<float>&) override;
		void clipRectClear() override;

		void update() override;

		void setViewport(const Rectangle<int>&) override;
		void setOrthoProjection(const Rectangle<float>&) override;
	};

} // namespace NAS2D
