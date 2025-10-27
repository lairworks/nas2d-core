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

#include "Point.h"

#include <algorithm>
#include <stdexcept>


namespace NAS2D
{

	/**
	 * Determines if a given line intersects a given circle.
	 *
	 * \param	p	First point of a line segment.
	 * \param	q	Second point of a line segment.
	 * \param	c	Center point of a circle.
	 * \param	r	Radius of a circle.
	 */
	bool lineIntersectsCircle(Point<int> p, Point<int> q, Point<int> c, float r)
	{
		const auto centerToStart = (p - c).to<float>();
		const auto lineSize = (q - p).to<float>();

		const auto t = std::clamp<float>(-centerToStart.dotProduct(lineSize) / lineSize.lengthSquared(), 0, 1);
		const auto minDistance = lineSize * t + centerToStart;

		return minDistance.lengthSquared() < (r * r);
	}


	/**
	 * Basic integer division that rounds up to the nearest whole number.
	 *
	 * \param	dividend	Number to be divided.
	 * \param	divisor		Divisor.
	 *
	 * \return	Returns the divided number rounded up to the nearest whole number.
	 */
	int divideUp(int dividend, int divisor)
	{
		if (divisor == 0)
		{
			throw std::domain_error("Division by zero: divideUp(dividend, 0)");
		}
		return (dividend + (divisor - 1)) / divisor;
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

}
