#include "RendererNull.h"

#include "../Math/Point.h"
#include "../Math/Vector.h"
#include "../Math/Angle.h"


using namespace NAS2D;


RendererNull::~RendererNull()
{
}


void RendererNull::drawImage(const Image&, Point<float>, float, Color)
{
}


void RendererNull::drawSubImage(const Image&, Point<float>, const Rectangle<float>&, Color)
{
}


void RendererNull::drawSubImageRotated(const Image&, Point<float>, const Rectangle<float>&, Angle, Color)
{
}


void RendererNull::drawImageRotated(const Image&, Point<float>, Angle, Color, float)
{
}


void RendererNull::drawImageStretched(const Image&, const Rectangle<float>&, Color)
{
}


void RendererNull::drawImageRepeated(const Image&, const Rectangle<float>&)
{
}


void RendererNull::drawSubImageRepeated(const Image&, const Rectangle<float>&, const Rectangle<float>&)
{
}


void RendererNull::drawImageToImage(const Image&, const Image&, Point<float>)
{
}


void RendererNull::drawPoint(Point<float>, Color)
{
}


void RendererNull::drawLine(Point<float>, Point<float>, Color, int)
{
}


void RendererNull::drawBox(const Rectangle<float>&, Color)
{
}


void RendererNull::drawBoxFilled(const Rectangle<float>&, Color)
{
}


void RendererNull::drawCircle(Point<float>, float, Color, int, Vector<float>)
{
}


void RendererNull::drawGradient(const Rectangle<float>&, Color, Color, Color, Color)
{
}


void RendererNull::drawText(const Font&, std::string_view, Point<float>, Color)
{
}


void RendererNull::clearScreen(Color)
{
}


void RendererNull::clipRect(const Rectangle<float>&)
{
}


void RendererNull::clipRectClear()
{
}


void RendererNull::update()
{
}


void RendererNull::setViewport(const Rectangle<int>&)
{
}


void RendererNull::setOrthoProjection(const Rectangle<float>&)
{
}
