#pragma once

#include "MathHelper.h"

#if (RJE_GRAPHIC_API == DIRECTX_11)
#	include <d3d11.h>
typedef ID3D11ShaderResourceView ShaderResource;
#else
#	define ShaderResource
#endif

//////////////////////////////////////////////////////////////////////////
struct Texture
{
	ShaderResource*		mTexture;
	Vector2				mTiling;
	Vector2				mOffset;
	f32					mRotationAngleInDegrees;
};
