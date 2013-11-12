#pragma once

#include "Types.h"
#include <DirectXMath.h>

template <typename Real>
struct Matrix44_T
{
	Real m11, m12, m13, m14;
	Real m21, m22, m23, m24;
	Real m31, m32, m33, m34;
	Real m41, m42, m43, m44;

	Matrix44_T();
	Matrix44_T(	Real m11, Real m12, Real m13, Real m14,
				Real m21, Real m22, Real m23, Real m24,
				Real m31, Real m32, Real m33, Real m34,
				Real m41, Real m42, Real m43, Real m44);
	Matrix44_T(const Matrix44_T&);

	static const Matrix44_T identity;

	Matrix44_T  operator + (const Matrix44_T&);
	Matrix44_T  operator - (const Matrix44_T&);
	Matrix44_T  operator * (const Matrix44_T&);
	Matrix44_T& operator = (const Matrix44_T&);
	Matrix44_T& operator = (const DirectX::XMMATRIX&);

	operator DirectX::XMMATRIX();

	friend Matrix44_T operator == (const Matrix44_T&, const Matrix44_T&);
	//---------------------------
	Real		Trace();
	// TODO: add matrix "everything" !
};

typedef Matrix44_T<f32> Matrix44;

#include "Matrix44.inl"
