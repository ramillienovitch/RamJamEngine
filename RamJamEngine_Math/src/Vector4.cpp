#include "Vector4.h"

Vector4::Vector4()											{x = y = z = w = 0;}
Vector4::Vector4(f32 val)									{x = y = z = w = val;}
Vector4::Vector4(f32 val1, f32 val2, f32 val3, f32 val4)	{x = val1; y = val2; z = val3; w = val4;}
Vector4::Vector4(const Vector4 & v)							{x = v.x; y = v.y; z = v.z; w = v.w;}
Vector4::~Vector4()											{}

const Vector4 Vector4::zero		= Vector4();
const Vector4 Vector4::one		= Vector4(1.0f);

Vector4		Vector4::operator + (const Vector4& vIn)
{
	Vector4 vOut = (*this);
	vOut.x += vIn.x;
	vOut.y += vIn.y;
	vOut.z += vIn.z;
	vOut.w += vIn.w;
	return vOut;
}
Vector4		Vector4::operator - (const Vector4& vIn)
{
	Vector4 vOut = (*this);
	vOut.x -= vIn.x;
	vOut.y -= vIn.y;
	vOut.z -= vIn.z;
	vOut.w -= vIn.w;
	return vOut;
}
Vector4		Vector4::operator * (const Vector4& vIn)
{
	Vector4 vOut = (*this);
	vOut.x *= vIn.x;
	vOut.y *= vIn.y;
	vOut.z *= vIn.z;
	vOut.w *= vIn.w;
	return vOut;
}
Vector4&	Vector4::operator = (const Vector4& vIn)
{
	x = vIn.x;
	y = vIn.y;
	z = vIn.z;
	w = vIn.w;
	return *this;
}
BOOL		operator == (const Vector4& v1, const Vector4& v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
}
