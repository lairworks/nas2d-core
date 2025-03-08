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

#include "IsZero.h"


namespace NAS2D
{
	template <typename BaseType>
	struct Vector
	{
		BaseType x = 0;
		BaseType y = 0;

		constexpr bool operator==(const Vector& vector) const
		{
			return isEqual(x, vector.x) && isEqual(y, vector.y);
		}

		constexpr bool operator!=(const Vector& vector) const
		{
			return !(*this == vector);
		}

		Vector& operator+=(const Vector& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		Vector& operator-=(const Vector& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}

		constexpr Vector operator+(const Vector& vector) const
		{
			return {x + vector.x, y + vector.y};
		}

		constexpr Vector operator-(const Vector& vector) const
		{
			return {x - vector.x, y - vector.y};
		}

		Vector& operator*=(BaseType scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vector& operator/=(BaseType scalar)
		{
			verifyNotZero(scalar, "Cannot divide vector by 0");
			x /= scalar;
			y /= scalar;
			return *this;
		}

		constexpr Vector operator*(BaseType scalar) const
		{
			return {x * scalar, y * scalar};
		}

		constexpr Vector operator/(BaseType scalar) const
		{
			verifyNotZero(scalar, "Cannot divide vector by 0");
			return {x / scalar, y / scalar};
		}

		constexpr Vector skewBy(const Vector& other) const
		{
			return {x * other.x, y * other.y};
		}

		constexpr Vector skewInverseBy(const Vector& other) const
		{
			verifyNotZero(other.x, other.y, "Cannot skewInverseBy a vector with a zero component");
			return {x / other.x, y / other.y};
		}

		constexpr BaseType lengthSquared() const
		{
			return (x * x) + (y * y);
		}

		constexpr BaseType dotProduct(const Vector& other) const
		{
			return (x * other.x) + (y * other.y);
		}

		constexpr Vector reflectX() const
		{
			return {-x, y};
		}

		constexpr Vector reflectY() const
		{
			return {x, -y};
		}

		template <typename NewBaseType>
		constexpr operator Vector<NewBaseType>() const
		{
			return {NewBaseType(x), NewBaseType(y)};
		}

		template <typename NewBaseType>
		constexpr Vector<NewBaseType> to() const
		{
			return Vector<NewBaseType>(*this);
		}
	};


	template <typename BaseType>
	Vector(BaseType, BaseType) -> Vector<BaseType>;


	// Commutative scalar * vector
	template <typename BaseType>
	constexpr Vector<BaseType> operator*(BaseType scalar, Vector<BaseType> vector)
	{
		return vector * scalar;
	}


	// Partial order comparisons

	template <typename BaseType>
	bool operator<=(Vector<BaseType> v1, Vector<BaseType> v2)
	{
		return (v1.x <= v2.x) && (v1.y <= v2.y);
	}

	template <typename BaseType>
	bool operator>=(Vector<BaseType> v1, Vector<BaseType> v2)
	{
		return v2 <= v1;
	}

	template <typename BaseType>
	bool operator<(Vector<BaseType> v1, Vector<BaseType> v2)
	{
		return (v1.x < v2.x) && (v1.y < v2.y);
	}

	template <typename BaseType>
	bool operator>(Vector<BaseType> v1, Vector<BaseType> v2)
	{
		return v2 < v1;
	}


	extern template struct Vector<int>;
	extern template struct Vector<float>;
}
