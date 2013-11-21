//////////////////////////////////////////////////////////////////////////

#include "MathHelper.h"

//----------------------------
template<typename Real>
const Quaternion_T<Real> Quaternion_T<Real>::identity = Quaternion_T<Real>();

//----------------------------
template<typename Real>
Quaternion_T<Real>::Quaternion_T(Real pitch, Real yaw, Real roll)
{
	const Real DegToRad = static_cast<Real>(RJE::Math::Pi / 180.0);
	const Real radX = pitch * DegToRad;
	const Real radY = yaw   * DegToRad;
	const Real radZ = roll  * DegToRad;

	const Real c1=cos(radX*static_cast<Real>(0.5));
	const Real s1=sin(radX*static_cast<Real>(0.5));
	const Real c2=cos(radY*static_cast<Real>(0.5));
	const Real s2=sin(radY*static_cast<Real>(0.5));
	const Real c3=cos(radZ*static_cast<Real>(0.5));
	const Real s3=sin(radZ*static_cast<Real>(0.5));
	const Real c3c2=c3*c2;
	const Real s3s2=s3*s2;
	const Real s3c2=s3*c2;
	const Real c3s2=c3*s2;
	w = c3c2*c1-s3s2*s1;
	x = c3c2*s1+s3s2*c1;
	y = c3s2*c1-s3c2*s1;
	z = s3c2*c1+c3s2*s1;
}

//-----------------------------
template<typename Real>
Quaternion_T<Real>::Quaternion_T(Vector3_T<Real> axis, Real degrees)
{
	axis.Normalize();

	Real rad = degrees * static_cast<Real>(RJE::Math::Pi / 180.0);
	const Real sin_a = sin( rad / 2 );
	const Real cos_a = cos( rad / 2 );
	x    = axis.x * sin_a;
	y    = axis.y * sin_a;
	z    = axis.z * sin_a;
	w    = cos_a;
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
{ return (RJE::Math::IsOne(x*x + y*y + z*z + w*w)); }

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
	{
		this->Conjugate();
		return *this;
	}
	else
	{
		Quaternion_T<Real> q(w, x, y, z);
		q.Conjugate();
		Real mag = this->SqrMagnitude();
		if (mag)	return q/mag;
		else		return *this;
	}
}

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Quaternion_T<Real>& Quaternion_T<Real>::LookAt(Vector3_T<Real>& lookAt, Vector3_T<Real>& upDirection)
{
	Vector3_T<Real> forward = lookAt;
	Vector3_T<Real> up      = upDirection;
	forward.Normalize();
	up.Normalize();
	Vector3_T<Real>::OrthoNormalize(forward, up);

	Vector3_T<Real> right = Vector3_T<Real>::Cross(up, forward);

	Quaternion_T<Real> ret;
	ret.w = sqrt(static_cast<Real>(1.0) + right.x + up.y + forward.z) * static_cast<Real>(0.5);
	float w4_recip = -static_cast<Real>(1.0) / (static_cast<Real>(4.0) * ret.w);
	ret.x = (up.z - forward.y)    * w4_recip;
	ret.y = (forward.x - right.z) * w4_recip;
	ret.z = (right.y - up.x)      * w4_recip;

	*this = ret;
	return *this;
}


//---------------------------------------------------------------------------
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::Rotate (IN const Real& x, IN  const Real& y, IN const Real& z)
{
	Quaternion_T<Real> q(*this);
	Quaternion_T<Real> q2(	static_cast<Real>(0.0),
							static_cast<Real>(RJE::Math::Pi/180)*x,
							static_cast<Real>(RJE::Math::Pi/180)*y,
							static_cast<Real>(RJE::Math::Pi/180)*z);
	Quaternion_T<Real> qinv = q;
	q.Conjugate();

	q = q*q2*qinv;
	return Vector3_T<Real>(q.x,q.y,q.z) * static_cast<Real>(180/RJE::Math::Pi);
}
//---------------------------------------------------------------------------
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::Rotate (IN const Vector3_T<Real>& v)
{ return this->Rotate(v.x, v.y, v.z); }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Matrix44_T<Real> Quaternion_T<Real>::ToMatrix()
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
FORCEINLINE Quaternion_T<Real>::operator Matrix44_T<Real>()
{ return this->ToMatrix(); }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::ToEulerAngles()
{
	Vector3_T<Real> euler;
	Real One = static_cast<Real>(1);
	euler.y=RJE::Math::Clamp(static_cast<Real>(2*(w*y+x*z)), -One, One);
	if(euler.y==One||euler.y==-One)
	{
		euler.x=2*atan2(x,w);
		euler.y*=static_cast<Real>(RJE::Math::Pi_Half);
		euler.z=0;
	}
	euler.x=atan2(2*(w*x-y*z), 1-2*(x*x+y*y));
	euler.y=asin(euler.y);
	euler.z=atan2(2*(w*z-x*y), 1-2*(y*y+z*z));

	euler *= static_cast<Real>(180.0 / RJE::Math::Pi);
	return euler;
}

//--------------------------------------------------------------------
	// TODO
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::GetRightVector() const
{ Quaternion_T<Real> q(*this); return q.Rotate(Vector3_T<Real>(1,0,0)); }

//--------------------------------------------------------------------
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::GetUpVector() const
{ Quaternion_T<Real> q(*this); return q.Rotate(Vector3_T<Real>(0,1,0)); }

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE Vector3_T<Real> Quaternion_T<Real>::GetForwardVector() const
{ Quaternion_T<Real> q(*this); return q.Rotate(Vector3_T<Real>(0,0,1)); }

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
