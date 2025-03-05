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

#include <cstdint>


namespace NAS2D
{
	template <typename BaseType>
	struct Point;

	bool lineIntersectsCircle(Point<int> p, Point<int> q, Point<int> c, float r);

	int divideUp(int to_divide, int divisor);

	uint32_t roundUpPowerOf2(uint32_t number);

	/**
	 * Applies a linear conversion to map an input domain to an output range.
	 *
	 * \param	value	The value to be converted from the input domain to the output range.
	 * \param	domainPoint1	A known fixed point in the domain.
	 * \param	domainPoint2	Another corresponding known fixed point in the domain.
	 * \param	rangePoint1	A known fixed point in the range.
	 * \param	rangePoint2	Another corresponding known fixed point in the range.
	 * \return	Returns value as mapped to the output range.
	 * \remarks
	 * `value` is not required to start within [domainPoint1,domainPoint2]. E.g. temperature conversions:
	 *
	 *     scaleLinear(98.6f, 32.0f, 212.0f, 0.0f, 100.0f)
	 *
	 * will convert 98.6 degrees Fahrenheit (average human body temperature) to its corresponding value `37.0` degrees Celsius.
	 *
	 * Avoid inputs such that the difference of domainPoint2 and domainPoint1 is zero. Otherwise, a divide by zero error could occur.
	 *
	 * If the difference of rangePoint2 and rangePoint1 is zero. The result of the function is always rangePoint1.
	 *
	 * InputType and OutputType must support all arithmetic operators and all arithmetic operators between each other.
	 */
	template <typename InputType, typename OutputType>
	constexpr OutputType scaleLinear(const InputType& value, const InputType& domainPoint1, const InputType& domainPoint2, const OutputType& rangePoint1, const OutputType& rangePoint2)
	{
		return static_cast<OutputType>(((domainPoint2 - value) * rangePoint1 + (value - domainPoint1) * rangePoint2) / (domainPoint2 - domainPoint1));
	}

} // namespace NAS2D
