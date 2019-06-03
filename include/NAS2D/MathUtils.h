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
	namespace Constants
	{
		constexpr float M_PI = 3.14159265358979323846f;		  // pi
		constexpr float M_2PI = 6.28318530717958647692f;	  // 2pi
		constexpr float M_PI_2 = 1.57079632679489661923f;	 // pi/2
		constexpr float M_PI_4 = 0.78539816339744830962f;	 // pi/4
		constexpr float M_1_PI = 0.31830988618379067151f;	 // 1/pi
		constexpr float M_2_PI = 0.63661977236758134308f;	 // 2/pi
		constexpr float M_2_SQRTPI = 1.12837916709551257390f; // 2/sqrt(pi)
	}

    // Utility Functions
	bool isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH);
	bool isPointInRect(const Point_2d& point, const Rectangle_2d& rect);
	bool isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2);
	bool isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b);

	int divideUp(int to_divide, int divisor);
	constexpr float convertDegreesToRadians(float degrees) noexcept;
	constexpr float convertRadiansToDegrees(float radians) noexcept;

} // namespace NAS2D

