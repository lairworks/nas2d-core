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

namespace NAS2D
{

	class Vector2;

	class Vector3
	{
	  public:
		static const Vector3 Zero;
		static const Vector3 X_Axis;
		static const Vector3 Y_Axis;
		static const Vector3 Z_Axis;
		static const Vector3 One;
		static const Vector3 XY_Axis;
		static const Vector3 XZ_Axis;
		static const Vector3 YX_Axis;
		static const Vector3 YZ_Axis;
		static const Vector3 ZX_Axis;
		static const Vector3 ZY_Axis;
		static const Vector3 XYZ_Axis;

		Vector3() = default;
		Vector3(const Vector3& rhs) = default;
		Vector3(Vector3&& rhs) = default;
		Vector3& operator=(const Vector3& other) = default;
		Vector3& operator=(Vector3&& r_other) = default;
		~Vector3() = default;
		Vector3(float x, float y, float z) noexcept;
		Vector3(const Vector2& xy, float z) noexcept;
		//TODO: Construct from std::string

		Vector3 operator+(const Vector3& rhs) const noexcept;
		Vector3& operator+=(const Vector3& rhs) noexcept;

		Vector3 operator-() const noexcept;
		Vector3 operator-(const Vector3& rhs) const noexcept;
		Vector3& operator-=(const Vector3& rhs) noexcept;

		friend Vector3 operator*(float lhs_scalar, const Vector3& rhs) noexcept;
		Vector3 operator*(float rhs_scalar) const noexcept;
		Vector3& operator*=(float rhs_scalar) noexcept;
		Vector3 operator*(const Vector3& rhs) const noexcept;
		Vector3& operator*=(const Vector3& rhs) noexcept;

		Vector3 operator/(float rhs_scalar) const noexcept;
		Vector3& operator/=(float rhs_scalar) noexcept;
		Vector3 operator/(const Vector3& rhs) const noexcept;
		Vector3& operator/=(const Vector3& rhs) noexcept;

		bool operator==(const Vector3& rhs) const noexcept;
		bool operator!=(const Vector3& rhs) const noexcept;

		void getXYZ(float& x, float& y, float& z) const noexcept;
		float* getAsFloatArray() noexcept;

		float calcLength() const noexcept;
		float calcLengthSquared() const noexcept;

		void setXYZ(float newX, float newY, float newZ) noexcept;

		float normalize() noexcept;
		Vector3 getNormalize() const noexcept;

		friend void swap(Vector3& a, Vector3& b) noexcept;

		//TODO: streaming operations

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

	  protected:
	  private:
		float calcInverseLength() const noexcept;
	};

} // namespace NAS2D