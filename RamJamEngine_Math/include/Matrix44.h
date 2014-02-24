#pragma once

#include "Types.h"
#include "Quaternion.h"
#include "Vector3.h"
#include <DirectXMath.h>

template <typename Real>
struct Quaternion_T;

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
	Matrix44_T(const DirectX::XMMATRIX&);
	//------------
	static const Matrix44_T identity;
	//------------
	Matrix44_T		operator +  (const Matrix44_T&);
	Matrix44_T		operator -  (const Matrix44_T&);
	Matrix44_T		operator *  (const Matrix44_T&);
	Vector3_T<Real>	operator *  (const Vector3_T<Real>&);
	Vector4_T<Real>	operator *  (const Vector4_T<Real>&);
	Matrix44_T&		operator *= (const Matrix44_T&);
	Matrix44_T&		operator =  (const Matrix44_T&);
	Matrix44_T&		operator =  (const DirectX::XMMATRIX&);
	BOOL			operator == (const Matrix44_T&);
	BOOL			operator != (const Matrix44_T&);
	//---------------------------
	operator DirectX::XMMATRIX();
	//---------------------------
	Real			Trace();
	Real			Determinant();
	Matrix44_T&		Transpose();
	Matrix44_T&		Inverse();
	Matrix44_T&		InverseTranspose();
	Vector3_T<Real>	TransformVector(Vector3_T<Real>);
	//---------------------------
	BOOL IsIdentity() const;
	//---------------------------
	void	Decompose(OUT Vector3_T<Real>& position, OUT Vector3_T<Real>& scale, OUT Quaternion_T<Real>& rotation);
	void	DecomposeNoScale(OUT Vector3_T<Real>& position, OUT Quaternion_T<Real>& rotation);
	//---------------------------
	static Matrix44_T&	FromEulerAngles(Matrix44_T<Real>& m, Real x, Real y, Real z);
	static Matrix44_T&	FromEulerAngles(Matrix44_T<Real>& m, Vector3_T<Real> euler);
	//---------------------------
	static Matrix44_T	Translation(Vector3_T<Real>);
	static Matrix44_T	Translation(Real x, Real y, Real z);
	static Matrix44_T	Scaling(Vector3_T<Real>);
	static Matrix44_T	Scaling(Real x, Real y, Real z);
	static Matrix44_T	RotationX(Real degrees);
	static Matrix44_T	RotationY(Real degrees);
	static Matrix44_T	RotationZ(Real degrees);
	//---------------------------
	static Matrix44_T	LookAt(Vector3_T<Real> pos, Vector3_T<Real> dir, Vector3_T<Real> up);
	static Matrix44_T	LookAt(Vector3_T<Real> pos, Vector3_T<Real> dir);
	static void			LookAt(Matrix44_T& mOut, Vector3_T<Real> pos, Vector3_T<Real> dir);
	//---------------------------
	static Matrix44_T	PerspectiveFov(Real Fov, Real AspectRatio, Real NearZ, Real FarZ);
	static Matrix44_T	Orthographic(Real ViewWidth, Real ViewHeight, Real NearZ, Real FarZ);
	static void			Orthographic(Matrix44_T& mOut, Real ViewWidth, Real ViewHeight, Real NearZ, Real FarZ);
	//---------------------------
	// Transformation Matrix for the reflection with the plane equation Ax+By+Cz+D=0.
	static Matrix44_T	Reflection(Vector4_T<Real> planeEquation);
	static Matrix44_T	Reflection(Real a, Real b, Real c, Real d);
};

typedef Matrix44_T<f32> Matrix44;

#include "Matrix44.inl"
