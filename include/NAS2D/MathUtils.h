// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "Renderer/Point.h"
#include "Renderer/Rectangle.h"
#include "Renderer/Vector.h"

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace NAS2D
{

	// Utility Functions
	bool isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH);
	bool isPointInRect(const Point_2d& point, const Rectangle_2d& rect);
	bool isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2);
	bool isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b);

	int divideUp(int to_divide, int divisor);

	

/**
 * \fn OutputType scaleLinear(const InputType& a, const OutputType& b)
 *
 * Applies a linear conversion to map an input domain to an output range.
 *
 * \param	value: The value to be converted from the input domain to the output range.
 * \param	domainPoint1: A known lower fixed point in the domain. Must be less than domainPoint2.
 * \param	domainPoint2: A known upper fixed point in the domain. Must be greater than domainPoint1.
 * \param	rangePoint1: A known lower fixed point in the range. Must be less than or equal to rangePoint2.
 * \param	rangePoint2: A known upper fixed point in the range. Must be greater than or equal to rangePoint1.
 * \return	Returns value as mapped to the output range.
 * \remarks
 * `value` is not required to start within [domainPoint1,domainPoint2]. E.g. temperature conversions:
 *
 *     scaleLinear(0.0f, 32.0f, 212.0f, 0.0f, 100.0f)
 *
 * will convert 0 degees Fahrenheit to its corresponding value `-17.77777` degrees Celsius.
 * 
 * Avoid inputs such that the difference of domainPoint2 and domainPoint1 is zero. Otherwise, a divide by zero error could occur.
 * 
 * If the difference of rangePoint2 and rangePoint1 is zero. The result of the function is always rangePoint1.
 * 
 * InputType and OutputType must support all arithmetic operators and all arithmetic operators between each other.
 */
template<typename InputType, typename OutputType>
OutputType scaleLinear(const InputType& value, const InputType& domainPoint1, const InputType& domainPoint2, const OutputType& rangePoint1, const OutputType& rangePoint2)
{
	return (value - domainPoint1) * (rangePoint2 - rangePoint1) / (domainPoint2 - domainPoint1) + rangePoint1;
}

} // namespace NAS2D

