//////////////////////////////////////////////////////////////////////////

#include "MathHelper.h"

//------------------------
template <typename Real>
const Matrix44_T<Real> Matrix44_T<Real>::identity = Matrix44_T<Real>();


//----------------------------------------------------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T()
{
	m12=m13=m14=m21=m23=m24=m31=m32=m34=m41=m42=m43 = 0.0f;
	m11=m22=m33=m44 = 1.0f;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T( const Matrix44_T& matIn )
{
	m11 = matIn.m11;	m12 = matIn.m12;	m13 = matIn.m13;	m14 = matIn.m14;
	m21 = matIn.m21;	m22 = matIn.m22;	m23 = matIn.m23;	m24 = matIn.m24;
	m31 = matIn.m31;	m32 = matIn.m32;	m33 = matIn.m33;	m34 = matIn.m34;
	m41 = matIn.m41;	m42 = matIn.m42;	m43 = matIn.m43;	m44 = matIn.m44;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T( const DirectX::XMMATRIX& matIn )
{
	m11 = matIn.r[0].m128_f32[0];	m12 = matIn.r[0].m128_f32[1];	m13 = matIn.r[0].m128_f32[2];	m14 = matIn.r[0].m128_f32[3];
	m21 = matIn.r[1].m128_f32[0];	m22 = matIn.r[1].m128_f32[1];	m23 = matIn.r[1].m128_f32[2];	m24 = matIn.r[1].m128_f32[3];
	m31 = matIn.r[2].m128_f32[0];	m32 = matIn.r[2].m128_f32[1];	m33 = matIn.r[2].m128_f32[2];	m34 = matIn.r[2].m128_f32[3];
	m41 = matIn.r[3].m128_f32[0];	m42 = matIn.r[3].m128_f32[1];	m43 = matIn.r[3].m128_f32[2];	m44 = matIn.r[3].m128_f32[3];
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
Matrix44_T<Real>::Matrix44_T( Real _m11, Real _m12, Real _m13, Real _m14, Real _m21, Real _m22, Real _m23, Real _m24, Real _m31, Real _m32, Real _m33, Real _m34, Real _m41, Real _m42, Real _m43, Real _m44 )
{
	m11 = _m11;		m12 = _m12;		m13 = _m13;		m14 = _m14;
	m21 = _m21;		m22 = _m22;		m23 = _m23;		m24 = _m24;
	m31 = _m31;		m32 = _m32;		m33 = _m33;		m34 = _m34;
	m41 = _m41;		m42 = _m42;		m43 = _m43;		m44 = _m44;
}
//----------------------------------------------------------------------


//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::operator+( const Matrix44_T<Real>& matIn )
{
	return Matrix44_T<Real>(
		this->m11+matIn.m11, this->m12+matIn.m12, this->m13+matIn.m13, this->m14+matIn.m14,
		this->m21+matIn.m21, this->m22+matIn.m22, this->m23+matIn.m23, this->m24+matIn.m24,
		this->m31+matIn.m31, this->m32+matIn.m32, this->m33+matIn.m33, this->m34+matIn.m34,
		this->m41+matIn.m41, this->m42+matIn.m42, this->m43+matIn.m43, this->m44+matIn.m44);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::operator-( const Matrix44_T<Real>& matIn )
{
	return Matrix44_T<Real>(
		this->m11-matIn.m11, this->m12-matIn.m12, this->m13-matIn.m13, this->m14-matIn.m14,
		this->m21-matIn.m21, this->m22-matIn.m22, this->m23-matIn.m23, this->m24-matIn.m24,
		this->m31-matIn.m31, this->m32-matIn.m32, this->m33-matIn.m33, this->m34-matIn.m34,
		this->m41-matIn.m41, this->m42-matIn.m42, this->m43-matIn.m43, this->m44-matIn.m44);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::operator*( const Matrix44_T<Real>& matIn )
{
	return Matrix44_T<Real> (	matIn.m11 * m11 + matIn.m21 * m12 + matIn.m31 * m13 + matIn.m41 * m14,
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
}
//---------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Matrix44_T<Real>::operator * ( const Vector3_T<Real>& pVector )
{
	return Vector3_T<Real> (	m11 * pVector.x + m12 * pVector.y + m13 * pVector.z + m14,
								m21 * pVector.x + m22 * pVector.y + m23 * pVector.z + m24,
								m31 * pVector.x + m32 * pVector.y + m33 * pVector.z + m34);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::operator*=( const Matrix44_T<Real>& matIn )
{ *this = *this*matIn; return *this; }
//---------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::operator=( const Matrix44_T<Real>& matIn )
{
	m11 = matIn.m11;	m12 = matIn.m12;	m13 = matIn.m13;	m14 = matIn.m14;
	m21 = matIn.m21;	m22 = matIn.m22;	m23 = matIn.m23;	m24 = matIn.m24;
	m31 = matIn.m31;	m32 = matIn.m32;	m33 = matIn.m33;	m34 = matIn.m34;
	m41 = matIn.m41;	m42 = matIn.m42;	m43 = matIn.m43;	m44 = matIn.m44;
	return *this;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::operator=( const DirectX::XMMATRIX& matIn)
{
	m11 = matIn.r[0].m128_f32[0];	m12 = matIn.r[0].m128_f32[1];	m13 = matIn.r[0].m128_f32[2];	m14 = matIn.r[0].m128_f32[3];
	m21 = matIn.r[1].m128_f32[0];	m22 = matIn.r[1].m128_f32[1];	m23 = matIn.r[1].m128_f32[2];	m24 = matIn.r[1].m128_f32[3];
	m31 = matIn.r[2].m128_f32[0];	m32 = matIn.r[2].m128_f32[1];	m33 = matIn.r[2].m128_f32[2];	m34 = matIn.r[2].m128_f32[3];
	m41 = matIn.r[3].m128_f32[0];	m42 = matIn.r[3].m128_f32[1];	m43 = matIn.r[3].m128_f32[2];	m44 = matIn.r[3].m128_f32[3];
	return *this;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>::operator DirectX::XMMATRIX ()
{
	DirectX::XMMATRIX M(	m11, m12, m13, m14,
							m21, m22, m23, m24,
							m31, m32, m33, m34,
							m41, m42, m43, m44);
	return M;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE BOOL Matrix44_T<Real>::operator == (const Matrix44_T<Real>& m)
{
	return (m11 == m.m11 &&	m12 == m.m12 &&	m13 == m.m13 &&	m14 == m.m14 &&
			m21 == m.m21 &&	m22 == m.m22 &&	m23 == m.m23 &&	m24 == m.m24 &&
			m31 == m.m31 &&	m32 == m.m32 &&	m33 == m.m33 &&	m34 == m.m34 &&
			m41 == m.m41 &&	m42 == m.m42 &&	m43 == m.m43 &&	m44 == m.m44 &&);
}
//----------------------------------------------------------------------
template<typename Real>
FORCEINLINE BOOL Matrix44_T<Real>::operator != (const Matrix44_T<Real>& m)
{
	return (m11 != m.m11 ||	m12 != m.m12 ||	m13 != m.m13 ||	m14 != m.m14 ||
			m21 != m.m21 ||	m22 != m.m22 ||	m23 != m.m23 ||	m24 != m.m24 ||
			m31 != m.m31 ||	m32 != m.m32 ||	m33 != m.m33 ||	m34 != m.m34 ||
			m41 != m.m41 ||	m42 != m.m42 ||	m43 != m.m43 ||	m44 != m.m44 ||);
}
//----------------------------------------------------------------------


//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Matrix44_T<Real>::Trace()
{  return m11+m22+m33+m44; }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Real Matrix44_T<Real>::Determinant()
{
	return	  m11*m22*m33*m44 - m11*m22*m34*m43 + m11*m23*m34*m42 - m11*m23*m32*m44 
			+ m11*m24*m32*m43 - m11*m24*m33*m42 - m12*m23*m34*m41 + m12*m23*m31*m44 
			- m12*m24*m31*m43 + m12*m24*m33*m41 - m12*m21*m33*m44 + m12*m21*m34*m43 
			+ m13*m24*m31*m42 - m13*m24*m32*m41 + m13*m21*m32*m44 - m13*m21*m34*m42 
			+ m13*m22*m34*m41 - m13*m22*m31*m44 - m14*m21*m32*m43 + m14*m21*m33*m42
			- m14*m22*m33*m41 + m14*m22*m31*m43 - m14*m23*m31*m42 + m14*m23*m32*m41;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::Transpose()
{
	std::swap(m21, m12);
	std::swap(m32, m23);
	std::swap(m31, m13);
	std::swap(m41, m14);
	std::swap(m42, m24);
	std::swap(m43, m34);
	return *this;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::Inverse()
{
	const Real det = Determinant();

	if(det == static_cast<Real>(0.0))
	{
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense but it is easy to debug for the
		// programmer.
		const Real nan = RJE::RJE_NAN(Real);
		*this = Matrix44_T<Real>(	nan,nan,nan,nan,
									nan,nan,nan,nan,
									nan,nan,nan,nan,
									nan,nan,nan,nan);
		return *this;
	}

	const Real invdet = static_cast<Real>(1.0) / det;

	aiMatrix4x4t<Real> res;
	res.m11 =  invdet * (m22 * (m33 * m44 - m34 * m43) + m23 * (m34 * m42 - m32 * m44) + m24 * (m32 * m43 - m33 * m42));
	res.m12 = -invdet * (m12 * (m33 * m44 - m34 * m43) + m13 * (m34 * m42 - m32 * m44) + m14 * (m32 * m43 - m33 * m42));
	res.m13 =  invdet * (m12 * (m23 * m44 - m24 * m43) + m13 * (m24 * m42 - m22 * m44) + m14 * (m22 * m43 - m23 * m42));
	res.m14 = -invdet * (m12 * (m23 * m34 - m24 * m33) + m13 * (m24 * m32 - m22 * m34) + m14 * (m22 * m33 - m23 * m32));
	res.m21 = -invdet * (m21 * (m33 * m44 - m34 * m43) + m23 * (m34 * m41 - m31 * m44) + m24 * (m31 * m43 - m33 * m41));
	res.m22 =  invdet * (m11 * (m33 * m44 - m34 * m43) + m13 * (m34 * m41 - m31 * m44) + m14 * (m31 * m43 - m33 * m41));
	res.m23 = -invdet * (m11 * (m23 * m44 - m24 * m43) + m13 * (m24 * m41 - m21 * m44) + m14 * (m21 * m43 - m23 * m41));
	res.m24 =  invdet * (m11 * (m23 * m34 - m24 * m33) + m13 * (m24 * m31 - m21 * m34) + m14 * (m21 * m33 - m23 * m31));
	res.m31 =  invdet * (m21 * (m32 * m44 - m34 * m42) + m22 * (m34 * m41 - m31 * m44) + m24 * (m31 * m42 - m32 * m41));
	res.m32 = -invdet * (m11 * (m32 * m44 - m34 * m42) + m12 * (m34 * m41 - m31 * m44) + m14 * (m31 * m42 - m32 * m41));
	res.m33 =  invdet * (m11 * (m22 * m44 - m24 * m42) + m12 * (m24 * m41 - m21 * m44) + m14 * (m21 * m42 - m22 * m41));
	res.m34 = -invdet * (m11 * (m22 * m34 - m24 * m32) + m12 * (m24 * m31 - m21 * m34) + m14 * (m21 * m32 - m22 * m31));
	res.m41 = -invdet * (m21 * (m32 * m43 - m33 * m42) + m22 * (m33 * m41 - m31 * m43) + m23 * (m31 * m42 - m32 * m41));
	res.m42 =  invdet * (m11 * (m32 * m43 - m33 * m42) + m12 * (m33 * m41 - m31 * m43) + m13 * (m31 * m42 - m32 * m41));
	res.m43 = -invdet * (m11 * (m22 * m43 - m23 * m42) + m12 * (m23 * m41 - m21 * m43) + m13 * (m21 * m42 - m22 * m41));
	res.m44 =  invdet * (m11 * (m22 * m33 - m23 * m32) + m12 * (m23 * m31 - m21 * m33) + m13 * (m21 * m32 - m22 * m31));
	*this = res;

	return *this;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE BOOL Matrix44_T<Real>::IsIdentity() const
{
	// Use a small epsilon to solve floating-point inaccuracies
	const static Real epsilon = 10e-3f;

	return (a2 <= epsilon && a2 >= -epsilon &&
			a3 <= epsilon && a3 >= -epsilon &&
			a4 <= epsilon && a4 >= -epsilon &&
			b1 <= epsilon && b1 >= -epsilon &&
			b3 <= epsilon && b3 >= -epsilon &&
			b4 <= epsilon && b4 >= -epsilon &&
			c1 <= epsilon && c1 >= -epsilon &&
			c2 <= epsilon && c2 >= -epsilon &&
			c4 <= epsilon && c4 >= -epsilon &&
			d1 <= epsilon && d1 >= -epsilon &&
			d2 <= epsilon && d2 >= -epsilon &&
			d3 <= epsilon && d3 >= -epsilon &&
			a1 <= 1.f+epsilon && a1 >= 1.f-epsilon &&
			b2 <= 1.f+epsilon && b2 >= 1.f-epsilon &&
			c3 <= 1.f+epsilon && c3 >= 1.f-epsilon &&
			d4 <= 1.f+epsilon && d4 >= 1.f-epsilon);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE void Matrix44_T<Real>::Decompose(OUT Vector3_T<Real>& position, OUT Vector3_T<Real>& scale, OUT Quaternion_T<Real>& rotation)
{
	// extract translation
	position.x = this->m41;
	position.y = this->m42;
	position.z = this->m43;

	// extract the rows of the matrix
	Vector3_T<Real> vRows[3] =
	{
		Vector3_T<Real>(this->m11, this->m12, this->m13),
		Vector3_T<Real>(this->m21, this->m22, this->m23),
		Vector3_T<Real>(this->m31, this->m32, this->m33)
	};

	// extract the scaling factors
	scale.x = vRows[0].Magnitude();
	scale.y = vRows[1].Magnitude();
	scale.z = vRows[2].Magnitude();

	// and the sign of the scaling
	if (Determinant() < 0)
	{
		scale.x = -scale.x;
		scale.y = -scale.y;
		scale.z = -scale.z;
	}

	// and remove all scaling from the matrix
	if(scale.x)		vRows[0] /= scale.x;
	if(scale.y)		vRows[1] /= scale.y;
	if(scale.z)		vRows[2] /= scale.z;

	// build a 3x3 rotation matrix
	Vector3_T<Real> m(	vRows[0].x,vRows[1].x,vRows[2].x,
						vRows[0].y,vRows[1].y,vRows[2].y,
						vRows[0].z,vRows[1].z,vRows[2].z);

	// and generate the rotation quaternion from it
	rotation = Quaternion_T<Real>(m);
}
//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE void Matrix44_T<Real>::DecomposeNoScale(OUT Vector3_T<Real>& position, OUT Quaternion_T<Real>& rotation)
{
	// extract translation
	position.x = this->m41;
	position.y = this->m42;
	position.z = this->m43;

	// extract rotation
	rotation = Quaternion_T<Real>(*this);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::FromEulerAngles(Matrix44_T<Real>& m, Real x, Real y, Real z)
{
	Real DegToRad = static_cast<Real>(3.1415926535f / 180.0f);
	Real cr = cos( x * DegToRad );
	Real sr = sin( x * DegToRad );
	Real cp = cos( y * DegToRad );
	Real sp = sin( y * DegToRad );
	Real cy = cos( z * DegToRad );
	Real sy = sin( z * DegToRad );

	m.m11 = cp*cy ;
	m.m12 = cp*sy;
	m.m13 = -sp ;

	Real srsp = sr*sp;
	Real crsp = cr*sp;

	m.m21 = srsp*cy-cr*sy;
	m.m22 = srsp*sy+cr*cy;
	m.m23 = sr*cp;

	m.m31 = crsp*cy+sr*sy;
	m.m32 = crsp*sy-sr*cy;
	m.m33 = cr*cp ;

	m.m14 = m.m24 = m.m34 = m.m41 = m.m42 = m.m43 = 0.0f;
	m.m44 = 1.0f;

	return m;
}
//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::FromEulerAngles(Matrix44_T<Real>& m, Vector3_T<Real> euler)
{ return FromEulerAngles(m, euler.x, euler.y, euler.z); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Translation(Vector3_T<Real> position)
{
	Matrix44_T<Real> out;
	out.m41 = position.x;
	out.m42 = position.y;
	out.m43 = position.z;
	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Scaling(Vector3_T<Real> scale)
{
	Matrix44_T<Real> out;
	out.m11 = scale.x;
	out.m22 = scale.y;
	out.m33 = scale.z;
	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::RotationX(Real degrees)
{
	//---------------------------------
	//      |  1  0       0       0 |
	//  M = |  0  cos(A) -sin(A)  0 |
	//      |  0  sin(A)  cos(A)  0 |
	//      |  0  0       0       1 |
	//---------------------------------
	Real rad = degrees * static_cast<Real>(3.1415926535f / 180.0f);
	Matrix44_T<Real> out;
	out.m22 = out.m33 = cos(rad);
	out.m23 = -(out.m32 = sin(rad));
	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::RotationY(Real degrees)
{
	//---------------------------------
	//     |  cos(A)  0   sin(A)  0 |
	// M = |  0       1   0       0 |
	//     | -sin(A)  0   cos(A)  0 |
	//     |  0       0   0       1 |
	//---------------------------------
	Real rad = degrees * static_cast<Real>(3.1415926535f / 180.0f);
	Matrix44_T<Real> out;
	out.m11 = out.m33 = cos(rad);
	out.m31 = -(out.m13 = sin(rad));
	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::RotationZ(Real degrees)
{
	//---------------------------------
	//     |  cos(A)  -sin(A)   0   0 |
	// M = |  sin(A)   cos(A)   0   0 |
	//     |  0        0        1   0 |
	//     |  0        0        0   1 |
	//---------------------------------
	Real rad = degrees * static_cast<Real>(3.1415926535f / 180.0f);
	Matrix44_T<Real> out;
	out.m11 = out.m22 = cos(rad);
	out.m12 = -(out.m21 = sin(rad));
	return out;
}
//----------------------------------------------------------------------
