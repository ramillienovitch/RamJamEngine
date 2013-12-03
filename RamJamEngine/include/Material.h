#pragma once

#include "MathHelper.h"

//////////////////////////////////////////////////////////////////////////
struct Material
{
	Material()
	{ 
		ZeroMemory(this, sizeof(this));
	}

	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular; // w = SpecPower
	Vector4 Reflect;
};