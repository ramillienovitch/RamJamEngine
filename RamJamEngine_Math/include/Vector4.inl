//////////////////////////////////////////////////////////////////////////

//-------------------------
template <typename Real>
const Vector4_T<Real> Vector4_T<Real>::zero	= Vector4_T<Real>();
//-------------------------
template <typename Real>
const Vector4_T<Real> Vector4_T<Real>::one	= Vector4_T<Real>(1.0f);
//-------------------------

//-------------------------
template <typename Real>
Vector4_T<Real> Vector4_T<Real>::operator + (const Vector4_T<Real>& vIn)
{
	Vector4_T vOut = (*this);
	vOut.x += vIn.x;
	vOut.y += vIn.y;
	vOut.z += vIn.z;
	vOut.w += vIn.w;
	return vOut;
}
//-------------------------
template <typename Real>
Vector4_T<Real> Vector4_T<Real>::operator - (const Vector4_T<Real>& vIn)
{
	Vector4_T vOut = (*this);
	vOut.x -= vIn.x;
	vOut.y -= vIn.y;
	vOut.z -= vIn.z;
	vOut.w -= vIn.w;
	return vOut;
}
//-------------------------
template <typename Real>
Vector4_T<Real> Vector4_T<Real>::operator * (const Vector4_T<Real>& vIn)
{
	Vector4_T vOut = (*this);
	vOut.x *= vIn.x;
	vOut.y *= vIn.y;
	vOut.z *= vIn.z;
	vOut.w *= vIn.w;
	return vOut;
}
//-------------------------
template <typename Real>
Vector4_T<Real>& Vector4_T<Real>::operator = (const Vector4_T<Real>& vIn)
{
	x = vIn.x;
	y = vIn.y;
	z = vIn.z;
	w = vIn.w;
	return *this;
}
//-------------------------
template <typename Real>
BOOL operator == (const Vector4_T<Real>& v1, const Vector4_T<Real>& v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
}
