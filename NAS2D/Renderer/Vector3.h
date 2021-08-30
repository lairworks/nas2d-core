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

#include <cmath>
#include <stdexcept>


namespace NAS2D
{
	template <typename BaseType>
	struct Vector3
	{

		BaseType x{};
		BaseType y{};
		BaseType z{};

		static Vector3<BaseType> Zero;
		static Vector3<BaseType> X_Axis;
		static Vector3<BaseType> Y_Axis;
		static Vector3<BaseType> Z_Axis;
		static Vector3<BaseType> XY_Axis;
		static Vector3<BaseType> XZ_Axis;
		static Vector3<BaseType> YZ_Axis;
		static Vector3<BaseType> One;

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

		constexpr Vector3 operator-() const {
			return {-x, -y, -z};
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
			//Floating-point -> Integral conversions require a call to floor due to rounding/truncation rules of negative numbers.
			if constexpr(std::is_floating_point_v<BaseType> && std::is_integral_v<NewBaseType>)
			{
				return {static_cast<NewBaseType>(std::floor(x)), static_cast<NewBaseType>(std::floor(y)), static_cast<NewBaseType>(std::floor(z))};
			}
			else
			{
				return {static_cast<NewBaseType>(x), static_cast<NewBaseType>(y), static_cast<NewBaseType>(z)};
			}
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

template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::Zero{BaseType{}, BaseType{}, BaseType{}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::X_Axis{BaseType{1}, BaseType{}, BaseType{}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::Y_Axis{BaseType{}, BaseType{1}, BaseType{}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::Z_Axis{BaseType{}, BaseType{}, BaseType{1}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::XY_Axis{BaseType{1}, BaseType{1}, BaseType{}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::XZ_Axis{BaseType{1}, BaseType{}, BaseType{1}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::YZ_Axis{BaseType{}, BaseType{1}, BaseType{1}};
template<typename BaseType>
NAS2D::Vector3<BaseType> NAS2D::Vector3<BaseType>::One{BaseType{1}, BaseType{1}, BaseType{1}};
