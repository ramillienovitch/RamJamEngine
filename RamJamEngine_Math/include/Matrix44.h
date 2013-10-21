#pragma once

#include "Types.h"
#include <DirectXMath.h>

struct Matrix44
{
	f32 m11, m12, m13, m14;
	f32 m21, m22, m23, m24;
	f32 m31, m32, m33, m34;
	f32 m41, m42, m43, m44;

	Matrix44();
	Matrix44(	f32 m11, f32 m12, f32 m13, f32 m14,
				f32 m21, f32 m22, f32 m23, f32 m24,
				f32 m31, f32 m32, f32 m33, f32 m34,
				f32 m41, f32 m42, f32 m43, f32 m44);
	Matrix44(const Matrix44&);
	~Matrix44();

	static const Matrix44 identity;

	Matrix44  operator + (const Matrix44&);
	Matrix44  operator - (const Matrix44&);
	Matrix44  operator * (const Matrix44&);
	Matrix44& operator = (const Matrix44&);
	Matrix44& operator = (const DirectX::XMMATRIX&);

	operator DirectX::XMMATRIX();

	friend Matrix44 operator == (const Matrix44&, const Matrix44&);

	// TODO: add matrix "everything" !
};