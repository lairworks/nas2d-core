#pragma once

#include <stdexcept>


namespace NAS2D {


template <typename BaseType>
struct Vector {
	BaseType x = 0;
	BaseType y = 0;

	bool operator==(const Vector& vector) const {
		return (x == vector.x) && (y == vector.y);
	}
	bool operator!=(const Vector& vector) const {
		return !(*this == vector);
	}

	Vector& operator+=(const Vector& vector) {
		x += vector.x;
		y += vector.y;
		return *this;
	}
	Vector& operator-=(const Vector& vector) {
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	Vector operator+(const Vector& vector) const {
		return {
			x + vector.x,
			y + vector.y
		};
	}
	Vector operator-(const Vector& vector) const {
		return {
			x - vector.x,
			y - vector.y
		};
	}

	Vector& operator*=(BaseType scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	Vector& operator/=(BaseType scalar) {
		if (scalar == 0) {
			throw std::domain_error("Cannot divide vector by 0");
		}
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector operator*(BaseType scalar) const {
		return {x * scalar, y * scalar};
	}
	Vector operator/(BaseType scalar) const {
		if (scalar == 0) {
			throw std::domain_error("Cannot divide vector by 0");
		}
		return {x / scalar, y / scalar};
	}

	BaseType lengthSquared() const {
		return (x * x) + (y * y);
	}

	BaseType dotProduct(const Vector& other) const {
		return (x * other.x) + (y * other.y);
	}

	template <typename NewBaseType>
	operator Vector<NewBaseType>() const {
		return {
			static_cast<NewBaseType>(x),
			static_cast<NewBaseType>(y)
		};
	}

	template <typename NewBaseType>
	Vector<NewBaseType> to() const {
		return static_cast<Vector<NewBaseType>>(*this);
	}
};


template <typename BaseType>
Vector(BaseType, BaseType) -> Vector<BaseType>;

}
