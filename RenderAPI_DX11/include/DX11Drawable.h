#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Drawable
{
	Transform*		mTransform;
	Material*		mMaterial;
	DX11Mesh		mMesh;
	//------
	BasicEffect*	mShader;
	//-----------------
	void Initialize(BasicEffect* shader);
	void Render(ID3DX11EffectPass* shaderPass);
};
