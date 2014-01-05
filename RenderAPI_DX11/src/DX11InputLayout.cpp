#include "DX11InputLayout.h"

#pragma region InputLayoutDesc

// const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTex[3] = 
// {
// 	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
// 	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
// 	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
// };

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTanTex[4] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosColor[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region InputLayouts

//ID3D11InputLayout* DX11InputLayouts::PosNormalTex    = nullptr;
ID3D11InputLayout* DX11InputLayouts::PosNormalTanTex = nullptr;
ID3D11InputLayout* DX11InputLayouts::PosColor        = nullptr;

void DX11InputLayouts::InitAll(ID3D11Device* device)
{
	//////////////////////////////////////////////////////////////////////////
	// PosNormalTex
// 	DX11Effects::BasicFX->BasicTech->GetPassByIndex(0)->GetDesc(&basicPassDesc);
// 	RJE_CHECK_FOR_SUCCESS(device->CreateInputLayout(	InputLayoutDesc::PosNormalTex,
// 														3,
// 														basicPassDesc.pIAInputSignature,
// 														basicPassDesc.IAInputSignatureSize,
// 														&PosNormalTex));

	//////////////////////////////////////////////////////////////////////////
	// PosNormalTanTex
	D3DX11_PASS_DESC basicPassDesc;
	DX11Effects::BasicFX->BasicTech->GetPassByIndex(0)->GetDesc(&basicPassDesc);
	RJE_CHECK_FOR_SUCCESS(device->CreateInputLayout(	InputLayoutDesc::PosNormalTanTex,
														4,
														basicPassDesc.pIAInputSignature,
														basicPassDesc.IAInputSignatureSize,
														&PosNormalTanTex));

	//////////////////////////////////////////////////////////////////////////
	// PosColor
	D3DX11_PASS_DESC colorPassDesc;
	DX11Effects::ColorFX->ColorTech->GetPassByIndex(0)->GetDesc(&colorPassDesc);
	RJE_CHECK_FOR_SUCCESS(device->CreateInputLayout(	InputLayoutDesc::PosColor,
														2,
														colorPassDesc.pIAInputSignature,
														colorPassDesc.IAInputSignatureSize,
														&PosColor));
}

void DX11InputLayouts::DestroyAll()
{
	//RJE_SAFE_RELEASE(PosNormalTex);
	RJE_SAFE_RELEASE(PosNormalTanTex);
	RJE_SAFE_RELEASE(PosColor);
}

#pragma endregion