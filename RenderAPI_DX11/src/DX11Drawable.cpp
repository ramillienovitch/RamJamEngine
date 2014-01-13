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
void DX11Drawable::Render(ID3DX11EffectPass* shaderPass, BOOL bDrawOpaque /*= true*/)
{
	// Set per object constants.
	RJE_CHECK_FOR_SUCCESS(sShader->SetWorld(mTransform->WorldMat));
	if (mMesh->mSubsetCount==1)
	{
		// opaque test
		if (mMesh->mMaterial[0]->mIsOpaque == bDrawOpaque)
		{
			RJE_CHECK_FOR_SUCCESS(sShader->SetMaterial(mMesh->mMaterial[0].get()));
			RJE_CHECK_FOR_SUCCESS(shaderPass->Apply(NULL, mMesh->sDeviceContext));
			mMesh->Render(-1);
		}
	}
	else
	{
		for (u32 iSubset=0 ; iSubset<mMesh->mSubsetCount; ++iSubset)
		{
			// opaque test
			if (mMesh->mMaterial[iSubset]->mIsOpaque == bDrawOpaque)
			//if (mMesh->mMaterial[0]->mIsOpaque == bDrawOpaque)
			{
				RJE_CHECK_FOR_SUCCESS(sShader->SetMaterial(mMesh->mMaterial[iSubset].get()));
				//RJE_CHECK_FOR_SUCCESS(sShader->SetMaterial(mMesh->mMaterial[0].get()));
				RJE_CHECK_FOR_SUCCESS(shaderPass->Apply(NULL, mMesh->sDeviceContext));
				mMesh->Render(iSubset);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11Drawable::RenderGizmo(ID3DX11EffectPass* shaderPass)
{
	RJE_CHECK_FOR_SUCCESS(sShader_Gizmo->SetColor(mGizmoColor.GetVector4RGBANorm()));
	RJE_CHECK_FOR_SUCCESS(sShader_Gizmo->SetWorld(mTransform->WorldMatNoScale));

	RJE_CHECK_FOR_SUCCESS(shaderPass->Apply(NULL, mGizmo->sDeviceContext));
	mGizmo->Render(-1);
}
