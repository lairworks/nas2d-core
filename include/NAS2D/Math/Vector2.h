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

	class Vector2
	{
	  public:
		static const Vector2 Zero;
		static const Vector2 X_Axis;
		static const Vector2 Y_Axis;
		static const Vector2 One;
		static const Vector2 XY_Axis;
		static const Vector2 YX_Axis;

		Vector2() = default;
		Vector2(const Vector2& rhs) = default;
		Vector2(Vector2&& rhs) = default;
		Vector2& operator=(const Vector2& other) = default;
		Vector2& operator=(Vector2&& r_other) = default;
		~Vector2() = default;
		Vector2(float x, float y) noexcept;
        //TODO: Construct from std::string

		Vector2 operator+(const Vector2& rhs) const noexcept;
		Vector2& operator+=(const Vector2& rhs) noexcept;

		Vector2 operator-() const noexcept;
		Vector2 operator-(const Vector2& rhs) const noexcept;
		Vector2& operator-=(const Vector2& rhs) noexcept;

		friend Vector2 operator*(float lhs_scalar, const Vector2& rhs) noexcept;
		Vector2 operator*(float rhs_scalar) const noexcept;
		Vector2& operator*=(float rhs_scalar) noexcept;
		Vector2 operator*(const Vector2& rhs) const noexcept;
		Vector2& operator*=(const Vector2& rhs) noexcept;

		Vector2 operator/(float rhs_scalar) const noexcept;
		Vector2& operator/=(float rhs_scalar) noexcept;
		Vector2 operator/(const Vector2& rhs) const noexcept;
		Vector2& operator/=(const Vector2& rhs) noexcept;

		bool operator==(const Vector2& rhs) const noexcept;
		bool operator!=(const Vector2& rhs) const noexcept;

		void getXY(float& x, float& y) const noexcept;
		float* getAsFloatArray() noexcept;

		float calcHeadingRadians() const noexcept;
		float calcHeadingDegrees() const noexcept;
		float calcLength() const noexcept;
		float calcLengthSquared() const noexcept;

		void setXY(float newX, float newY) noexcept;
		void setHeadingRadians(float headingRadians) noexcept;
		void setHeadingDegrees(float headingDegrees) noexcept;
		void setUnitLengthAndHeadingRadians(float headingRadians) noexcept;
		void setUnitLengthAndHeadingDegrees(float headingDegrees) noexcept;
		float setLength(float length) noexcept;
		void setLengthAndHeadingRadians(float headingRadians, float length) noexcept;
		void setLengthAndHeadingDegrees(float headingDegrees, float length) noexcept;

		float normalize() noexcept;
		Vector2 getNormalize() const noexcept;

		void rotate90Degrees() noexcept;
		void rotateNegative90Degrees() noexcept;
		void rotateRadians(float radiansToAdd) noexcept;
		void rotateDegrees(float degreesToAdd) noexcept;

		friend void swap(Vector2& a, Vector2& b) noexcept;

        //TODO: streaming operations

		float x = 0.0f;
		float y = 0.0f;

	  protected:
	  private:
		float calcInverseLength() const noexcept;
	};

} // namespace NAS2D