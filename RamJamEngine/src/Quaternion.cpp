#include "stdafx.h"

Quaternion::Quaternion()									{x=y=z= 0.0f; w = 1.0f;}
Quaternion::Quaternion( f32 qx, f32 qy, f32 qz, f32 qw )	{x=qx; y=qy; z=qz; w=qw;}
Quaternion::Quaternion( const Quaternion& q)				{x=q.x; y=q.y; z=q.z; w=q.w;}
Quaternion::~Quaternion()									{}

const Quaternion Quaternion::identity = Quaternion();

Quaternion	Quaternion::operator+( const Quaternion& qIn)
{
	Quaternion qOut = (*this);
	qOut.x += qIn.x;
	qOut.y += qIn.y;
	qOut.z += qIn.z;
	qOut.w += qIn.w;
	return qOut;
}
Quaternion	Quaternion::operator-( const Quaternion& qIn)
{
	Quaternion qOut = (*this);
	qOut.x -= qIn.x;
	qOut.y -= qIn.y;
	qOut.z -= qIn.z;
	qOut.w -= qIn.w;
	return qOut;
}
Quaternion	Quaternion::operator*( const Quaternion& qIn)
{
	Quaternion qOut = (*this);
	qOut.x *= qIn.x;
	qOut.y *= qIn.y;
	qOut.z *= qIn.z;
	qOut.w *= qIn.w;
	return qOut;
}
Quaternion& Quaternion::operator=( const Quaternion& qIn)
{
	x = qIn.x;
	y = qIn.y;
	z = qIn.z;
	w = qIn.w;
	return *this;
}
BOOL operator == (const Quaternion& v1, const Quaternion& v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
}

