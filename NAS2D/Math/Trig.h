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


	Angle getAngle(Vector<float> direction);
	Vector<float> getDirectionVector(Angle angle);
}
