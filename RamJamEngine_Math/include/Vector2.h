#pragma once

#include "Types.h"

template <typename Real>
struct Vector2_T
{
	Real x;
	Real y;

	Vector2_T()                   : x(), y()   {};
	Vector2_T(Real x)             : x(x), y(x) {};
	Vector2_T(Real x, Real y)     : x(x), y(y) {};
	Vector2_T(const Vector2_T& v) : x(v.x), y(v.y) {};

	static const Vector2_T zero;
	static const Vector2_T one;

	Vector2_T	operator + (const Vector2_T&);
	Vector2_T	operator - (const Vector2_T&);
	Vector2_T	operator * (const Vector2_T&);
	Vector2_T&	operator = (const Vector2_T&);
	friend BOOL	operator == (const Vector2_T&, const Vector2_T&);

	// TODO: add vector "everything" !
};

typedef Vector2_T<f32> Vector2;

#include "Vector2.inl"