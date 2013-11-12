//////////////////////////////////////////////////////////////////////////

//------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T()
{
	m12=m13=m14=m21=m23=m24=m31=m32=m34=m41=m42=m43 = 0.0f;
	m11=m22=m33=m44 = 1.0f;
}

//------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T( const Matrix44_T& matIn )
{
	m11 = matIn.m11;	m12 = matIn.m12;	m13 = matIn.m13;	m14 = matIn.m14;
	m21 = matIn.m21;	m22 = matIn.m22;	m23 = matIn.m23;	m24 = matIn.m24;
	m31 = matIn.m31;	m32 = matIn.m32;	m33 = matIn.m33;	m34 = matIn.m34;
	m41 = matIn.m41;	m42 = matIn.m42;	m43 = matIn.m43;	m44 = matIn.m44;
}

//------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T( Real _m11, Real _m12, Real _m13, Real _m14, Real _m21, Real _m22, Real _m23, Real _m24, Real _m31, Real _m32, Real _m33, Real _m34, Real _m41, Real _m42, Real _m43, Real _m44 )
{
	m11 = _m11;		m12 = _m12;		m13 = _m13;		m14 = _m14;
	m21 = _m21;		m22 = _m22;		m23 = _m23;		m24 = _m24;
	m31 = _m31;		m32 = _m32;		m33 = _m33;		m34 = _m34;
	m41 = _m41;		m42 = _m42;		m43 = _m43;		m44 = _m44;
}

//------------------------
template <typename Real>
const Matrix44_T<Real> Matrix44_T<Real>::identity = Matrix44_T<Real>();

//------------------------
template <typename Real>
Matrix44_T<Real> Matrix44_T<Real>::operator+( const Matrix44_T<Real>& matIn )
{
	Matrix44_T matOut = (*this);
	matOut.m11 += matIn.m11;	matOut.m12 += matIn.m12;	matOut.m13 += matIn.m13;	matOut.m14 += matIn.m14;
	matOut.m21 += matIn.m21;	matOut.m22 += matIn.m22;	matOut.m23 += matIn.m23;	matOut.m24 += matIn.m24;
	matOut.m31 += matIn.m31;	matOut.m32 += matIn.m32;	matOut.m33 += matIn.m33;	matOut.m34 += matIn.m34;
	matOut.m41 += matIn.m41;	matOut.m42 += matIn.m42;	matOut.m43 += matIn.m43;	matOut.m44 += matIn.m44;
	return matOut;
}

//------------------------
template <typename Real>
Matrix44_T<Real> Matrix44_T<Real>::operator-( const Matrix44_T<Real>& matIn )
{
	Matrix44_T matOut = (*this);
	matOut.m11 -= matIn.m11;	matOut.m12 -= matIn.m12;	matOut.m13 -= matIn.m13;	matOut.m14 -= matIn.m14;
	matOut.m21 -= matIn.m21;	matOut.m22 -= matIn.m22;	matOut.m23 -= matIn.m23;	matOut.m24 -= matIn.m24;
	matOut.m31 -= matIn.m31;	matOut.m32 -= matIn.m32;	matOut.m33 -= matIn.m33;	matOut.m34 -= matIn.m34;
	matOut.m41 -= matIn.m41;	matOut.m42 -= matIn.m42;	matOut.m43 -= matIn.m43;	matOut.m44 -= matIn.m44;
	return matOut;
}

//------------------------
template <typename Real>
Matrix44_T<Real> Matrix44_T<Real>::operator*( const Matrix44_T<Real>& matIn )
{
	*this = Matrix44_T (	matIn.m11 * m11 + matIn.m21 * m12 + matIn.m31 * m13 + matIn.m41 * m14,
							matIn.m12 * m11 + matIn.m22 * m12 + matIn.m32 * m13 + matIn.m42 * m14,
							matIn.m13 * m11 + matIn.m23 * m12 + matIn.m33 * m13 + matIn.m43 * m14,
							matIn.m14 * m11 + matIn.m24 * m12 + matIn.m34 * m13 + matIn.m44 * m14,
							matIn.m11 * m21 + matIn.m21 * m22 + matIn.m31 * m23 + matIn.m41 * m24,
							matIn.m12 * m21 + matIn.m22 * m22 + matIn.m32 * m23 + matIn.m42 * m24,
							matIn.m13 * m21 + matIn.m23 * m22 + matIn.m33 * m23 + matIn.m43 * m24,
							matIn.m14 * m21 + matIn.m24 * m22 + matIn.m34 * m23 + matIn.m44 * m24,
							matIn.m11 * m31 + matIn.m21 * m32 + matIn.m31 * m33 + matIn.m41 * m34,
							matIn.m12 * m31 + matIn.m22 * m32 + matIn.m32 * m33 + matIn.m42 * m34,
							matIn.m13 * m31 + matIn.m23 * m32 + matIn.m33 * m33 + matIn.m43 * m34,
							matIn.m14 * m31 + matIn.m24 * m32 + matIn.m34 * m33 + matIn.m44 * m34,
							matIn.m11 * m41 + matIn.m21 * m42 + matIn.m31 * m43 + matIn.m41 * m44,
							matIn.m12 * m41 + matIn.m22 * m42 + matIn.m32 * m43 + matIn.m42 * m44,
							matIn.m13 * m41 + matIn.m23 * m42 + matIn.m33 * m43 + matIn.m43 * m44,
							matIn.m14 * m41 + matIn.m24 * m42 + matIn.m34 * m43 + matIn.m44 * m44);
	return *this;
}

//------------------------
template <typename Real>
Matrix44_T<Real>& Matrix44_T<Real>::operator=( const Matrix44_T<Real>& matIn )
{
	m11 = matIn.m11;	m12 = matIn.m12;	m13 = matIn.m13;	m14 = matIn.m14;
	m21 = matIn.m21;	m22 = matIn.m22;	m23 = matIn.m23;	m24 = matIn.m24;
	m31 = matIn.m31;	m32 = matIn.m32;	m33 = matIn.m33;	m34 = matIn.m34;
	m41 = matIn.m41;	m42 = matIn.m42;	m43 = matIn.m43;	m44 = matIn.m44;
	return *this;
}

//------------------------
template <typename Real>
Matrix44_T<Real>::operator DirectX::XMMATRIX ()
{
	DirectX::XMMATRIX M(	m11, m12, m13, m14,
							m21, m22, m23, m24,
							m31, m32, m33, m34,
							m41, m42, m43, m44);
	return M;
}

//------------------------
template <typename Real>
Matrix44_T<Real>& Matrix44_T<Real>::operator=( const DirectX::XMMATRIX& matIn)
{
	m11 = matIn.r[0].m128_f32[0];	m12 = matIn.r[0].m128_f32[1];	m13 = matIn.r[0].m128_f32[2];	m14 = matIn.r[0].m128_f32[3];
	m21 = matIn.r[1].m128_f32[0];	m22 = matIn.r[1].m128_f32[1];	m23 = matIn.r[1].m128_f32[2];	m24 = matIn.r[1].m128_f32[3];
	m31 = matIn.r[2].m128_f32[0];	m32 = matIn.r[2].m128_f32[1];	m33 = matIn.r[2].m128_f32[2];	m34 = matIn.r[2].m128_f32[3];
	m41 = matIn.r[3].m128_f32[0];	m42 = matIn.r[3].m128_f32[1];	m43 = matIn.r[3].m128_f32[2];	m44 = matIn.r[3].m128_f32[3];
	return *this;
}

//------------------------
template <typename Real>
Real Matrix44_T<Real>::Trace()
{  return m11+m22+m33+m44; }
