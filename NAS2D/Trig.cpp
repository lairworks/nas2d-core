// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Trig.h"

#include <cmath>
#include <algorithm>

namespace NAS2D {

// Common Trig Defines
const float PI = 3.14159265f;
const float PI_2 = PI * 2;

const float DEG2RAD = PI / 180.0f;
const float RAD2DEG = 180 / PI;


/**
 * Gets an angle in radians from degrees.
 */
float degToRad(float degree)
{
	return degree * DEG2RAD;
}


/**
 * Gets an angle in degrees from radians.
 */
float radToDeg(float rad)
{
	return rad * -RAD2DEG;
}


/**
 * Gets the angle of a line in degrees given two points.
 */
float angleFromPoints(float x, float y, float x2, float y2)
{
	return 90.0f - radToDeg(std::atan2(y2 - y, x2 - x));
}


/**
 * Gets a directional vector from an angle in degrees.
 *
 * Assumes screen coordinates (origin is top left).
 * Angle of 0 corresponds to up, and increases clockwise.
 */
Vector<float> getDirectionVector(float angle)
{
	return {std::sin(degToRad(angle)), -std::cos(degToRad(angle))};
}


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
	float dx = static_cast<float>(q.x() - p.x());
	float dy = static_cast<float>(q.y() - p.y());

	float t = -(((p.x() - c.x()) * dx) + ((p.y() - c.y()) * dy)) / ((dx * dx) + (dy * dy));

	t = std::clamp(t, 0.0f, 1.0f);

	dx = (p.x() + (t * (q.x() - p.x()))) - c.x();
	dy = (p.y() + (t * (q.y() - p.y()))) - c.y();
	float rt = (dx * dx) + (dy * dy);

	if (rt < (r * r))
	{
		return true;
	}

	return false;
}

}
