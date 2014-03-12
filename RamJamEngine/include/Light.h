#pragma once

#include "MathHelper.h"

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

//////////////////////////////////////////////////////////////////////////
RJE_ALIGNOF(16)
struct DirectionalLight
{
	DirectionalLight()
	{ 
		ZeroMemory(this, sizeof(this));
	}

	Vector4 Color;
	Vector4 Direction;
// 	float pad1;
// 	float pad2;
};

//////////////////////////////////////////////////////////////////////////
struct PointLight
{
	PointLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	Vector3 Position;
	Vector3 Color;
	float   Range;
	float   Intensity;
};

//////////////////////////////////////////////////////////////////////////
struct SpotLight
{
	SpotLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	Vector3 Color;
	Vector3 Position;
	Vector3 Direction;
	float   Spot;
	float   Range;
	float   Intensity;
};