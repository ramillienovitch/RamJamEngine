#pragma once

#include "Types.h"

template <typename Real>
struct Vector4_T
{
	Real x;
	Real y;
	Real z;
	Real w;

	Vector4_T()                               : x(),    y(),    z(),    w() {};
	Vector4_T(Real val)                       : x(val), y(val), z(val), w(val) {};
	Vector4_T(Real x, Real y, Real z, Real w) : x(x),   y(y),   z(z),   w(w) {};
	Vector4_T(const Vector4_T& v)             : x(v.x), y(v.y), z(v.z), w(v.w) {};

	static const Vector4_T zero;
	static const Vector4_T one;

	Vector4_T	operator + (const Vector4_T&);
	Vector4_T	operator - (const Vector4_T&);
	Vector4_T	operator * (const Vector4_T&);
	Vector4_T&	operator = (const Vector4_T&);
	friend BOOL	operator == (const Vector4_T&, const Vector4_T&);

	// TODO: add vector "everything" !
};

typedef Vector4_T<f32> Vector4;

#include "Vector4.inl"