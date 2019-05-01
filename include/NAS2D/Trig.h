// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

namespace NAS2D {

extern const float PI;
extern const float PI_2;
extern const float DEG2RAD;
extern const float RAD2DEG;


class Point_2d;
class Point_2df;


float degToRad(float degree);
float radToDeg(float rad);
float angleFromPoints(float x, float y, float x2, float y2);
Point_2df getDirectionVector(float angle);

bool lineIntersectsCircle(const Point_2d& p, const Point_2d& q, const Point_2d& c, float r);

};
