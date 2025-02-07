#pragma once

#include <numbers>


namespace NAS2D
{
	class Angle
	{
	protected:
		// Non-public constructor forces the use of static factory methods
		constexpr Angle(float degrees) : degreeMeasure{degrees} {}

		static constexpr float DegreeToRadians = std::numbers::pi_v<float> / 180;
		static constexpr float RadiansToDegrees = 180 / std::numbers::pi_v<float>;

		static constexpr float degToRad(float degrees) { return degrees * DegreeToRadians; }
		static constexpr float radToDeg(float radians) { return radians * RadiansToDegrees; }

	public:
		// Static factory methods, with an explicit angle measure
		static constexpr Angle degrees(float degrees) { return Angle{degrees}; }
		static constexpr Angle radians(float radians) { return Angle{radToDeg(radians)}; }

		constexpr Angle operator-() const { return Angle{-degreeMeasure}; }

		constexpr Angle operator+(const Angle& other) const { return Angle{degreeMeasure + other.degreeMeasure}; }
		constexpr Angle operator-(const Angle& other) const { return Angle{degreeMeasure - other.degreeMeasure}; }

		constexpr float degrees() const { return degreeMeasure; }
		constexpr float radians() const { return degToRad(degreeMeasure); }

	private:
		float degreeMeasure;
	};
}
