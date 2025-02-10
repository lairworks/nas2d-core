// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "Vector.h"


namespace NAS2D
{
	class Angle;


	constexpr float PI = 3.14159265f;
	constexpr float PI_2 = PI * 2;

	constexpr float DEG2RAD = PI / 180;
	constexpr float RAD2DEG = 180 / PI;


	float degToRad(float degree);
	float radToDeg(float rad);

	Angle getAngle(Vector<float> direction);
	Vector<float> getDirectionVector(Angle angle);
}
