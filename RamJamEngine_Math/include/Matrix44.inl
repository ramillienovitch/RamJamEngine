//////////////////////////////////////////////////////////////////////////

#include "MathHelper.h"
#include "Debug.h"

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
FORCEINLINE Vector4_T<Real> Matrix44_T<Real>::operator * ( const Vector4_T<Real>& pVector )
{
	return Vector4_T<Real> (	m11 * pVector.w + m12 * pVector.x + m13 * pVector.y + m14 * pVector.z,
								m21 * pVector.w + m22 * pVector.x + m23 * pVector.y + m24 * pVector.z,
								m31 * pVector.w + m32 * pVector.x + m33 * pVector.y + m34 * pVector.z,
								m41 * pVector.w + m42 * pVector.x + m43 * pVector.y + m44 * pVector.z);
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
		const Real nan = RJE::Math::NaN<Real>();
		*this = Matrix44_T<Real>(	nan,nan,nan,nan,
									nan,nan,nan,nan,
									nan,nan,nan,nan,
									nan,nan,nan,nan);
		return *this;
	}

	const Real invdet = static_cast<Real>(1.0) / det;

	Matrix44_T<Real> res;
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
FORCEINLINE Matrix44_T<Real>& Matrix44_T<Real>::InverseTranspose()
{
	// Inverse-transpose is just applied to normals.  So zero out 
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	m41 = m42 = m43 = 0.0f;
	m44 = 1.0f;

	Matrix44_T<Real> out = *(this);

	out.Inverse();
	return out.Transpose();
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Vector3_T<Real> Matrix44_T<Real>::TransformVector(Vector3_T<Real> v)
{ return (*this) * v; }

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
	Matrix44_T<Real> m(	vRows[0].x,vRows[1].x,vRows[2].x, 0,
						vRows[0].y,vRows[1].y,vRows[2].y, 0,
						vRows[0].z,vRows[1].z,vRows[2].z, 0,
						0         , 0        , 0        , 1);

	// and generate the rotation quaternion from it
	rotation = Quaternion_T<Real>(m);
	rotation.Conjugate();
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
//-------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Translation(Real x, Real y, Real z)
{
	Matrix44_T<Real> out;
	out.m41 = x;
	out.m42 = y;
	out.m43 = z;
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
//-------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Scaling(Real x, Real y, Real z)
{
	Matrix44_T<Real> out;
	out.m11 = x;
	out.m22 = y;
	out.m33 = z;
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

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::LookAt(Vector3_T<Real> pos, Vector3_T<Real> dir, Vector3_T<Real> up)
{
	Vector3_T<Real> R2 = dir;
	R2.Normalize();

	Vector3_T<Real> R0 = Vector3_T<Real>::Cross(up, R2);
	R0.Normalize();

	Vector3_T<Real> R1 = Vector3_T<Real>::Cross(R2, R0);
	Vector3_T<Real> negPos = -pos;

	Real D0 = Vector3_T<Real>::Dot(R0, negPos);
	Real D1 = Vector3_T<Real>::Dot(R1, negPos);
	Real D2 = Vector3_T<Real>::Dot(R2, negPos);

	Real One  = static_cast<Real>(1.0);
	Real Zero = static_cast<Real>(0.0);

	Matrix44_T<Real> out;
	out.m11 = R0.x;		out.m12 = R0.y;		out.m13 = R0.z;		out.m14 = D0;
	out.m21 = R1.x;		out.m22 = R1.y;		out.m23 = R1.z;		out.m24 = D1;
	out.m31 = R2.x;		out.m32 = R2.y;		out.m33 = R2.z;		out.m34 = D2;
	out.m41 = Zero;		out.m42 = Zero;		out.m43 = Zero;		out.m44 = One;
	return out.Transpose();
}
//----------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::LookAt(Vector3_T<Real> pos, Vector3_T<Real> dir)
{ return LookAt(pos, dir, Vector3_T<Real>(0,1,0)); }
//----------------------------------------------
template <typename Real>
FORCEINLINE void Matrix44_T<Real>::LookAt(Matrix44_T<Real>& mOut, Vector3_T<Real> pos, Vector3_T<Real> dir)
{ mOut = LookAt(pos, dir, Vector3_T<Real>(0,1,0)); }
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::PerspectiveFov(Real Fov, Real AspectRatio, Real NearZ, Real FarZ)
{
	Real fov = static_cast<Real>(0.5f)* Fov;
	RJE_ASSERT(!RJE::Math::IsZero(fov));
	
	Real One  = static_cast<Real>(1.0);
	Real Zero = static_cast<Real>(0.0);

	Real Height = One/tan(fov);
	Real Width  = Height / AspectRatio;
	Real fRange = FarZ / (FarZ-NearZ);

	Matrix44_T<Real> out;
	out.m11 = Width;	out.m12 = Zero;		out.m13 = Zero;				out.m14 = Zero;
	out.m21 = Zero;		out.m22 = Height;	out.m23 = Zero;				out.m24 = Zero;
	out.m31 = Zero;		out.m32 = Zero;		out.m33 = fRange;			out.m34 = One;
	out.m41 = Zero;		out.m42 = Zero;		out.m43 = -fRange * NearZ;	out.m44 = Zero;
	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Orthographic(Real ViewWidth, Real ViewHeight, Real NearZ, Real FarZ)
{
	RJE_ASSERT(!RJE::Math::IsZero(ViewWidth));
	RJE_ASSERT(!RJE::Math::IsZero(ViewHeight));

	Real One    = static_cast<Real>(1.0);
	Real Zero   = static_cast<Real>(0.0);
	Real fRange = One / (FarZ-NearZ);

	Matrix44_T<Real> out;
	out.m11 = 2/ViewWidth;	out.m12 = Zero;				out.m13 = Zero;				out.m14 = Zero;
	out.m21 = Zero;			out.m22 = 2/ViewHeight;		out.m23 = Zero;				out.m24 = Zero;
	out.m31 = Zero;			out.m32 = Zero;				out.m33 = fRange;			out.m34 = Zero;
	out.m41 = Zero;			out.m42 = Zero;				out.m43 = -fRange * NearZ;	out.m44 = One;
	return out;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE void Matrix44_T<Real>::Orthographic(Matrix44_T<Real>& mOut, Real ViewWidth, Real ViewHeight, Real NearZ, Real FarZ)
{
	RJE_ASSERT(!RJE::Math::IsZero(ViewWidth));
	RJE_ASSERT(!RJE::Math::IsZero(ViewHeight));

	Real One    = static_cast<Real>(1.0);
	Real Zero   = static_cast<Real>(0.0);
	Real fRange = One / (FarZ-NearZ);

	mOut.m11 = 2/ViewWidth;		mOut.m12 = Zero;				mOut.m13 = Zero;				mOut.m14 = Zero;
	mOut.m21 = Zero;			mOut.m22 = 2/ViewHeight;		mOut.m23 = Zero;				mOut.m24 = Zero;
	mOut.m31 = Zero;			mOut.m32 = Zero;				mOut.m33 = fRange;				mOut.m34 = Zero;
	mOut.m41 = Zero;			mOut.m42 = Zero;				mOut.m43 = -fRange * NearZ;		mOut.m44 = One;
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Reflection(Vector4_T<Real> planeEquation)
{
	const Vector4_T<Real> NegativeTwo = Vector4_T<Real>(-2.0f, -2.0f, -2.0f, 0.0f);

	planeEquation.Normalize();
	Vector4_T<Real> S = planeEquation;
	planeEquation.Scale(NegativeTwo);
	Vector4_T<Real> A = Vector4_T<Real>(planeEquation.w, planeEquation.w, planeEquation.w, planeEquation.w);
	Vector4_T<Real> B = Vector4_T<Real>(planeEquation.x, planeEquation.x, planeEquation.x, planeEquation.x);
	Vector4_T<Real> C = Vector4_T<Real>(planeEquation.y, planeEquation.y, planeEquation.y, planeEquation.y);
	Vector4_T<Real> D = Vector4_T<Real>(planeEquation.z, planeEquation.z, planeEquation.z, planeEquation.z);

	Matrix44_T<Real> out;
	Vector4_T<Real> row1 = (A*S) + Vector4_T<Real>(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4_T<Real> row2 = (B*S) + Vector4_T<Real>(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4_T<Real> row3 = (C*S) + Vector4_T<Real>(0.0f, 0.0f, 1.0f, 0.0f);
	Vector4_T<Real> row4 = (D*S) + Vector4_T<Real>(0.0f, 0.0f, 0.0f, 1.0f);
	out.m11 = row1.w;	out.m12 = row1.x;	out.m13 = row1.y;	out.m14 = row1.z;
	out.m21 = row2.w;	out.m22 = row2.x;	out.m23 = row2.y;	out.m24 = row2.z;
	out.m31 = row3.w;	out.m32 = row3.x;	out.m33 = row3.y;	out.m34 = row3.z;
	out.m41 = row4.w;	out.m42 = row4.x;	out.m43 = row4.y;	out.m44 = row4.z;
	return out;
}
//-------------
template <typename Real>
FORCEINLINE Matrix44_T<Real> Matrix44_T<Real>::Reflection(Real w, Real x, Real y, Real z)
{ return Reflection(Vector4_T<Real>(w,x,y,z)); }
//----------------------------------------------------------------------
