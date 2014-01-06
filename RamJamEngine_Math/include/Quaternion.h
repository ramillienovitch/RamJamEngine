#pragma once

#include "Types.h"
#include "Matrix44.h"
#include "Vector3.h"
#include "Vector4.h"

template <typename Real>
struct Quaternion_T
{
	union
	{
		struct
		{
			Real w;
			Real x;
			Real y;
			Real z;
		};
		struct
		{
			Real			mReal;
			Vector3_T<Real>	mImaginary;
		};
		struct
		{
			Vector4_T<Real>	mVectorial;
		};
	};

	static const Quaternion_T identity;

	Quaternion_T()                               : w(1.0), x(0.0), y(0.0), z(0.0)	{}
	Quaternion_T(Real w, Real x, Real y, Real z) { Set(w,x,y,z); }
	Quaternion_T(const Quaternion_T& q)          { Set(q.w, q.x, q.y, q.z); }
	Quaternion_T(Real pitch, Real yaw, Real roll);
	Quaternion_T(Vector3_T<Real> axis, Real angle);
	Quaternion_T(Matrix44_T<Real>& rotation);

	//-------------------------

	Quaternion_T	operator +  (const Quaternion_T&);
	Quaternion_T	operator -  (const Quaternion_T&);
	Quaternion_T	operator *  (const Quaternion_T&);
	Quaternion_T	operator /  (const Real&);
	Quaternion_T&	operator =  (const Quaternion_T&);
	Quaternion_T&	operator += (const Quaternion_T&);
	Quaternion_T&	operator -= (const Quaternion_T&);
	BOOL			operator == (const Quaternion_T&);
	BOOL			operator != (const Quaternion_T&);
	//-------------------------
	operator Matrix44_T<Real>();
	//-------------------------
	void			Set(Real w, Real x, Real y, Real z);
	Real			Magnitude();
	Real			SqrMagnitude();
	BOOL			IsNormalized();
	//-------------------------
	Quaternion_T&	Conjugate();
	Quaternion_T&	Normalize();
	Quaternion_T&	Inverse();
	//-------------------------
	Quaternion_T&	LookAt(Vector3_T<Real>& lookAt, Vector3_T<Real>& up);
	Quaternion_T&	LookAt(Vector3_T<Real>& lookAt);
	//-------------------------
	Vector3_T<Real>		Rotate(const Real& x, const Real& y, const Real& z);
	Vector3_T<Real>		Rotate(const Vector3_T<Real>&);
	//-------------------------
	Matrix44_T<Real>	ToMatrix();
	Vector3_T<Real>		ToEulerAngles();
	Vector3_T<Real>		GetRightVector()   const;
	Vector3_T<Real>		GetUpVector()      const;
	Vector3_T<Real>		GetForwardVector() const;
	//-------------------------
	static void		Slerp(OUT Quaternion_T& qOut, const IN Quaternion_T& qStart, const IN Quaternion_T& qEnd, Real factor);
};

typedef Quaternion_T<f32> Quaternion;

//--------------------------------------------------
// Use to cast a quaternion from AntTweak GUI to our quaternion system
struct TwQuaternion
{
	float q[4];
	//-----------
	TwQuaternion()											{ q[0] = 0.0f; q[1] = 0.0f; q[2] = 0.0f; q[3] = 1.0f; }
	TwQuaternion(float q0, float q1, float q2, float q3)	{ q[0] = q0; q[1] = q1; q[2] = q2; q[3] = q3;}
	TwQuaternion(Quaternion quat)							{ q[0] = quat.z; q[1] = quat.w; q[2] = quat.x; q[3] = quat.y; }
	operator Quaternion() { return Quaternion(q[3], q[0], q[1], -q[2]); }
	Matrix44 ToMatrix() {Quaternion quat = *this; return quat.ToMatrix();}
};

#include "Quaternion.inl"
