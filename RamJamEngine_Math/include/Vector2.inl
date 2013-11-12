//-------------
template <typename Real>
const Vector2_T<Real> Vector2_T<Real>::zero	= Vector2_T<Real>();
//-------------
template <typename Real>
const Vector2_T<Real> Vector2_T<Real>::one	= Vector2_T<Real>(1.0f);


template <typename Real>
Vector2_T<Real> Vector2_T<Real>::operator + (const Vector2_T<Real>& vIn)
{
	Vector2_T vOut = (*this);
	vOut.x += vIn.x;
	vOut.y += vIn.y;
	return vOut;
}
template <typename Real>
Vector2_T<Real> Vector2_T<Real>::operator - (const Vector2_T<Real>& vIn)
{
	Vector2_T vOut = (*this);
	vOut.x -= vIn.x;
	vOut.y -= vIn.y;
	return vOut;
}
template <typename Real>
Vector2_T<Real> Vector2_T<Real>::operator * (const Vector2_T<Real>& vIn)
{
	Vector2_T vOut = (*this);
	vOut.x *= vIn.x;
	vOut.y *= vIn.y;
	return vOut;
}
template <typename Real>
Vector2_T<Real>& Vector2_T<Real>::operator = (const Vector2_T<Real>& vIn)
{
	x = vIn.x;
	y = vIn.y;
	return *this;
}
template <typename Real>
BOOL operator == (const Vector2_T<Real>& v1, const Vector2_T<Real>& v2)
{
	return (v1.x == v2.x && v1.y == v2.y);
}