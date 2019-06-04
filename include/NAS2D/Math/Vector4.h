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
	class Vector3;

	class Vector4
	{
	  public:
		static const Vector4 Zero;
		static const Vector4 Zero_XYZ_One_W;
		static const Vector4 One_XYZ_Zero_W;
		static const Vector4 X_Axis;
		static const Vector4 XY_Axis;
		static const Vector4 XZ_Axis;
		static const Vector4 Y_Axis;
		static const Vector4 YX_Axis;
		static const Vector4 YZ_Axis;
		static const Vector4 Z_Axis;
		static const Vector4 ZX_Axis;
		static const Vector4 ZY_Axis;
		static const Vector4 W_Axis;
		static const Vector4 WX_Axis;
		static const Vector4 WY_Axis;
		static const Vector4 WZ_Axis;
		static const Vector4 XYZ_Axis;
		static const Vector4 YZW_Axis;
		static const Vector4 XZW_Axis;
		static const Vector4 XYW_Axis;
		static const Vector4 XYZW_Axis;
		static const Vector4 One;

		static Vector4 calcHomogeneous(const Vector4& v);

		Vector4() = default;
		Vector4(const Vector4& rhs) = default;
		Vector4(Vector4&& rhs) = default;
		Vector4& operator=(const Vector4& other) = default;
		Vector4& operator=(Vector4&& r_other) = default;
		~Vector4() = default;
		Vector4(float x, float y, float z, float w) noexcept;
		Vector4(const Vector2& xy, float z, float w) noexcept;
		Vector4(const Vector3& xyz, float w) noexcept;
		Vector4(const Vector2& xy, const Vector2& zw) noexcept;
		//TODO: Construct from std::string

		Vector4 operator+(const Vector4& rhs) const noexcept;
		Vector4& operator+=(const Vector4& rhs) noexcept;

		Vector4 operator-() const noexcept;
		Vector4 operator-(const Vector4& rhs) const noexcept;
		Vector4& operator-=(const Vector4& rhs) noexcept;

		friend Vector4 operator*(float lhs_scalar, const Vector4& rhs) noexcept;
		Vector4 operator*(float rhs_scalar) const noexcept;
		Vector4& operator*=(float rhs_scalar) noexcept;
		Vector4 operator*(const Vector4& rhs) const noexcept;
		Vector4& operator*=(const Vector4& rhs) noexcept;

		Vector4 operator/(float rhs_scalar) const noexcept;
		Vector4& operator/=(float rhs_scalar) noexcept;
		Vector4 operator/(const Vector4& rhs) const noexcept;
		Vector4& operator/=(const Vector4& rhs) noexcept;

		bool operator==(const Vector4& rhs) const noexcept;
		bool operator!=(const Vector4& rhs) const noexcept;

		Vector2 getXY() const;
		Vector2 getZW() const;

		void getXYZ(float& x, float& y, float& z) const noexcept;
		void getXYZW(float& x, float& y, float& z, float w) const noexcept;
		float* getAsFloatArray() noexcept;

		float calcLength3D() const noexcept;
		float calcLength3DSquared() const noexcept;
		float calcLength4D() const noexcept;
		float calcLength4DSquared() const noexcept;
		void calcHomogeneous() noexcept;

		void setXYZ(float newX, float newY, float newZ) noexcept;
		void setXYZW(float newX, float newY, float newZ, float newW) noexcept;

		float normalize3D() noexcept;
		float normalize4D() noexcept;
		Vector4 getNormalize3D() const noexcept;
		Vector4 getNormalize4D() const noexcept;

		friend void swap(Vector4& a, Vector4& b) noexcept;

		//TODO: streaming operations

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

	  protected:
	  private:
	};

} // namespace NAS2D