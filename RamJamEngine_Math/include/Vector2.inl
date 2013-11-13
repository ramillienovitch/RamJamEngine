//-------------
template <typename Real>
const Vector2_T<Real> Vector2_T<Real>::zero	= Vector2_T<Real>();
//-------------
template <typename Real>
const Vector2_T<Real> Vector2_T<Real>::one	= Vector2_T<Real>(1.0f);


//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> Vector2_T<Real>::operator + (const Vector2_T<Real>& v)
{ return Vector2_T<Real>(this->x+v.x, this->y+v.y); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> Vector2_T<Real>::operator - (const Vector2_T<Real>& v)
{ return Vector2_T<Real>(this->x-v.x, this->y-v.y); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> Vector2_T<Real>::operator - ()
{ return Vector2_T<Real>(-this->x, -this->y); }
//---------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> Vector2_T<Real>::operator / (const Vector2_T<Real>& v)
{ return Vector2_T<Real>(this->x/v.x, this->y/v.y); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> Vector2_T<Real>::operator / (const Real& f)
{ RJE_ASSERT(f!=0);	return Vector2_T<Real>(this->x/f, this->y/f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> Vector2_T<Real>::operator * (const Real& f)
{ return Vector2_T<Real>(this->x*f, this->y*f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> operator * (const Real& f, const Vector2_T<Real>&v)
{ return Vector2_T<Real>(v.x*f, v.y*f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real> operator * (const int& f, const Vector2_T<Real>&v)
{ return Vector2_T<Real>(v.x*f, v.y*f); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator = (const Vector2_T<Real>& vIn)
{ x = vIn.x; y = vIn.y; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator = (const DirectX::XMFLOAT2& vIn)
{ x = vIn.x; y = vIn.y; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator += (const Vector2_T<Real>& v)
{ x += v.x; y += v.y; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator -= (const Vector2_T<Real>& v)
{ x -= v.x; y -= v.y; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator /= (const Vector2_T<Real>& v)
{ x /= v.x; y /= v.y; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator /= (const Real& f)
{ x /= f; y /= f; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::operator *= (const Real& f)
{ x *= f; y *= f; return *this; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE BOOL Vector2_T<Real>::operator == (const Vector2_T<Real>& v)
{ return (x == v.x && y == v.y); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE BOOL Vector2_T<Real>::operator != (const Vector2_T<Real>& v)
{ return (x != v.x || y != v.y); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
Vector2_T<Real>::operator DirectX::XMFLOAT2 ()
{ return DirectX::XMFLOAT2(x, y); }

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE void Vector2_T<Real>::Set (Real fx, Real fy)
{ x=fx; y=fy; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Vector2_T<Real>::SqrMagnitude()
{ return x*x + y*y }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Vector2_T<Real>::Magnitude()
{ return sqrt(x*x + y*y); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::Normalize()
{
	Real mag = Magnitude(); 
	if (!RJE::Math::IsZero(mag))
		*this /= mag; 
	return *this;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector2_T<Real>& Vector2_T<Real>::Scale(const Vector2_T& v)
{ x *= v.x; y *= v.y; return *this; }
//----------------------------------------------------------------------
