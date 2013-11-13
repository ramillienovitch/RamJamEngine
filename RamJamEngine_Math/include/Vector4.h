#pragma once

#include "Types.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

template <typename Real>
struct Vector4_T
{
	Real w;
	Real x;
	Real y;
	Real z;

	Vector4_T()                               : w()   , x(),    y(),    z()    {}
	Vector4_T(Real val)                       : w(val), x(val), y(val), z(val) {}
	Vector4_T(Real w, Real x, Real y, Real z) : w(w)  , x(x),   y(y),   z(z)   {}
	Vector4_T(const Vector4_T& v)             : w(v.w), x(v.x), y(v.y), z(v.z) {}

	static const Vector4_T zero;
	static const Vector4_T one;

	Vector4_T	operator +  (const Vector4_T&);
	Vector4_T	operator -  (const Vector4_T&);
	Vector4_T	operator -  ();
	Vector4_T	operator /  (const Vector4_T&);
	Vector4_T	operator /  (const Real&);
	Vector4_T	operator *  (const Real&);
	Vector4_T&	operator =  (const Vector4_T&);
	Vector4_T&	operator =  (const DirectX::XMFLOAT4&);
	Vector4_T&	operator =  (const DirectX::PackedVector::XMCOLOR&);
	Vector4_T&	operator += (const Vector4_T&);
	Vector4_T&	operator -= (const Vector4_T&);
	Vector4_T&	operator /= (const Vector4_T&);
	Vector4_T&	operator /= (const Real& f);
	Vector4_T&	operator *= (const Real& f);
	BOOL		operator == (const Vector4_T&);
	BOOL		operator != (const Vector4_T&);
	//---------------
	operator DirectX::XMFLOAT4();
	operator DirectX::PackedVector::XMCOLOR();
	//---------------
	void		Set (Real w, Real x, Real y, Real z);
	Real		SqrMagnitude();
	Real		Magnitude();
	//---------------
	Vector4_T&	Normalize();
	Vector4_T&	Scale(const Vector4_T& v);
};

typedef Vector4_T<f32> Vector4;

#include "Vector4.inl"