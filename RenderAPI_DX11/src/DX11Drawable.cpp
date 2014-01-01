#include "DX11Drawable.h"

//////////////////////////////////////////////////////////////////////////
void DX11Drawable::Initialize( BasicEffect* shader )
{
	mShader = shader;
}

//////////////////////////////////////////////////////////////////////////
void DX11Drawable::Render(ID3DX11EffectPass* shaderPass)
{
	// Set per object constants.
	mShader->SetWorld(mTransform->WorldMatrix());
	mShader->SetMaterial(*mMaterial);

	shaderPass->Apply(NULL, mMesh.mDeviceContext);

	mMesh.Render();
}
