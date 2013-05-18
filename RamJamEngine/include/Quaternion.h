#pragma once

#include "stdafx.h"

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
#if USE_QUATERNION_DX11 == 1
		struct
		{
			DirectX::XMVECTOR mVectorial;
		}
#else
		struct
		{
			Vector4 mVectorial;
		};
#endif
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
