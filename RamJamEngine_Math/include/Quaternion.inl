//////////////////////////////////////////////////////////////////////////

#include "MathHelper.h"

//----------------------------
template<typename Real>
const Quaternion_T<Real> Quaternion_T<Real>::identity = Quaternion_T<Real>();

//----------------------------
template<typename Real>
Quaternion_T<Real>::Quaternion_T(Real pitch, Real yaw, Real roll)
{
	// TODO: Fix this function => Do Not Use A Matrix !!

	Matrix44_T<Real> m;
	const Real fSinPitch(sin(pitch));
	const Real fCosPitch(cos(pitch));
	const Real fSinYaw(  sin(yaw));
	const Real fCosYaw(  cos(yaw));
	const Real fSinRoll( sin(roll));
	const Real fCosRoll( cos(roll));
	const Real fCosPitchSinYaw(fCosPitch * fSinYaw);
	const Real fSinPitchSinYaw(fSinPitch * fSinYaw);

	m.m11 = fCosYaw  * fCosRoll;
	m.m12 = -fCosYaw * fSinRoll;
	m.m13 = -fSinYaw;
	m.m21 = -fSinPitchSinYaw * fCosRoll + fCosPitch * fSinRoll;
	m.m22 = fSinPitchSinYaw  * fSinRoll + fCosPitch * fCosRoll;
	m.m23 = -fSinPitch * fCosYaw;
	m.m31 =  fCosPitchSinYaw * fCosRoll + fSinPitch * fSinRoll;
	m.m32 = -fCosPitchSinYaw * fSinRoll + fSinPitch * fCosRoll;
	m.m33 = fCosPitch * fCosYaw;

	m.m14 = m.m24 = m.m34 = m.m41 = m.m42 = m.m43 = static_cast<Real>(0.0);
	m.m44 = static_cast<Real>(1.0);

	Quaternion_T<Real> qr(m);
	w = qr.w;
	x = qr.x;
	y = qr.y;
	z = qr.z;

// 	const Real fSinPitch(sin(pitch*static_cast<Real>(0.5)));
// 	const Real fCosPitch(cos(pitch*static_cast<Real>(0.5)));
// 	const Real fSinYaw(  sin(yaw*  static_cast<Real>(0.5)));
// 	const Real fCosYaw(  cos(yaw*  static_cast<Real>(0.5)));
// 	const Real fSinRoll( sin(roll* static_cast<Real>(0.5)));
// 	const Real fCosRoll( cos(roll* static_cast<Real>(0.5)));
// 	const Real fCosPitchCosYaw(fCosPitch*fCosYaw);
// 	const Real fSinPitchSinYaw(fSinPitch*fSinYaw);
// 	x = fSinRoll * fCosPitchCosYaw     - fCosRoll * fSinPitchSinYaw;
// 	y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
// 	z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
// 	w = fCosRoll * fCosPitchCosYaw     + fSinRoll * fSinPitchSinYaw;
}

//-----------------------------
template<typename Real>
Quaternion_T<Real>::Quaternion_T(Matrix44_T<Real>& rotation)
{
	Real trace = rotation.Trace();

	if (trace>0)
	{
		Real s = static_cast<Real>(0.5)/sqrt(trace);
		x = (rotation.m32-rotation.m23) * s;
		y = (rotation.m13-rotation.m31) * s;
		z = (rotation.m21-rotation.m12) * s;
		w = static_cast<Real>(0.25) / s;
	}
	else
	{
		Real s = 0;
		if ((rotation.m11 > rotation.m22) && (rotation.m11 > rotation.m33))			// if m11 is the biggest
		{
			s = 2*sqrt(1+rotation.m11-rotation.m22-rotation.m33);
			x = static_cast<Real>(0.25) * s;
			y = (rotation.m12 + rotation.m21) / s;
			z = (rotation.m13 + rotation.m31) / s;
			w = (rotation.m23 - rotation.m32) / s;
		}
		else if ((rotation.m22 > rotation.m11) && (rotation.m22 > rotation.m33))	// if m22 is the biggest
		{
			s = 2*sqrt(1-rotation.m11+rotation.m22-rotation.m33);
			x = (rotation.m12 + rotation.m21) / s;
			y = static_cast<Real>(0.25) * s;
			z = (rotation.m23 + rotation.m32) / s;
			w = (rotation.m13 - rotation.m31) / s;
		}
		else if ((rotation.m33 > rotation.m11) && (rotation.m33 > rotation.m22))	// if m33 is the biggest
		{
			s = 2*sqrt(1-rotation.m11-rotation.m22+rotation.m33);
			x = (rotation.m13 + rotation.m31) / s;
			y = (rotation.m23 + rotation.m32) / s;
			z = static_cast<Real>(0.25) * s;
			w = (rotation.m12 - rotation.m21) / s;
		}
	}
}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real> Quaternion_T<Real>::operator+( const Quaternion_T<Real>& qIn)
{ return Quaternion_T<Real>(this->w+qIn.w, this->x+qIn.x, this->y+qIn.y, this->z+qIn.z); }
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real> Quaternion_T<Real>::operator-( const Quaternion_T<Real>& qIn)
{ return Quaternion_T<Real>(this->w-qIn.w, this->x-qIn.x, this->y-qIn.y, this->z-qIn.z); }
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real> Quaternion_T<Real>::operator*( const Quaternion_T<Real>& q)
{
	return Quaternion_T<Real>(	w*q.w - x*q.x - y*q.y - z*q.z,
								w*q.x + x*q.w + y*q.z - z*q.y,
								w*q.y + y*q.w + z*q.x - x*q.z,
								w*q.z + z*q.w + x*q.y - y*q.x);
}
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real> Quaternion_T<Real>::operator/( const Real& f)
{ return Quaternion_T<Real>(w/f, x/f, y/f, z/f); }
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::operator=( const Quaternion_T<Real>& qIn)
{ x = qIn.x; y = qIn.y; z = qIn.z; w = qIn.w; return *this; }
//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::operator += (const Quaternion_T<Real>& q)
{ w += qIn.w; x += qIn.x; y += qIn.y; z += qIn.z; return *this; }
//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::operator -= (const Quaternion_T<Real>& q)
{ w -= qIn.w; x -= qIn.x; y -= qIn.y; z -= qIn.z; return *this; }
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE BOOL Quaternion_T<Real>::operator == (const Quaternion_T<Real>& v)
{ return (x == v.x && y == v.y && z == v.z && w == v.w); }
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE BOOL Quaternion_T<Real>::operator != (const Quaternion_T<Real>& v)
{ return (x != v.x || y != v.y || z != v.z || w != v.w); }


//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real>::operator Matrix44_T<Real>()
{
	Matrix44_T<Real> mat;
	mat.m11 = static_cast<Real>(1.0) - static_cast<Real>(2.0) * (y*y + z*z);
	mat.m12 = static_cast<Real>(2.0) * (x*y - z*w);
	mat.m13 = static_cast<Real>(2.0) * (x*z + y*w);
	mat.m21 = static_cast<Real>(2.0) * (x*y + z*w);
	mat.m22 = static_cast<Real>(1.0) - static_cast<Real>(2.0) * (x*x + z*z);
	mat.m23 = static_cast<Real>(2.0) * (y*z - x*w);
	mat.m31 = static_cast<Real>(2.0) * (x*z - y*w);
	mat.m32 = static_cast<Real>(2.0) * (y*z + x*w);
	mat.m33 = static_cast<Real>(1.0) - static_cast<Real>(2.0) * (x*x + y*y);

	mat.m14 = mat.m24 = mat.m34 = mat.m41 = mat.m42 = mat.m43 = static_cast<Real>(0.0);
	mat.m44 = static_cast<Real>(1.0);

	return mat;
}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE void Quaternion_T<Real>::Set(Real fw, Real fx, Real fy, Real fz)
{ w=fw; x=fx; y=fy; z=fz; }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Real Quaternion_T<Real>::Magnitude()
{ return sqrt(x*x + y*y + z*z + w*w); }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Real Quaternion_T<Real>::SqrMagnitude()
{ return (x*x + y*y + z*z + w*w); }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE BOOL Quaternion_T<Real>::IsNormalized()
{ return (IsOne(x*x + y*y + z*z + w*w)) }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::Conjugate()
{ mImaginary = -mImaginary; return *this;}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::Normalize()
{
	Real mag = this->Magnitude();
	if (mag)	return *this/mag;
	else		return *this;
}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::Inverse()
{
	if (this->IsNormalized())
		this->Conjugate();
	else
	{
		Quaternion_T<Real> q(w, x, y, z);
		q.Conjugate;
		Real mag = this->SqrMagnitude();
		if (mag)	return q/mag;
		else		return *this;
	}
}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::ToEulerAngles()
{
	// TODO: Fix this function => Do Not Use A Matrix !!

	Vector3_T<Real> euler;
	Matrix44_T<Real> mat = *this;
	Real t  = cos(euler.y);
	euler.y = -asin(mat.m13);

	if (RJE::Math::Abs(t) > static_cast<Real>(0.4999))
	{
		euler.x = atan2(-mat.m23/t, mat.m33/t);
		euler.z = atan2(-mat.m12/t, mat.m11/t);
	}
	else 
	{
		euler.x = 0;
		euler.z = atan2(mat.m21, mat.m22);
	}
	return euler;

// 	Vector3_T<Real> euler;
// 	Real unit = x*x + y*y + z*z + w*w; // if normalized is one, otherwise correction factor
// 	Real test = x*y + z*w;
// 	if (test > static_cast<Real>(0.4999*unit)) // singularity at north pole
// 	{
// 		euler.x = static_cast<Real>(2) * atan2(x,w);
// 		euler.y = static_cast<Real>(3.141592653589793238462643383279*0.5);
// 		euler.z = static_cast<Real>(0);
// 	}
// 	else if (test < static_cast<Real>(-0.4999*unit)) // singularity at south pole
// 	{
// 		euler.x = static_cast<Real>(-2) * atan2(x,w);
// 		euler.y = static_cast<Real>(-3.141592653589793238462643383279*0.5);
// 		euler.z = static_cast<Real>(0);
// 	}
// 	else
// 	{
// 		euler.x = atan2( 2*y*w - 2*x*z, x*x - y*y - z*z + w*w );
// 		euler.y = asin( 2*x*y + 2*z*w ); 
// 		euler.z = atan2( 2*x*w - 2*y*z, -x*x + y*y - z*z + w*w );
// 	}
// 	return euler;
}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE void Quaternion_T<Real>::Slerp(OUT Quaternion_T<Real>& qOut, const IN Quaternion_T<Real>& qStart, const IN Quaternion_T<Real>& qEnd, Real factor)
{
	// calculate cosine theta
	Real cosom = qStart.x * qEnd.x + qStart.y * qEnd.y + qStart.z * qEnd.z + qStart.w * qEnd.w;

	// adjust signs (if necessary)
	Quaternion_T end = qEnd;
	if( cosom < static_cast<Real>(0.0))
	{
		cosom = -cosom;
		end.x = -end.x;   // Reverse all signs
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	} 

	// Calculate coefficients
	Real sclp, sclq;
	if( (static_cast<Real>(1.0) - cosom) > static_cast<Real>(0.0001)) // 0.0001 -> some epsilon
	{
		// Standard case (slerp)
		Real omega, sinom;
		omega = acos( cosom); // extract theta from dot product's cos theta
		sinom = sin( omega);
		sclp  = sin( (static_cast<Real>(1.0) - factor) * omega) / sinom;
		sclq  = sin( factor * omega) / sinom;
	} else
	{
		// Very close, do lerp (because it's faster)
		sclp = static_cast<Real>(1.0) - factor;
		sclq = factor;
	}

	qOut.x = sclp * qStart.x + sclq * end.x;
	qOut.y = sclp * qStart.y + sclq * end.y;
	qOut.z = sclp * qStart.z + sclq * end.z;
	qOut.w = sclp * qStart.w + sclq * end.w;
}
