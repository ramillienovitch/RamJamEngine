#include "DX11InputLayout.h"

#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTex[3] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region InputLayouts

ID3D11InputLayout* InputLayouts::PosNormalTex = nullptr;

void InputLayouts::InitAll(ID3D11Device* device)
{
	//
	// PosNormal
	//

	D3DX11_PASS_DESC passDesc;
	Effects::BasicFX->Light1_1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	RJE_CHECK_FOR_SUCCESS(device->CreateInputLayout(	InputLayoutDesc::PosNormalTex,
														3,
														passDesc.pIAInputSignature,
														passDesc.IAInputSignatureSize,
														&PosNormalTex));
}

void InputLayouts::DestroyAll()
{
	RJE_SAFE_RELEASE(PosNormalTex);
}

#pragma endregion