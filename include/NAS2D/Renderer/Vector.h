#pragma once

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

	template <typename NewBaseType>
	operator Vector<NewBaseType>() const {
		return {
			static_cast<NewBaseType>(x),
			static_cast<NewBaseType>(y)
		};
	}

};

}
