// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Trig.h"
#include "NAS2D/Renderer/Primitives.h"

#include <math.h>

using namespace NAS2D;

// Common Trig Defines
const float NAS2D::PI = 3.14159265f;
const float NAS2D::PI_2 = NAS2D::PI * 2;

const float NAS2D::DEG2RAD = NAS2D::PI / 180.0f;
const float NAS2D::RAD2DEG = 180 / NAS2D::PI;


/**
 * Gets an angle in radians from degrees.
 */
float NAS2D::degToRad(float degree)
{
	return degree * NAS2D::DEG2RAD;
}


/**
 * Gets an angle in degrees from radians.
 */
float NAS2D::radToDeg(float rad)
{
	return rad * -NAS2D::RAD2DEG;
}


/**
 * Gets the angle of a line in degrees given two points.
 */
float NAS2D::angleFromPoints(float x, float y, float x2, float y2)
{
#if defined(_DEBUG)
	float angle = 90 - NAS2D::radToDeg(atan2(y2 - y, x2 - x));

	// Corrects for an odd anomaly whenever the angle
	// is between 270.0f and 260.0f.
	if(angle < 0.0f)
		angle = 360.0f + angle;

	return angle;
#else
	return 90 - radToDeg(atan2(y2 - y, x2 - x));
#endif
}


/**
 * Gets a directional vector from an angle in degrees.
 */
Point_2df NAS2D::getDirectionVector(float angle)
{
	return Point_2df(sin(NAS2D::degToRad(angle)), -cos(NAS2D::degToRad(angle)));
}


/**
 * Determines if a given line intersects a given circle.
 * 
 * \param	p	First point of a line segment.
 * \param	q	Second point of a line segment.
 * \param	c	Center point of a circle.
 * \param	r	Radius of a circle.
 */
bool NAS2D::lineIntersectsCircle(const Point_2d& p, const Point_2d& q, const Point_2d& c, float r)
{
	float dx = static_cast<float>(q.x() - p.x());
	float dy = static_cast<float>(q.y() - p.y());

	float t = -(((p.x() - c.x()) * dx) + ((p.y() - c.y()) * dy)) / ((dx * dx) + (dy * dy));

	if(t < 0.0)
		t = 0.0;
	else if(t > 1.0)
		t = 1.0;

	dx = (p.x() + (t * (q.x() - p.x()))) - c.x();
	dy = (p.y() + (t * (q.y() - p.y()))) - c.y();
	float rt = (dx * dx) + (dy * dy);

	if(rt < (r * r))
		return true;

	return false;
}
