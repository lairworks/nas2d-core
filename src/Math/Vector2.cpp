#include "NAS2D/Math/Vector2.h"

#include "NAS2D/MathUtils.h"

#include <cmath>

namespace NAS2D
{
	//Ignore VS2019 26426 i.22 false-positives
	//These have known initialization and it doesn't matter what order.
	const Vector2 Vector2::Zero(0.0f, 0.0f);
	const Vector2 Vector2::X_Axis(1.0f, 0.0f);
	const Vector2 Vector2::Y_Axis(0.0f, 1.0f);
	const Vector2 Vector2::One(1.0f, 1.0f);
	const Vector2 Vector2::XY_Axis(1.0f, 1.0f);
	const Vector2 Vector2::YX_Axis(1.0f, 1.0f);

	Vector2::Vector2(float x, float y) noexcept
	: x(x)
	, y(y)
	{
		/* DO NOTHING */
	}

	Vector2 Vector2::operator+(const Vector2& rhs) const noexcept
	{
		return {x + rhs.x, y + rhs.y};
	}

	Vector2& Vector2::operator+=(const Vector2& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2 Vector2::operator-() const noexcept
	{
		return {-x, -y};
	}

	Vector2 Vector2::operator-(const Vector2& rhs) const noexcept
	{
		return {x - rhs.x, y - rhs.y};
	}

	Vector2& Vector2::operator-=(const Vector2& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2 operator*(float lhs_scalar, const Vector2& rhs) noexcept
	{
		//Same as rhs * lhs, but I'm striving for comprehension instead of terseness
		return {lhs_scalar * rhs.x, lhs_scalar * rhs.y};
	}

	Vector2 Vector2::operator*(float rhs_scalar) const noexcept
	{
		return {x * rhs_scalar, y * rhs_scalar};
	}

	Vector2& Vector2::operator*=(float rhs_scalar) noexcept
	{
		x *= rhs_scalar;
		y *= rhs_scalar;
		return *this;
	}

	Vector2 Vector2::operator*(const Vector2& rhs) const noexcept
	{
		return {x * rhs.x, y * rhs.y};
	}

	Vector2& Vector2::operator*=(const Vector2& rhs) noexcept
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	Vector2 Vector2::operator/(float scalar) const noexcept
	{
		return {x / scalar, y / scalar};
	}

	Vector2& Vector2::operator/=(float scalar) noexcept
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2 Vector2::operator/(const Vector2& rhs) const noexcept
	{
		return {x / rhs.x, y / rhs.y};
	}

	Vector2& Vector2::operator/=(const Vector2& rhs) noexcept
	{
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	bool Vector2::operator==(const Vector2& rhs) const noexcept
	{
		return x == rhs.x && y == rhs.y;
	}

	bool Vector2::operator!=(const Vector2& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	void Vector2::getXY(float& outX, float& outY) const noexcept
	{
		outX = x;
		outY = y;
	}

	float* Vector2::getAsFloatArray() noexcept
	{
		return &x;
	}

	float Vector2::calcHeadingRadians() const noexcept
	{
		return std::atan2(y, x);
	}

	float Vector2::calcHeadingDegrees() const noexcept
	{
		return convertRadiansToDegrees(calcHeadingRadians());
	}

	float Vector2::calcLength() const noexcept
	{
		return std::sqrt(calcLengthSquared());
	}

	float Vector2::calcLengthSquared() const noexcept
	{
		return x * x + y * y;
	}

	void Vector2::setXY(float newX, float newY) noexcept
	{
		x = newX;
		y = newY;
	}

	void Vector2::setHeadingRadians(float headingRadians) noexcept
	{
		const float R = calcLength();
		x = R * std::cos(headingRadians);
		y = R * std::sin(headingRadians);
	}

	void Vector2::setHeadingDegrees(float headingDegrees) noexcept
	{
		setHeadingRadians(convertDegreesToRadians(headingDegrees));
	}

	void Vector2::setUnitLengthAndHeadingRadians(float headingRadians) noexcept
	{
		x = std::cos(headingRadians);
		y = std::sin(headingRadians);
	}

	void Vector2::setUnitLengthAndHeadingDegrees(float headingDegrees) noexcept
	{
		setUnitLengthAndHeadingRadians(convertDegreesToRadians(headingDegrees));
	}

	float Vector2::setLength(float length) noexcept
	{
		const float old_length = calcLength();
		const float theta = calcHeadingRadians();
		x = length * std::cos(theta);
		y = length * std::sin(theta);
		return old_length;
	}

	void Vector2::setLengthAndHeadingRadians(float headingRadians, float length) noexcept
	{
		setLength(length);
		setHeadingRadians(headingRadians);
	}

	void Vector2::setLengthAndHeadingDegrees(float headingDegrees, float length) noexcept
	{
		setLengthAndHeadingRadians(convertDegreesToRadians(headingDegrees), length);
	}

	float Vector2::normalize() noexcept
	{
		const float length = calcLength();
		if (length > 0.0f)
		{
			const float inv_length = 1.0f / length;
			x *= inv_length;
			y *= inv_length;
			return length;
		}
		return 0.0f;
	}

	Vector2 Vector2::getNormalize() const noexcept
	{
		const float inv_length = calcInverseLength();
		float newX = x * inv_length;
		float newY = y * inv_length;
		return {newX, newY};
	}

	void Vector2::rotate90Degrees() noexcept
	{
		setXY(-y, x);
	}

	void Vector2::rotateNegative90Degrees() noexcept
	{
		setXY(y, -x);
	}

	void Vector2::rotateRadians(float radiansToAdd) noexcept
	{
		const float R = calcLength();
		const float old_angle = std::atan2(y, x);
		const float new_angle = old_angle + radiansToAdd;
		x = R * std::cos(new_angle);
		y = R * std::sin(new_angle);
	}

	void Vector2::rotateDegrees(float degreesToAdd) noexcept
	{
		rotateRadians(convertDegreesToRadians(degreesToAdd));
	}

	float Vector2::calcInverseLength() const noexcept
	{
		const auto length = calcLength();
		if (length > 0.0f)
		{
			return 1.0f / length;
		}
		return 0.0f;
	}

	void swap(Vector2& a, Vector2& b) noexcept
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
	}

} // namespace NAS2D