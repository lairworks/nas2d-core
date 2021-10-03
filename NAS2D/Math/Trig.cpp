// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Trig.h"

#include <cmath>


namespace NAS2D
{

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
		return rad * RAD2DEG;
	}


	/**
	 * Gets the angle of a direction vector
	 */
	float getAngle(Vector<float> direction)
	{
		return std::atan2(direction.y, direction.x);
	}


	/**
	 * Gets a directional vector from an angle in radians.
	 */
	Vector<float> getDirectionVector(float radian)
	{
		return {std::cos(radian), std::sin(radian)};
	}

}
