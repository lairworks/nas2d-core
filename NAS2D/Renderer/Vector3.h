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

#include <stdexcept>


namespace NAS2D
{
	template <typename BaseType>
	struct Vector3
	{
		static inline Vector3 Zero = Vector3(BaseType{}, BaseType{}, BaseType{});
		static inline Vector3 X_Axis = Vector3(BaseType{1}, BaseType{}, BaseType{});
		static inline Vector3 Y_Axis = Vector3(BaseType{}, BaseType{1}, BaseType{});
		static inline Vector3 Z_Axis = Vector3(BaseType{}, BaseType{}, BaseType{1});
		static inline Vector3 XY_Axis = Vector3(BaseType{1}, BaseType{1}, BaseType{});
		static inline Vector3 XZ_Axis = Vector3(BaseType{1}, BaseType{}, BaseType{1});
		static inline Vector3 YZ_Axis = Vector3(BaseType{}, BaseType{1}, BaseType{1});
		static inline Vector3 One = Vector3(BaseType{1}, BaseType{1}, BaseType{1});

		BaseType x{};
		BaseType y{};
		BaseType z{};

		constexpr bool operator==(const Vector3& vector) const
		{
			return (x == vector.x) && (y == vector.y) && (z == vector.z);
		}

		constexpr bool operator!=(const Vector3& vector) const
		{
			return !(*this == vector);
		}

		Vector3& operator+=(const Vector3& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			return *this;
		}

		constexpr Vector3 operator+(const Vector3& vector) const
		{
			return {x + vector.x, y + vector.y, z + vector.z};
		}

		constexpr Vector3 operator-(const Vector3& vector) const
		{
			return {x - vector.x, y - vector.y, z - vector.z};
		}

		Vector3& operator*=(BaseType scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		Vector3& operator/=(BaseType scalar)
		{
			if (scalar == 0)
			{
				throw std::domain_error("Cannot divide vector by 0");
			}
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		constexpr Vector3 operator*(BaseType scalar) const
		{
			return {x * scalar, y * scalar, z * scalar};
		}

		constexpr Vector3 operator/(BaseType scalar) const
		{
			if (scalar == 0)
			{
				throw std::domain_error("Cannot divide vector by 0");
			}
			return {x / scalar, y / scalar, z / scalar};
		}

		constexpr Vector3 skewBy(const Vector3& other) const
		{
			return {x * other.x, y * other.y, z * other.z};
		}

		constexpr Vector3 skewInverseBy(const Vector3& other) const
		{
			if (other.x == 0 || other.y == 0 || other.z == 0)
			{
				throw std::domain_error("Cannot skewInverseBy a vector with a zero component");
			}
			return {x / other.x, y / other.y, z / other.z};
		}

		constexpr BaseType lengthSquared() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		constexpr BaseType dotProduct(const Vector3& other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z);
		}
		
		constexpr Vector3 crossProduct(const Vector3& other) const
		{
			const auto a1 = a.x;
			const auto a2 = a.y;
			const auto a3 = a.z;

			const auto b1 = b.x;
			const auto b2 = b.y;
			const auto b3 = b.z;

			return Vector3(a2 * b3 - a3 * b2, a3 * b1 - a1 * b3, a1 * b2 - a2 * b1);
		}

		constexpr Vector3 reflectX() const
		{
			return {-x, y, z};
		}

		constexpr Vector3 reflectY() const
		{
			return {x, -y, z};
		}
		
		constexpr Vector3 reflectZ() const
		{
			return {x, y, -z};
		}

		template <typename NewBaseType>
		constexpr operator Vector3<NewBaseType>() const
		{
			return {static_cast<NewBaseType>(x), static_cast<NewBaseType>(y), static_cast<NewBaseType>(z)};
		}

		template <typename NewBaseType>
		constexpr Vector3<NewBaseType> to() const
		{
			return static_cast<Vector3<NewBaseType>>(*this);
		}
	};


	// Commutative scalar * vector
	template <typename BaseType>
	constexpr Vector3<BaseType> operator*(BaseType scalar, Vector3<BaseType> vector)
	{
		return vector * scalar;
	}


	template <typename BaseType>
	Vector3(BaseType, BaseType, BaseType) -> Vector3<BaseType>;


	// Partial order comparisons

	template <typename BaseType>
	bool operator<=(Vector3<BaseType> v1, Vector3<BaseType> v2)
	{
		return (v1.x <= v2.x) && (v1.y <= v2.y) && (v1.z <= v2.z);
	}

	template <typename BaseType>
	bool operator>=(Vector3<BaseType> v1, Vector3<BaseType> v2)
	{
		return v2 <= v1;
	}

	template <typename BaseType>
	bool operator<(Vector3<BaseType> v1, Vector3<BaseType> v2)
	{
		return (v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z);
	}

	template <typename BaseType>
	bool operator>(Vector3<BaseType> v1, Vector3<BaseType> v2)
	{
		return v2 < v1;
	}
}
