// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Renderer.h"
#include "../Math/Rectangle.h"

#include <algorithm>


using namespace NAS2D;


Renderer::Renderer(const std::string& appTitle) :
	Window(appTitle)
{
}


Renderer::~Renderer()
{
}


void Renderer::drawTextShadow(const Font& font, std::string_view text, Point<float> position, Vector<float> shadowOffset, Color textColor, Color shadowColor)
{
	const auto shadowPosition = position + shadowOffset;
	drawText(font, text, shadowPosition, shadowColor);
	drawText(font, text, position, textColor);
}


Point<int> Renderer::center() const
{
	return Point{0, 0} + mResolution / 2;
}
