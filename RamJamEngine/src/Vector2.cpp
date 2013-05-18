#include "stdafx.h"

Vector2::Vector2()						{x = y = 0;}
Vector2::Vector2(f32 val)				{x = y = val;}
Vector2::Vector2(f32 val1, f32 val2)	{x = val1; y = val2;}
Vector2::Vector2(const Vector2 & v)		{x = v.x; y = v.y;}
Vector2::~Vector2()						{}

const Vector2 Vector2::zero	= Vector2();
const Vector2 Vector2::one	= Vector2(1.0f);

Vector2		Vector2::operator + (const Vector2& vIn)
{
	Vector2 vOut = (*this);
	vOut.x += vIn.x;
	vOut.y += vIn.y;
	return vOut;
}
Vector2		Vector2::operator - (const Vector2& vIn)
{
	Vector2 vOut = (*this);
	vOut.x -= vIn.x;
	vOut.y -= vIn.y;
	return vOut;
}
Vector2		Vector2::operator * (const Vector2& vIn)
{
	Vector2 vOut = (*this);
	vOut.x *= vIn.x;
	vOut.y *= vIn.y;
	return vOut;
}
Vector2&	Vector2::operator = (const Vector2& vIn)
{
	x = vIn.x;
	y = vIn.y;
	return *this;
}
BOOL		operator == (const Vector2& v1, const Vector2& v2)
{
	return (v1.x == v2.x && v1.y == v2.y);
}
