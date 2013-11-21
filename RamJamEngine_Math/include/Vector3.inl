//////////////////////////////////////////////////////////////////////////

#include "MathHelper.h"

//-----------------------------
template <typename Real>
const Vector3_T<Real> Vector3_T<Real>::zero		= Vector3_T<Real>();
//-----------------------------
template <typename Real>
const Vector3_T<Real> Vector3_T<Real>::one		= Vector3_T<Real>(1.0f);
//-----------------------------
template <typename Real>
const Vector3_T<Real> Vector3_T<Real>::right	= Vector3_T<Real>(1.0f, 0.0f, 0.0f);
//-----------------------------
template <typename Real>
const Vector3_T<Real> Vector3_T<Real>::up		= Vector3_T<Real>(0.0f, 1.0f, 0.0f);
//-----------------------------
template <typename Real>
const Vector3_T<Real> Vector3_T<Real>::forward	= Vector3_T<Real>(0.0f, 0.0f, 1.0f);
//-----------------------------


//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::operator + (const Vector3_T<Real>& v)
{ return Vector3_T<Real>(this->x+v.x, this->y+v.y, this->z+v.z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::operator - (const Vector3_T<Real>& v)
{ return Vector3_T<Real>(this->x-v.x, this->y-v.y, this->z-v.z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::operator - ()
{ return Vector3_T<Real>(-this->x, -this->y, -this->z); }
//---------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::operator / (const Vector3_T<Real>& v)
{ return Vector3_T<Real>(this->x/v.x, this->y/v.y, this->z/v.z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::operator / (const Real& f)
{ return Vector3_T<Real>(this->x/f, this->y/f, this->z/f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::operator * (const Real& f)
{ return Vector3_T<Real>(this->x*f, this->y*f, this->z*f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> operator * (const Real& f, const Vector3_T<Real>&v)
{ return Vector3_T<Real>(v.x*f, v.y*f, v.z*f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> operator * (const int& f, const Vector3_T<Real>&v)
{ return Vector3_T<Real>(v.x*f, v.y*f, v.z*f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator = (const Vector3_T<Real>& vIn)
{ x = vIn.x; y = vIn.y; z = vIn.z; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator = (const DirectX::XMFLOAT3& vIn)
{ x = vIn.x; y = vIn.y; z = vIn.z; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator += (const Vector3_T<Real>& v)
{ x += v.x; y += v.y; z += v.z; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator -= (const Vector3_T<Real>& v)
{ x -= v.x; y -= v.y; z -= v.z; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator /= (const Vector3_T<Real>& v)
{ x /= v.x; y /= v.y; z /= v.z; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator /= (const Real& f)
{ x /= f; y /= f; z /= f; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::operator *= (const Real& f)
{ x *= f; y *= f; z *= f; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE BOOL Vector3_T<Real>::operator == (const Vector3_T<Real>& v)
{ return (x == v.x && y == v.y && z == v.z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE BOOL Vector3_T<Real>::operator != (const Vector3_T<Real>& v)
{ return (x != v.x || y != v.y || z != v.z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
Vector3_T<Real>::operator DirectX::XMFLOAT3 ()
{ return DirectX::XMFLOAT3(x, y, z); }

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE void Vector3_T<Real>::Set (Real fx, Real fy, Real fz)
{ x=fx; y=fy; z=fz; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Vector3_T<Real>::SqrMagnitude()
{ return x*x + y*y + z*z }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Vector3_T<Real>::Magnitude()
{ return sqrt(x*x + y*y + z*z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::Normalize()
{
	Real mag = Magnitude(); 
	if (!RJE::Math::IsZero(mag))
		*this /= mag; 
	return *this;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real>& Vector3_T<Real>::Scale(const Vector3_T& v)
{ x *= v.x; y *= v.y; z *= v.z; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::ProjectToNorm(const Vector3_T& direction) const
{
	Vector3_T<Real> dir = direction;
	dir.Normalize();
	return dir * Vector3_T<Real>::Dot(*this, dir);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE void Vector3_T<Real>::OrthoNormalize(Vector3_T &v1, Vector3_T &v2)
{
	if (v1 == Vector3_T<Real>::zero || v2 == Vector3_T<Real>::zero)
		return;

	v1.Normalize();
	v2 -= v2.ProjectToNorm(v1);
	v2.Normalize();
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::Cross(const Vector3_T& v1, const Vector3_T& v2)
{ return Vector3_T<Real>( v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Vector3_T<Real>::AngleBetween(const Vector3_T& v1, const Vector3_T& v2)
{
	Vector3_T<Real> a = v1;		a.Normalize();
	Vector3_T<Real> b = v2;		b.Normalize();
	
	Real angle = static_cast<Real>(180.0 / RJE::Math::Pi)*acos(Vector3_T<Real>::Dot(a,b));
	return angle;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Vector3_T<Real>::Dot(const Vector3_T& v1, const Vector3_T& v2)
{ return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
