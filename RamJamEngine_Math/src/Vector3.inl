#include "Vector3.h"

//-----------------------------
template <typename Real>
Vector3_T<Real>::Vector3_T(Real val)				{x = y = z = val;}
//-----------------------------
template <typename Real>
Vector3_T<Real>::Vector3_T(Real val1, Real val2, Real val3)	{x = val1; y = val2; z = val3;}
//-----------------------------
template <typename Real>
Vector3_T<Real>::Vector3_T(const Vector3_T & v)	{x = v.x; y = v.y; z = v.z;}
//-----------------------------

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

//-----------------------------
template <typename Real>
Vector3_T<Real> Vector3_T<Real>::operator + (const Vector3_T<Real>& vIn)
{
	Vector3_T vOut = (*this);
	vOut.x += vIn.x;
	vOut.y += vIn.y;
	vOut.z += vIn.z;
	return vOut;
}
//-----------------------------
template <typename Real>
Vector3_T<Real> Vector3_T<Real>::operator - (const Vector3_T<Real>& vIn)
{
	Vector3_T vOut = (*this);
	vOut.x -= vIn.x;
	vOut.y -= vIn.y;
	vOut.z -= vIn.z;
	return vOut;
}
//-----------------------------
template <typename Real>
Vector3_T<Real> Vector3_T<Real>::operator * (const Vector3_T<Real>& vIn)
{
	Vector3_T vOut = (*this);
	vOut.x *= vIn.x;
	vOut.y *= vIn.y;
	vOut.z *= vIn.z;
	return vOut;
}
//-----------------------------
template <typename Real>
Vector3_T<Real>& Vector3_T<Real>::operator = (const Vector3_T<Real>& vIn)
{
	x = vIn.x;
	y = vIn.y;
	z = vIn.z;
	return *this;
}
//-----------------------------
template <typename Real>
BOOL		operator == (const Vector3_T<Real>& v1, const Vector3_T<Real>& v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}
