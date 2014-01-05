#include "DX11Drawable.h"

//////////////////////////////////////////////////////////////////////////
BasicEffect*	DX11Drawable::sShader       = nullptr;
ColorEffect*	DX11Drawable::sShader_Gizmo = nullptr;

//////////////////////////////////////////////////////////////////////////
DX11Drawable::DX11Drawable()
{
	mTransform = nullptr;
	//------
	mMesh  = nullptr;
	mGizmo = nullptr;
	//------
	mGizmoColor = Color::White;
}
//-----------
DX11Drawable::~DX11Drawable()
{
	if (mMesh)
	{
		mMesh->Destroy();
		delete mMesh;
		mMesh = nullptr;
	}
	if (mGizmo)
	{
		mGizmo->Destroy();
		delete mGizmo;
		mGizmo = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11Drawable::SetShader( BasicEffect* shader )
{ sShader = shader; }
//-----------------
void DX11Drawable::SetShaderGizmo(ColorEffect* shaderGizmo )
{ sShader_Gizmo = shaderGizmo; }

//////////////////////////////////////////////////////////////////////////
void DX11Drawable::Render(ID3DX11EffectPass* shaderPass)
{
	// Set per object constants.
	sShader->SetWorld(mTransform->WorldMat);
	for (u32 iSubset=0 ; iSubset<mMesh->mSubsetCount; ++iSubset)
	{
		sShader->SetMaterial(mMesh->mMaterial[iSubset]);
		shaderPass->Apply(NULL, mMesh->sDeviceContext);
		mMesh->Render(iSubset);
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11Drawable::RenderGizmo(ID3DX11EffectPass* shaderPass)
{
	sShader_Gizmo->SetColor(mGizmoColor.GetVector4RGBANorm());
	sShader_Gizmo->SetWorld(mTransform->WorldMatNoScale);

	shaderPass->Apply(NULL, mGizmo->sDeviceContext);
	mGizmo->Render(0);
}
