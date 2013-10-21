#pragma once

#include "Types.h"

struct Vector3
{
	f32 x;
	f32 y;
	f32 z;

	Vector3();
	Vector3(f32);
	Vector3(f32, f32, f32);
	Vector3(const Vector3&);
	~Vector3();

	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;

	Vector3	operator + (const Vector3&);
	Vector3	operator - (const Vector3&);
	Vector3	operator * (const Vector3&);
	Vector3& operator = (const Vector3&);
	friend BOOL	operator == (const Vector3&, const Vector3&);

	// TODO: add vector "everything" !
};