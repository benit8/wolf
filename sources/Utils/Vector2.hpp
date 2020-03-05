/*
** wolf, 2020
** Vector2.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Vector2
{
	T x, y;

	Vector2()
	: x(0)
	, y(0)
	{}

	Vector2(T _x, T _y)
	: x(_x)
	, y(_y)
	{}

	Vector2(const Vector2 &v)
	: x(v.x)
	, y(v.y)
	{}

	template <typename U>
	explicit Vector2(const Vector2<U> &v)
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	{}

	Vector2 &rotate(double a) {
		return *this = {
			x * cos(a) - y * sin(a),
			x * sin(a) + y * cos(a)
		};
	}

	Vector2 rotated(double a) const {
		return {
			x * cos(a) - y * sin(a),
			x * sin(a) + y * cos(a)
		};
	}

	Vector2 operator +(const Vector2 &rhs) const { return { x + rhs.x, y + rhs.y }; }
	Vector2 operator -(const Vector2 &rhs) const { return { x - rhs.x, y - rhs.y }; }
	Vector2 operator *(T rhs) const { return { x * rhs, y * rhs }; }
	Vector2 operator /(T rhs) const { return { x / rhs, y / rhs }; }
	Vector2 &operator +=(const Vector2 &rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vector2 &operator -=(const Vector2 &rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	Vector2 &operator *=(T rhs) const { x *= rhs; y *= rhs; return *this; }
	Vector2 &operator /=(T rhs) const { x /= rhs; y /= rhs; return *this; }

	Vector2 &operator =(const Vector2 &rhs) { x = rhs.x; y = rhs.y; return *this; }
};

typedef Vector2<double> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector2<unsigned> Vector2u;