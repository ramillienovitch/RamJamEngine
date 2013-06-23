#pragma once

#include "Types.h"
#include "Vector3.h"
#include "Vector4.h"

struct Quaternion
{
	union
	{
		struct
		{
			f32 w;
			f32 x;
			f32 y;
			f32 z;
		};
		struct
		{
			f32		mReal;
			Vector3	mImaginary;
		};
		struct
		{
			Vector4 mVectorial;
		};
	};

	Quaternion();
	Quaternion(f32, f32, f32, f32);
	Quaternion(const Quaternion&);
	~Quaternion();

	static const Quaternion identity;

	Quaternion	operator + (const Quaternion&);
	Quaternion	operator - (const Quaternion&);
	Quaternion	operator * (const Quaternion&);
	Quaternion& operator = (const Quaternion&);
	friend BOOL	operator == (const Quaternion&, const Quaternion&);
};

// TODO : FINSIH HIM !!!
