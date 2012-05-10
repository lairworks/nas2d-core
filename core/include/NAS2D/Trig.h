// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __NAS_TRIG__
#define __NAS_TRIG__

extern const float PI;
extern const float PI_2;
extern const float DEG2RAD;
extern const float RAD2DEG;

/**
 * \struct Vec2f
 * \brief	Contains two values.
 *
 * Built with directional vectors in mind.
 */
struct Vec2f
{
	Vec2f(): x(0.0f), y(0.0f) {}
	Vec2f(float x, float y): x(x), y(y) {}

	float x, y;
};

class Point_2d;

float degToRad(float degree);
float radToDeg(float rad);
float angleFromPoints(float x, float y, float x2, float y2);
Vec2f getDirectionVector(float angle);

bool lineIntersectsCircle(const Point_2d& p, const Point_2d& q, const Point_2d& c, float r);

#endif