#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Drawable
{
	DX11Drawable();
	~DX11Drawable();
	//-----------------
	// These are just pointers to the GameObject Trf and Material
	Transform*		mTransform;
	//------
	DX11Mesh*		mMesh;
	DX11Mesh*		mGizmo;
	//------
	Color			mGizmoColor;
	//------
	static BasicEffect*		sShader;
	static ColorEffect*		sShader_Gizmo;
	//-----------------
	static void SetShader(BasicEffect* shader);
	static void SetShaderGizmo(ColorEffect* shader);
	//------
	void Render(ID3DX11EffectPass* shaderPass);
	void RenderGizmo(ID3DX11EffectPass* shaderPass);
};
