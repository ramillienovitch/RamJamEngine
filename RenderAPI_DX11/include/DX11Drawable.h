#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Drawable
{
	DX11Drawable();
	~DX11Drawable();
	//-----------------
	// This is just a pointer to the GameObject Transform
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
	void Render(ID3DX11EffectPass* shaderPass, BOOL bDrawOpaque = true);
	void RenderGizmo(ID3DX11EffectPass* shaderPass);
};
