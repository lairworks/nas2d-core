// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "MathUtils.h"
#include <algorithm>
#include <stdexcept>


namespace NAS2D {

bool lineIntersectsCircle(Point<int> p, Point<int> q, Point<int> c, float r)
{
	const auto centerToStart = (p - c).to<float>();
	const auto lineSize = (q - p).to<float>();

	const auto t = std::clamp<float>(-centerToStart.dotProduct(lineSize) / lineSize.lengthSquared(), 0, 1);
	const auto minDistance = lineSize * t + centerToStart;

	return minDistance.lengthSquared() < (r * r);
}

int divideUp(int to_divide, int divisor)
{
	if (divisor == 0) {
		throw std::domain_error("Division by zero: divideUp(to_divide, 0)");
	}
	return (to_divide + (divisor - 1)) / divisor;
}

uint32_t roundUpPowerOf2(uint32_t number)
{
	--number;
	number |= number >> 1;
	number |= number >> 2;
	number |= number >> 4;
	number |= number >> 8;
	number |= number >> 16;
	return ++number;
}

} // namespace
