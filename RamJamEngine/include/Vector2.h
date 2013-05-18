#pragma once

#include "stdafx.h"

struct Vector2
{
	f32 x;
	f32 y;

	Vector2();
	Vector2(f32);
	Vector2(f32, f32);
	Vector2(const Vector2&);
	~Vector2();

	static const Vector2 zero;
	static const Vector2 one;

	Vector2	operator + (const Vector2&);
	Vector2	operator - (const Vector2&);
	Vector2	operator * (const Vector2&);
	Vector2& operator = (const Vector2&);
	friend BOOL	operator == (const Vector2&, const Vector2&);

	// TODO : add vector "everything" !
};