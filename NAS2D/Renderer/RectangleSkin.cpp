// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================


#include "RectangleSkin.h"
#include "Renderer.h"
#include "../Math/Rectangle.h"
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


Vector<int> RectangleSkin::minSize() const
{
	return mTopLeft.size() + mBottomRight.size();
}


void RectangleSkin::draw(Renderer& renderer, const Rectangle<int>& rect) const
{
	// Partial order comparison - must hold for both components simultaneously
	if (!(minSize() <= rect.size)) { return; }

	const auto p0 = rect.position;
	const auto p1 = rect.position + mTopLeft.size();
	const auto p2 = rect.crossXPoint() + mTopRight.size().reflectX();
	const auto p3 = rect.crossYPoint() + mBottomLeft.size().reflectY();
	const auto p4 = rect.endPoint() - mBottomRight.size();

	// Draw the center area
	renderer.drawImageRepeated(mCenter, Rectangle<int>::Create(p1, p4));

	// Draw the sides
	renderer.drawImageRepeated(mTop, Rectangle<int>::Create({p1.x, p0.y}, p2));
	renderer.drawImageRepeated(mBottom, Rectangle<int>::Create(p3, Point{p4.x, rect.endPoint().y}));
	renderer.drawImageRepeated(mLeft, Rectangle<int>::Create({p0.x, p1.y}, p3));
	renderer.drawImageRepeated(mRight, Rectangle<int>::Create(p2, Point{rect.endPoint().x, p4.y}));

	// Draw the corners
	renderer.drawImage(mTopLeft, rect.position);
	renderer.drawImage(mTopRight, Point{p2.x, p0.y});
	renderer.drawImage(mBottomLeft, Point{p0.x, p3.y});
	renderer.drawImage(mBottomRight, p4);
}
