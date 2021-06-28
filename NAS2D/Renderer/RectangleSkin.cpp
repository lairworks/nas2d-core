// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "RectangleSkin.h"
#include "Renderer.h"
#include "Rectangle.h"
#include "../Resource/Image.h"


using namespace NAS2D;


RectangleSkin::RectangleSkin(const Image& topLeft, const Image& top, const Image& topRight, const Image& left, const Image& center, const Image& right, const Image& bottomLeft, const Image& bottom, const Image& bottomRight) :
	mTopLeft{topLeft},
	mTop{top},
	mTopRight{topRight},
	mLeft{left},
	mCenter{center},
	mRight{right},
	mBottomLeft{bottomLeft},
	mBottom{bottom},
	mBottomRight{bottomRight}
{}


void RectangleSkin::draw(Renderer& renderer, const Rectangle<float>& rect) const
{
	const auto p1 = rect.startPoint() + mTopLeft.size().to<float>();
	const auto p2 = rect.crossXPoint() + mTopRight.size().reflectX().to<float>();
	const auto p3 = rect.crossYPoint() + mBottomLeft.size().reflectY().to<float>();
	const auto p4 = rect.endPoint() - mBottomRight.size().to<float>();

	renderer.drawImageRepeated(mCenter, Rectangle<float>::Create(p1, p4));

	renderer.drawImageRepeated(mTop, Rectangle<float>::Create({p1.x, rect.y}, p2));
	renderer.drawImageRepeated(mBottom, Rectangle<float>::Create(p3, Point{p4.x, rect.endPoint().y}));
	renderer.drawImageRepeated(mLeft, Rectangle<float>::Create({rect.x, p1.y}, p3));
	renderer.drawImageRepeated(mRight, Rectangle<float>::Create(p2, Point{rect.endPoint().x, p4.y}));

	renderer.drawImage(mTopLeft, rect.startPoint());
	renderer.drawImage(mTopRight, {p2.x, rect.y});
	renderer.drawImage(mBottomLeft, {rect.x, p3.y});
	renderer.drawImage(mBottomRight, p4);
}
