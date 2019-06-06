#include "NAS2D/Math/Vector3.h"

#include "NAS2D/Math/Vector2.h"
#include "NAS2D/MathUtils.h"

#include <cmath>

namespace NAS2D
{
	//Ignore VS2019 26426 i.22 false-positives
	//These have known initialization and it doesn't matter what order.
	const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::X_Axis(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::Y_Axis(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::Z_Axis(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
	const Vector3 Vector3::XY_Axis(1.0f, 1.0f, 0.0f);
	const Vector3 Vector3::YX_Axis(1.0f, 1.0f, 0.0f);
	const Vector3 Vector3::XZ_Axis(1.0f, 0.0f, 1.0f);
	const Vector3 Vector3::YZ_Axis(0.0f, 1.0f, 1.0f);
	const Vector3 Vector3::ZX_Axis(1.0f, 0.0f, 1.0f);
	const Vector3 Vector3::ZY_Axis(0.0f, 1.0f, 1.0f);
	const Vector3 Vector3::XYZ_Axis(1.0f, 1.0f, 1.0f);

	Vector3::Vector3(float x, float y, float z) noexcept
	: x(x)
	, y(y)
	, z(z)
	{
		/* DO NOTHING */
	}

	Vector3::Vector3(const Vector2& xy, float z) noexcept
	: x(xy.x)
	, y(xy.y)
	, z(z)
	{
	}

	Vector3 Vector3::operator+(const Vector3& rhs) const noexcept
	{
		return {x + rhs.x, y + rhs.y, z + rhs.z};
	}

	Vector3& Vector3::operator+=(const Vector3& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3 Vector3::operator-() const noexcept
	{
		return {-x, -y, -z};
	}

	Vector3 Vector3::operator-(const Vector3& rhs) const noexcept
	{
		return {x - rhs.x, y - rhs.y, z - rhs.z};
	}

	Vector3& Vector3::operator-=(const Vector3& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3 operator*(float lhs_scalar, const Vector3& rhs) noexcept
	{
		//Same as rhs * lhs, but I'm striving for comprehension instead of terseness
		return {lhs_scalar * rhs.x, lhs_scalar * rhs.y, lhs_scalar * rhs.z};
	}

	Vector3 Vector3::operator*(float rhs_scalar) const noexcept
	{
		return {x * rhs_scalar, y * rhs_scalar, z * rhs_scalar};
	}

	Vector3& Vector3::operator*=(float rhs_scalar) noexcept
	{
		x *= rhs_scalar;
		y *= rhs_scalar;
		z *= rhs_scalar;
		return *this;
	}

	Vector3 Vector3::operator*(const Vector3& rhs) const noexcept
	{
		return {x * rhs.x, y * rhs.y, z * rhs.z};
	}

	Vector3& Vector3::operator*=(const Vector3& rhs) noexcept
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	Vector3 Vector3::operator/(float scalar) const noexcept
	{
		return {x / scalar, y / scalar, z / scalar};
	}

	Vector3& Vector3::operator/=(float scalar) noexcept
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	Vector3 Vector3::operator/(const Vector3& rhs) const noexcept
	{
		return {x / rhs.x, y / rhs.y, z / rhs.z};
	}

	Vector3& Vector3::operator/=(const Vector3& rhs) noexcept
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	bool Vector3::operator==(const Vector3& rhs) const noexcept
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool Vector3::operator!=(const Vector3& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	void Vector3::getXYZ(float& outX, float& outY, float& outZ) const noexcept
	{
		outX = x;
		outY = y;
		outZ = z;
	}

	float* Vector3::getAsFloatArray() noexcept
	{
		return &x;
	}

	float Vector3::calcLength() const noexcept
	{
		return std::sqrt(calcLengthSquared());
	}

	float Vector3::calcLengthSquared() const noexcept
	{
		return x * x + y * y + z * z;
	}

	void Vector3::setXYZ(float newX, float newY, float newZ) noexcept
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	float Vector3::normalize() noexcept
	{
		const float length = calcLength();
		if (length > 0.0f)
		{
			const float inv_length = 1.0f / length;
			x *= inv_length;
			y *= inv_length;
			z *= inv_length;
			return length;
		}
		return 0.0f;
	}

	Vector3 Vector3::getNormalize() const noexcept
	{
		const float inv_length = calcInverseLength();
    	return {x * inv_length, y * inv_length, z * inv_length};
	}

	float Vector3::calcInverseLength() const noexcept
	{
		const auto length = calcLength();
        if(length > 0.0f) {
			return 1.0f / length;
        }
		return 0.0f;
	}

	void swap(Vector3& a, Vector3& b) noexcept
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.z, b.z);
	}

} // namespace NAS2D