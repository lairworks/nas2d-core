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

#include "Renderer/Point.h"

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace NAS2D
{
	bool lineIntersectsCircle(Point<int> p, Point<int> q, Point<int> c, float r);

	int divideUp(int to_divide, int divisor);

	uint32_t roundUpPowerOf2(uint32_t number);

template<typename InputType, typename OutputType>
OutputType scaleLinear(const InputType& value, const InputType& domainPoint1, const InputType& domainPoint2, const OutputType& rangePoint1, const OutputType& rangePoint2)
{
	return (value - domainPoint1) * (rangePoint2 - rangePoint1) / (domainPoint2 - domainPoint1) + rangePoint1;
}

} // namespace NAS2D

