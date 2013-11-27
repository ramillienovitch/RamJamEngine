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
FORCEINLINE Real Vector3_T<Real>::Dot(const Vector3_T& v1, const Vector3_T& v2)
{ return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

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
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::ReflectRay(const Vector3_T<Real>& incident, const Vector3_T<Real>& normal)
{
	Vector3_T<Real> out;

	Real s = static_cast<Real>(2.0f) * ( incident.x*normal.x + incident.y*normal.y + incident.z*normal.z );

	out.x = incident.x - s * normal.x;
	out.y = incident.y - s * normal.y;
	out.z = incident.z - s * normal.z;

	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::RandUnitSphere()
{
	Real One = static_cast<1.0f>;
	// Keep trying until we get a point on/in the sphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		Vector3_T<Real> v = Vector3_T<Real>(RJE::Math::Rand(-One, One), RJE::Math::Rand(-One, One), RJE::Math::Rand(-One, One));

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if( v.SqrMagnitude > One )
			continue;

		return v.Normalize();
	}
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Vector3_T<Real>::RandUnitHemisphere(Vector3_T<Real> normal)
{
	Real One  = static_cast<1.0f>;
	Real Zero = static_cast<0.0f>;

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		Vector3_T<Real> v = RandUnitSphere();

		// Ignore points outside the hemisphere.
		if( Dot(normal, v) < Zero )
			continue;

		return v.Normalize();
	}
}
//----------------------------------------------------------------------
