#include "Vector3.h"

Vector3::Vector3()								{x = y = z = 0;}
Vector3::Vector3(f32 val)						{x = y = z = val;}
Vector3::Vector3(f32 val1, f32 val2, f32 val3)	{x = val1; y = val2; z = val3;}
Vector3::Vector3(const Vector3 & v)				{x = v.x; y = v.y; z = v.z;}
Vector3::~Vector3()								{}

const Vector3 Vector3::zero		= Vector3();
const Vector3 Vector3::one		= Vector3(1.0f);
const Vector3 Vector3::right	= Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::up		= Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::forward	= Vector3(0.0f, 0.0f, 1.0f);

Vector3		Vector3::operator + (const Vector3& vIn)
{
	Vector3 vOut = (*this);
	vOut.x += vIn.x;
	vOut.y += vIn.y;
	vOut.z += vIn.z;
	return vOut;
}
Vector3		Vector3::operator - (const Vector3& vIn)
{
	Vector3 vOut = (*this);
	vOut.x -= vIn.x;
	vOut.y -= vIn.y;
	vOut.z -= vIn.z;
	return vOut;
}
Vector3		Vector3::operator * (const Vector3& vIn)
{
	Vector3 vOut = (*this);
	vOut.x *= vIn.x;
	vOut.y *= vIn.y;
	vOut.z *= vIn.z;
	return vOut;
}
Vector3&	Vector3::operator = (const Vector3& vIn)
{
	x = vIn.x;
	y = vIn.y;
	z = vIn.z;
	return *this;
}
BOOL		operator == (const Vector3& v1, const Vector3& v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}
