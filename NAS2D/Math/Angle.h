#pragma once

#include <numbers>


namespace NAS2D
{
	class Angle
	{
	protected:
		// Non-public constructor forces the use of static factory methods
		constexpr Angle(float radians) : radianMeasure{radians} {}

		static constexpr float DegreeToRadians = std::numbers::pi_v<float> / 180;
		static constexpr float RadiansToDegrees = 180 / std::numbers::pi_v<float>;

		static constexpr float degToRad(float degrees) { return degrees * DegreeToRadians; }
		static constexpr float radToDeg(float radians) { return radians * RadiansToDegrees; }

	public:
		// Static factory methods, with an explicit angle measure
		static constexpr Angle degrees(float degrees) { return Angle{degToRad(degrees)}; }
		static constexpr Angle radians(float radians) { return Angle{radians}; }

		constexpr float degrees() const { return radToDeg(radianMeasure); }
		constexpr float radians() const { return radianMeasure; }

	private:
		float radianMeasure;
	};
}
