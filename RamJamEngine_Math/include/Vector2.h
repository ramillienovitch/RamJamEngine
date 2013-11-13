#pragma once

#include "Types.h"
#include <DirectXMath.h>

template <typename Real>
struct Vector2_T
{
	Real x;
	Real y;

	Vector2_T()                   : x(), y()       {}
	Vector2_T(Real x)             : x(x), y(x)     {}
	Vector2_T(Real x, Real y)     : x(x), y(y)     {}
	Vector2_T(const Vector2_T& v) : x(v.x), y(v.y) {}

	static const Vector2_T zero;
	static const Vector2_T one;

	Vector2_T	operator +  (const Vector2_T&);
	Vector2_T	operator -  (const Vector2_T&);
	Vector2_T	operator -  ();
	Vector2_T	operator /  (const Vector2_T&);
	Vector2_T	operator /  (const Real&);
	Vector2_T	operator *  (const Real&);
	Vector2_T&	operator =  (const Vector2_T&);
	Vector2_T&	operator =  (const DirectX::XMFLOAT2&);
	Vector2_T&	operator += (const Vector2_T&);
	Vector2_T&	operator -= (const Vector2_T&);
	Vector2_T&	operator /= (const Vector2_T&);
	Vector2_T&	operator /= (const Real& f);
	Vector2_T&	operator *= (const Real& f);
	BOOL		operator == (const Vector2_T&);
	BOOL		operator != (const Vector2_T&);
	//---------------
	operator DirectX::XMFLOAT2();
	//---------------
	void		Set (Real x, Real y);
	Real		SqrMagnitude();
	Real		Magnitude();
	//---------------
	Vector2_T&	Normalize();
	Vector2_T&	Scale(const Vector2_T& v);
};

typedef Vector2_T<f32> Vector2;

#include "Vector2.inl"