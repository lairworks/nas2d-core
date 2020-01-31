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
 * \fn T convertDomainToRange(T a, T b)
 *
 * Applies a linear conversion to map an input domain to an output range for built-in types.
 *
 * \param	value: The value to be converted from the input domain to the output range.
 * \param	minDomain: Smallest value of the domain.
 * \param	maxDomain: Largest value of the domain.
 * \param	minRange: Smallest value of the range.
 * \param	maxRange: Largest value of the range.
 *
 * \return	Returns the mapped value in the closed range [minOutputRange,maxOutputRange]
 */
template<typename InputType, typename OutputType>
OutputType convertDomainToRange(const InputType& value, const InputType& minDomain, const InputType& maxDomain, const OutputType& minRange, const OutputType& maxRange)
{
	return (value - minDomain) * (maxRange - minRange) / (maxDomain - minDomain) + minRange;
}

} // namespace NAS2D

