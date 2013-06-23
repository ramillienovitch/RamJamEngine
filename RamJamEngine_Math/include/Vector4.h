#pragma once

#include "Types.h"

struct Vector4
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;

	Vector4();
	Vector4(f32);
	Vector4(f32, f32, f32, f32);
	Vector4(const Vector4&);
	~Vector4();

	static const Vector4 zero;
	static const Vector4 one;

	Vector4	operator + (const Vector4&);
	Vector4	operator - (const Vector4&);
	Vector4	operator * (const Vector4&);
	Vector4& operator = (const Vector4&);
	friend BOOL	operator == (const Vector4&, const Vector4&);

	// TODO : add vector "everything" !
};