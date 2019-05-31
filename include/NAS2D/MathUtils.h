// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "NAS2D/Renderer/Primitives.h"

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace NAS2D
{

	// Utility Functions
	bool isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH);
	bool isPointInRect(const Point_2d& point, const Rectangle_2d& rect);
	bool isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2);
	bool isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b);

	int divideUp(int to_divide, int divisor);

} // namespace NAS2D

