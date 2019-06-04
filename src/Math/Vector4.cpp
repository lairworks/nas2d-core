#include "NAS2D/Math/Vector4.h"

#include "NAS2D/Math/Vector2.h"
#include "NAS2D/Math/Vector3.h"
#include "NAS2D/MathUtils.h"

#include <cmath>

namespace NAS2D
{
	//Ignore VS2019 26426 i.22 false-positives
	//These have known initialization and it doesn't matter what order.
	const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::Zero_XYZ_One_W(0.0f, 0.0f, 0.0f, 1.0f);
	const Vector4 Vector4::One_XYZ_Zero_W(1.0f, 1.0f, 1.0f, 0.0f);
	const Vector4 Vector4::X_Axis(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::Y_Axis(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::Z_Axis(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::XY_Axis(1.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::YX_Axis(1.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::XZ_Axis(1.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::YZ_Axis(0.0f, 1.0f, 1.0f, 0.0f);
	const Vector4 Vector4::ZX_Axis(1.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::ZY_Axis(0.0f, 1.0f, 1.0f, 0.0f);
	const Vector4 Vector4::W_Axis(0.0f, 0.0f, 0.0f, 1.0f);
	const Vector4 Vector4::WX_Axis(1.0f, 0.0f, 0.0f, 1.0f);
	const Vector4 Vector4::WY_Axis(0.0f, 1.0f, 0.0f, 1.0f);
	const Vector4 Vector4::WZ_Axis(0.0f, 0.0f, 1.0f, 1.0f);
	const Vector4 Vector4::XYZ_Axis(1.0f, 1.0f, 1.0f, 0.0f);
	const Vector4 Vector4::YZW_Axis(0.0f, 1.0f, 1.0f, 1.0f);
	const Vector4 Vector4::XZW_Axis(1.0f, 0.0f, 1.0f, 1.0f);
	const Vector4 Vector4::XYW_Axis(1.0f, 1.0f, 0.0f, 1.0f);
	const Vector4 Vector4::XYZW_Axis(1.0f, 1.0f, 1.0f, 1.0f);
	const Vector4 Vector4::One(1.0f, 1.0f, 1.0f, 1.0f);

	Vector4::Vector4(float x, float y, float z, float w) noexcept
	: x(x)
	, y(y)
	, z(z)
	, w(w)
	{
		/* DO NOTHING */
	}

	Vector4::Vector4(const Vector2& xy, float z, float w) noexcept
	: x(xy.x)
	, y(xy.y)
	, z(z)
	, w(w)
	{
		/* DO NOTHING */
	}

	Vector4::Vector4(const Vector3& xyz, float w) noexcept
	: x(xyz.x)
	, y(xyz.y)
	, z(xyz.z)
	, w(w)
	{
		/* DO NOTHING */
	}

	Vector4::Vector4(const Vector2& xy, const Vector2& zw) noexcept
	: x(xy.x)
	, y(xy.y)
	, z(zw.x)
	, w(zw.y)
	{
		/* DO NOTHING */
	}

	Vector4 Vector4::operator+(const Vector4& rhs) const noexcept
	{
		return {x + rhs.x, y + rhs.y, z + rhs.z, z + rhs.w};
	}

	Vector4& Vector4::operator+=(const Vector4& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vector4 Vector4::operator-() const noexcept
	{
		return {-x, -y, -z, -w};
	}

	Vector4 Vector4::operator-(const Vector4& rhs) const noexcept
	{
		return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
	}

	Vector4& Vector4::operator-=(const Vector4& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vector4 operator*(float lhs_scalar, const Vector4& rhs) noexcept
	{
		//Same as rhs * lhs, but I'm striving for comprehension instead of terseness
		return {lhs_scalar * rhs.x, lhs_scalar * rhs.y, lhs_scalar * rhs.z, lhs_scalar * rhs.w};
	}

	Vector4 Vector4::operator*(float rhs_scalar) const noexcept
	{
		return {x * rhs_scalar, y * rhs_scalar, z * rhs_scalar, w * rhs_scalar};
	}

	Vector4& Vector4::operator*=(float rhs_scalar) noexcept
	{
		x *= rhs_scalar;
		y *= rhs_scalar;
		z *= rhs_scalar;
		w *= rhs_scalar;
		return *this;
	}

	Vector4 Vector4::operator*(const Vector4& rhs) const noexcept
	{
		return {x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w};
	}

	Vector4& Vector4::operator*=(const Vector4& rhs) noexcept
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	Vector4 Vector4::operator/(float scalar) const noexcept
	{
		return {x / scalar, y / scalar, z / scalar, w / scalar};
	}

	Vector4& Vector4::operator/=(float scalar) noexcept
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	Vector4 Vector4::operator/(const Vector4& rhs) const noexcept
	{
		return {x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w};
	}

	Vector4& Vector4::operator/=(const Vector4& rhs) noexcept
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	bool Vector4::operator==(const Vector4& rhs) const noexcept
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool Vector4::operator!=(const Vector4& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	Vector2 Vector4::getXY() const
	{
		return {x, y};
	}

	Vector2 Vector4::getZW() const
	{
		return {z, w};
	}

	void Vector4::getXYZ(float& outX, float& outY, float& outZ) const noexcept
	{
		outX = x;
		outY = y;
		outZ = z;
	}

	void Vector4::getXYZW(float& outX, float& outY, float& outZ, float outW) const noexcept
	{
		outX = x;
		outY = y;
		outZ = z;
		outW = w;
	}

	float* Vector4::getAsFloatArray() noexcept
	{
		return &x;
	}

	float Vector4::calcLength3D() const noexcept
	{
		return std::sqrt(calcLength3DSquared());
	}

	float Vector4::calcLength3DSquared() const noexcept
	{
		return x * x + y * y + z * z;
	}

	float Vector4::calcLength4D() const noexcept
	{
		return std::sqrt(calcLength4DSquared());
	}

	float Vector4::calcLength4DSquared() const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	void Vector4::setXYZ(float newX, float newY, float newZ) noexcept
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	void Vector4::setXYZW(float newX, float newY, float newZ, float newW) noexcept
	{
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	float Vector4::normalize3D() noexcept
	{
		float length = calcLength3D();
		if (length > 0.0f)
		{
			float inv_length = 1.0f / length;
			x *= inv_length;
			y *= inv_length;
			z *= inv_length;
			w *= inv_length;
			return length;
		}
		return 0.0f;
	}

	float Vector4::normalize4D() noexcept
	{
		float length = calcLength4D();
		if (length > 0.0f)
		{
			float inv_length = 1.0f / length;
			x *= inv_length;
			y *= inv_length;
			z *= inv_length;
			w *= inv_length;
			return length;
		}
		return 0.0f;
	}

	Vector4 Vector4::getNormalize3D() const noexcept
	{
		float length = calcLength3D();
		if (length > 0.0f)
		{
			float inv_length = 1.0f / length;
			return Vector4{x * inv_length, y * inv_length, z * inv_length, w};
		}
		return Vector4::Zero;
	}

	Vector4 Vector4::getNormalize4D() const noexcept
	{
		float length = calcLength4D();
		if (length > 0.0f)
		{
			float inv_length = 1.0f / length;
			return Vector4{x * inv_length, y * inv_length, z * inv_length, w * inv_length};
		}
		return Vector4::Zero;
	}

	Vector4 Vector4::calcHomogeneous(const Vector4& v)
	{
		return std::fabs(v.w - 0.0f) < 0.0001f == false ? v / v.w : v;
	}

	void Vector4::calcHomogeneous() noexcept
	{
		if (std::fabs(w - 0.0f) < 0.0001f == false)
		{
			x /= w;
			y /= w;
			z /= w;
			w = 1.0f;
		}
	}

	void swap(Vector4& a, Vector4& b) noexcept
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.z, b.z);
		std::swap(a.w, b.w);
	}

} // namespace NAS2D