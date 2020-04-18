// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
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

/**
 * Determines if a given line intersects a given circle.
 *
 * \param	p	First point of a line segment.
 * \param	q	Second point of a line segment.
 * \param	c	Center point of a circle.
 * \param	r	Radius of a circle.
 */
bool lineIntersectsCircle(const Point<int>& p, const Point<int>& q, const Point<int>& c, float r)
{
	const auto centerToStart = p - c;
	float dx = static_cast<float>(q.x() - p.x());
	float dy = static_cast<float>(q.y() - p.y());

	float t = -(((centerToStart.x) * dx) + ((centerToStart.y) * dy)) / ((dx * dx) + (dy * dy));

	t = std::clamp(t, 0.0f, 1.0f);

	dx = centerToStart.x + (t * (q.x() - p.x()));
	dy = centerToStart.y + (t * (q.y() - p.y()));
	float rt = (dx * dx) + (dy * dy);

	if (rt < (r * r))
	{
		return true;
	}

	return false;
}


/**
 * Basic integer division that rounds up to the nearest whole number.
 *
 * \param	to_divide	Number to be divided.
 * \param	divisor		Divisor.
 *
 * \return	Returns the divided number rounded up to the nearest whole number.
 */
int divideUp(int to_divide, int divisor)
{
	if (divisor == 0) {
		throw std::domain_error("Division by zero: divideUp(to_divide, 0)");
	}
	return (to_divide + (divisor - 1)) / divisor;
}

/**
 * Rounds a number up to a power of 2
 *
 * Domain: 1 .. 2^31
 * Values outside the domain may map to 0 (which is not a power of 2)
 * Note: 0 is outside the domain
 */
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
