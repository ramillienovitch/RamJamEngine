#pragma once

#include "Types.h"

template <typename Real>
struct Vector3_T
{
	Real x;
	Real y;
	Real z;

	Vector3_T()                       : x(), y(), z()    {};
	Vector3_T(Real val)               : x(val), y(val)   {};
	Vector3_T(Real x, Real y, Real z) : x(x), y(y), z(z) {};
	Vector3_T(const Vector3_T& v)     : x(v.x), y(v.y), z(v.z) {};

	static const Vector3_T zero;
	static const Vector3_T one;
	static const Vector3_T right;
	static const Vector3_T up;
	static const Vector3_T forward;

	Vector3_T	operator +  (const Vector3_T&);
	Vector3_T	operator -  (const Vector3_T&);
	Vector3_T	operator -  ();
	Vector3_T	operator /  (const Vector3_T&);
	Vector3_T	operator /  (const Real&);
	Vector3_T	operator *  (const Real&);
	Vector3_T&	operator =  (const Vector3_T&);
	Vector3_T&	operator += (const Vector3_T&);
	Vector3_T&	operator -= (const Vector3_T&);
	Vector3_T&	operator /= (const Vector3_T&);
	Vector3_T&	operator /= (const Real& f);
	Vector3_T&	operator *= (const Real& f);
	//Vector3_T	operator *= (const Matrix33_T&);
	BOOL		operator == (const Vector3_T&);
	BOOL		operator != (const Vector3_T&);
	//---------------
	void		Set (Real x, Real y, Real z);
	Real		SqrMagnitude();
	Real		Magnitude();
	//---------------
	Vector3_T&	Normalize();
	Vector3_T&	Scale(const Vector3_T& v);
	//---------------
	static Vector3_T	Cross(const Vector3_T& v1, const Vector3_T& v2);
	static Real			Dot  (const Vector3_T& v1, const Vector3_T& v2);
};

typedef Vector3_T<f32> Vector3;

#include "Vector3.inl"
