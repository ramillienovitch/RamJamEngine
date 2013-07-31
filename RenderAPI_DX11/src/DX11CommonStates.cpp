#include "DX11CommonStates.h"


//////////////////////////////////////////////////////////////////////////
HRESULT DX11CommonStates::CreateBlendState(ID3D11Device* device,
										   _Out_ ID3D11BlendState** pResult,
										   BOOL alphaToCoverageEnable  = false,
										   BOOL independentBlendEnable = false,
										   BOOL blendEnable = true,
										   D3D11_BLEND srcBlend        = D3D11_BLEND_ONE,
										   D3D11_BLEND srcBlendAlpha   = D3D11_BLEND_ONE,
										   D3D11_BLEND destBlend       = D3D11_BLEND_ZERO,
										   D3D11_BLEND destBlendAlpha  = D3D11_BLEND_ZERO,
										   D3D11_BLEND_OP blendOp      = D3D11_BLEND_OP_ADD,
										   D3D11_BLEND_OP blendOpAlpha = D3D11_BLEND_OP_ADD)
{
	D3D11_BLEND_DESC desc;
	//ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable  = alphaToCoverageEnable;
	desc.IndependentBlendEnable = independentBlendEnable;

	desc.RenderTarget[0].BlendEnable    = blendEnable;
	if (blendEnable)
	{
		desc.RenderTarget[0].SrcBlend       = srcBlend;
		desc.RenderTarget[0].SrcBlendAlpha  = srcBlendAlpha;
		desc.RenderTarget[0].DestBlend      = destBlend;
		desc.RenderTarget[0].DestBlendAlpha = destBlendAlpha;
		desc.RenderTarget[0].BlendOp        = blendOp;
		desc.RenderTarget[0].BlendOpAlpha   = blendOpAlpha;
	}
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = device->CreateBlendState(&desc, pResult);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11CommonStates::CreateDepthStencilState(ID3D11Device* device, bool enable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.DepthEnable    = enable;
	desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable    = false;
	desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	desc.BackFace = desc.FrontFace;

	HRESULT hr = device->CreateDepthStencilState(&desc, pResult);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11CommonStates::CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.CullMode          = cullMode;
	desc.FillMode          = fillMode;
	desc.DepthClipEnable   = true;
	desc.MultisampleEnable = true;
	//desc.AntialiasedLineEnable = false;
	//desc.AntialiasedLineEnable = true;

	HRESULT hr = device->CreateRasterizerState(&desc, pResult);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11CommonStates::CreateSamplerState(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, _Out_ ID3D11SamplerState** pResult)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Filter = filter;

	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;

	desc.MaxAnisotropy = 16;

	desc.MaxLOD         = FLT_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = device->CreateSamplerState(&desc, pResult);

	return hr;
}

//////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------------------
// Blend states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::Opaque(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{
	return CreateBlendState(pDevice, pResult);
}

HRESULT DX11CommonStates::Tranparent(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{
	return CreateBlendState(pDevice, pResult, false, false, true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE);
}

HRESULT DX11CommonStates::AlphaToCoverage(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{
	return CreateBlendState(pDevice, pResult, true, false, false);
}

HRESULT DX11CommonStates::AlphaBlend(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{
	return CreateBlendState(pDevice, pResult, false, false, true, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA);
}

HRESULT DX11CommonStates::Additive(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{
	return CreateBlendState(pDevice, pResult, false, false, true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
}

HRESULT DX11CommonStates::NonPremultiplied(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{
	return CreateBlendState(pDevice, pResult, false, false, false, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA);
}


//--------------------------------------------------------------------------------------
// Depth stencil states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::DepthNone(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{
	return CreateDepthStencilState(pDevice, false, false, pResult);
}

HRESULT DX11CommonStates::DepthDefault(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{
	return CreateDepthStencilState(pDevice, true, true, pResult);
}

HRESULT DX11CommonStates::DepthRead(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{
	return CreateDepthStencilState(pDevice, true, false, pResult);
}


//--------------------------------------------------------------------------------------
// Rasterizer states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::CullNone(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{
	return CreateRasterizerState(pDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult);
}

HRESULT DX11CommonStates::CullClockwise(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{
	return CreateRasterizerState(pDevice, D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult);
}

HRESULT DX11CommonStates::CullCounterClockwise(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{
	return CreateRasterizerState(pDevice, D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult);
}

HRESULT DX11CommonStates::Wireframe(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{
	return CreateRasterizerState(pDevice, D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult);
}


//--------------------------------------------------------------------------------------
// Sampler states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::PointWrap(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{
	return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
}

HRESULT DX11CommonStates::PointClamp(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{
	return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
}

HRESULT DX11CommonStates::LinearWrap(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{
	return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
}

HRESULT DX11CommonStates::LinearClamp(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{
	return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
}

HRESULT DX11CommonStates::AnisotropicWrap(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{
	return CreateSamplerState(pDevice, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
}

HRESULT DX11CommonStates::AnisotropicClamp(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{
	return CreateSamplerState(pDevice, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
}

//////////////////////////////////////////////////////////////////////////

void DX11CommonStates::InitAll(ID3D11Device* device)
{
	RJE_CHECK_FOR_SUCCESS(Wireframe(           device, &sRasterizerState_Wireframe));
	RJE_CHECK_FOR_SUCCESS(CullCounterClockwise(device, &sRasterizerState_Solid));
	sCurrentRasterizerState = sRasterizerState_Solid;

	RJE_CHECK_FOR_SUCCESS(AnisotropicWrap(device, &sSamplerState_Anisotropic));
	RJE_CHECK_FOR_SUCCESS(LinearWrap(     device, &sSamplerState_Linear));
	sCurrentSamplerState = sSamplerState_Anisotropic;

	RJE_CHECK_FOR_SUCCESS(Opaque(         device, &sBlendState_Opaque));
	RJE_CHECK_FOR_SUCCESS(Tranparent(     device, &sBlendState_Transparent));
	RJE_CHECK_FOR_SUCCESS(AlphaToCoverage(device, &sBlendState_AlphaToCoverage));
	sCurrentBlendState = sBlendState_Transparent;
}

void DX11CommonStates::DestroyAll()
{
	RJE_SAFE_RELEASE(sBlendState_Opaque);
	RJE_SAFE_RELEASE(sBlendState_Transparent);
	RJE_SAFE_RELEASE(sBlendState_AlphaToCoverage);

	RJE_SAFE_RELEASE(sSamplerState_Anisotropic);
	RJE_SAFE_RELEASE(sSamplerState_Linear);

	RJE_SAFE_RELEASE(sRasterizerState_Solid);
	RJE_SAFE_RELEASE(sRasterizerState_Wireframe);
}

//////////////////////////////////////////////////////////////////////////

ID3D11RasterizerState*	DX11CommonStates::sRasterizerState_Solid     = nullptr;
ID3D11RasterizerState*	DX11CommonStates::sRasterizerState_Wireframe = nullptr;
ID3D11RasterizerState*	DX11CommonStates::sCurrentRasterizerState    = nullptr;

ID3D11SamplerState*	DX11CommonStates::sSamplerState_Anisotropic = nullptr;
ID3D11SamplerState*	DX11CommonStates::sSamplerState_Linear      = nullptr;
ID3D11SamplerState*	DX11CommonStates::sCurrentSamplerState      = nullptr;

ID3D11BlendState*	DX11CommonStates::sBlendState_Opaque          = nullptr;
ID3D11BlendState*	DX11CommonStates::sBlendState_Transparent     = nullptr;
ID3D11BlendState*	DX11CommonStates::sBlendState_AlphaToCoverage = nullptr;
ID3D11BlendState*	DX11CommonStates::sCurrentBlendState          = nullptr;
