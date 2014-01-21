#include "DX11CommonStates.h"


//////////////////////////////////////////////////////////////////////////
HRESULT DX11CommonStates::CreateBlendState(ID3D11Device* device,
										   _Out_ ID3D11BlendState** pResult,
										   BOOL alphaToCoverageEnable  = false,
										   BOOL independentBlendEnable = false,
										   BOOL blendEnable            = true,
										   D3D11_BLEND srcBlend        = D3D11_BLEND_ONE,
										   D3D11_BLEND srcBlendAlpha   = D3D11_BLEND_ONE,
										   D3D11_BLEND destBlend       = D3D11_BLEND_ZERO,
										   D3D11_BLEND destBlendAlpha  = D3D11_BLEND_ZERO,
										   D3D11_BLEND_OP blendOp      = D3D11_BLEND_OP_ADD,
										   D3D11_BLEND_OP blendOpAlpha = D3D11_BLEND_OP_ADD,
										   UINT8 renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

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
HRESULT DX11CommonStates::CreateDepthStencilState(ID3D11Device* device,
												  _Out_ ID3D11DepthStencilState** pResult,
												  bool depthEnable      = true,
												  bool depthWriteEnable = true,
												  bool stencilEnable    = false,
												  D3D11_COMPARISON_FUNC depthFunc     = D3D11_COMPARISON_LESS_EQUAL,
												  D3D11_COMPARISON_FUNC stencilFunc   = D3D11_COMPARISON_ALWAYS,
												  D3D11_STENCIL_OP backFaceStencilPassOp      = D3D11_STENCIL_OP_KEEP,
												  D3D11_STENCIL_OP backFaceStencilFailOp      = D3D11_STENCIL_OP_KEEP,
												  D3D11_STENCIL_OP backFaceStencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
												  D3D11_STENCIL_OP frontFaceStencilPassOp      = (D3D11_STENCIL_OP)-1,
												  D3D11_STENCIL_OP frontFaceStencilFailOp      = (D3D11_STENCIL_OP)-1,
												  D3D11_STENCIL_OP frontFaceStencilDepthFailOp = (D3D11_STENCIL_OP)-1)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.DepthEnable    = depthEnable;
	desc.DepthWriteMask = depthWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc      = depthFunc;

	desc.StencilEnable    = stencilEnable;
	desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.BackFace.StencilFunc        = stencilFunc;
	desc.BackFace.StencilPassOp      = backFaceStencilPassOp;
	desc.BackFace.StencilFailOp      = backFaceStencilFailOp;
	desc.BackFace.StencilDepthFailOp = backFaceStencilDepthFailOp;

	if (frontFaceStencilPassOp == -1)
		desc.FrontFace = desc.BackFace;
	else
	{
		desc.FrontFace.StencilFunc        = stencilFunc;
		desc.FrontFace.StencilPassOp      = frontFaceStencilPassOp;
		desc.FrontFace.StencilFailOp      = frontFaceStencilFailOp;
		desc.FrontFace.StencilDepthFailOp = frontFaceStencilDepthFailOp;
	}

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
{ return CreateBlendState(pDevice, pResult); }

HRESULT DX11CommonStates::Tranparent(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{ return CreateBlendState(pDevice, pResult, false, false, true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_ONE); }

HRESULT DX11CommonStates::AlphaToCoverage(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{ return CreateBlendState(pDevice, pResult, true, false, false); }

HRESULT DX11CommonStates::AlphaBlend(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{ return CreateBlendState(pDevice, pResult, false, false, true, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA); }

HRESULT DX11CommonStates::BlendFactor(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{ return CreateBlendState(pDevice, pResult, false, false, true, D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR); }

HRESULT DX11CommonStates::NonPremultiplied(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{ return CreateBlendState(pDevice, pResult, false, false, false, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA); }

HRESULT DX11CommonStates::NoRenderTargetWrites(ID3D11Device* pDevice, ID3D11BlendState** pResult)
{ return CreateBlendState(pDevice, pResult, false, false, false, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_BLEND_OP_ADD, 0); }


//--------------------------------------------------------------------------------------
// Depth stencil states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::DepthNone(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult, false, false); }

HRESULT DX11CommonStates::DepthDefault(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult); }

HRESULT DX11CommonStates::DepthDisable(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult, false, true, true, D3D11_COMPARISON_LESS, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR); }

HRESULT DX11CommonStates::DepthRead(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult, true, false); }

HRESULT DX11CommonStates::MarkStencil(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult, true, false, true, D3D11_COMPARISON_LESS, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_REPLACE); }

HRESULT DX11CommonStates::DrawStenciled(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult, true, true, true, D3D11_COMPARISON_LESS, D3D11_COMPARISON_EQUAL); }

HRESULT DX11CommonStates::NoDoubleBlend(ID3D11Device* pDevice, ID3D11DepthStencilState** pResult)
{ return CreateDepthStencilState(pDevice, pResult, true, true, true, D3D11_COMPARISON_LESS_EQUAL, D3D11_COMPARISON_EQUAL, D3D11_STENCIL_OP_INCR); }


//--------------------------------------------------------------------------------------
// Rasterizer states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::CullNone(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{ return CreateRasterizerState(pDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult); }

HRESULT DX11CommonStates::CullClockwise(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{ return CreateRasterizerState(pDevice, D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult); }

HRESULT DX11CommonStates::CullCounterClockwise(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{ return CreateRasterizerState(pDevice, D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult); }

HRESULT DX11CommonStates::Wireframe(ID3D11Device* pDevice, ID3D11RasterizerState** pResult)
{ return CreateRasterizerState(pDevice, D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult); }


//--------------------------------------------------------------------------------------
// Sampler states
//--------------------------------------------------------------------------------------
HRESULT DX11CommonStates::PointWrap(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{ return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, pResult); }

HRESULT DX11CommonStates::PointClamp(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{ return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pResult); }

HRESULT DX11CommonStates::LinearWrap(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{ return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, pResult); }

HRESULT DX11CommonStates::LinearClamp(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{ return CreateSamplerState(pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult); }

HRESULT DX11CommonStates::AnisotropicWrap(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{ return CreateSamplerState(pDevice, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, pResult); }

HRESULT DX11CommonStates::AnisotropicClamp(ID3D11Device* pDevice, ID3D11SamplerState** pResult)
{ return CreateSamplerState(pDevice, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, pResult); }

//////////////////////////////////////////////////////////////////////////

void DX11CommonStates::InitAll(ID3D11Device* device)
{
	RJE_CHECK_FOR_SUCCESS(Wireframe(           device, &sRasterizerState_Wireframe));
	RJE_CHECK_FOR_SUCCESS(CullClockwise(       device, &sRasterizerState_CullClockwise));
	RJE_CHECK_FOR_SUCCESS(CullCounterClockwise(device, &sRasterizerState_Solid));
	RJE_CHECK_FOR_SUCCESS(CullNone(            device, &sRasterizerState_CullNone));
	sCurrentRasterizerState = sRasterizerState_Solid;

	RJE_CHECK_FOR_SUCCESS(AnisotropicWrap(device, &sSamplerState_Anisotropic));
	RJE_CHECK_FOR_SUCCESS(LinearWrap(     device, &sSamplerState_Linear));
	sCurrentSamplerState = sSamplerState_Anisotropic;

	RJE_CHECK_FOR_SUCCESS(Opaque(              device, &sBlendState_Opaque));
	RJE_CHECK_FOR_SUCCESS(Tranparent(          device, &sBlendState_Transparent));
	RJE_CHECK_FOR_SUCCESS(AlphaToCoverage(     device, &sBlendState_AlphaToCoverage));
	RJE_CHECK_FOR_SUCCESS(BlendFactor(         device, &sBlendState_BlendFactor));
	RJE_CHECK_FOR_SUCCESS(NoRenderTargetWrites(device, &sBlendState_NoRenderTargetWrites));
	sCurrentBlendState = sBlendState_AlphaToCoverage;

	RJE_CHECK_FOR_SUCCESS(DepthDefault( device, &sDepthStencilState_Default));
	RJE_CHECK_FOR_SUCCESS(DepthDisable( device, &sDepthStencilState_DepthDisable));
	RJE_CHECK_FOR_SUCCESS(DepthRead(    device, &sDepthStencilState_DepthRead));
	RJE_CHECK_FOR_SUCCESS(MarkStencil(  device, &sDepthStencilState_MarkStencil));
	RJE_CHECK_FOR_SUCCESS(DrawStenciled(device, &sDepthStencilState_DrawStenciled));
	RJE_CHECK_FOR_SUCCESS(NoDoubleBlend(device, &sDepthStencilState_NoDoubleBlend));
	sCurrentDepthStencilState = sDepthStencilState_Default;
}

void DX11CommonStates::DestroyAll()
{
	RJE_SAFE_RELEASE(sDepthStencilState_Default);
	RJE_SAFE_RELEASE(sDepthStencilState_DepthDisable);
	RJE_SAFE_RELEASE(sDepthStencilState_DepthRead);
	RJE_SAFE_RELEASE(sDepthStencilState_MarkStencil);
	RJE_SAFE_RELEASE(sDepthStencilState_DrawStenciled);
	RJE_SAFE_RELEASE(sDepthStencilState_NoDoubleBlend);

	RJE_SAFE_RELEASE(sBlendState_Opaque);
	RJE_SAFE_RELEASE(sBlendState_Transparent);
	RJE_SAFE_RELEASE(sBlendState_AlphaToCoverage);
	RJE_SAFE_RELEASE(sBlendState_BlendFactor);
	RJE_SAFE_RELEASE(sBlendState_NoRenderTargetWrites);

	RJE_SAFE_RELEASE(sSamplerState_Anisotropic);
	RJE_SAFE_RELEASE(sSamplerState_Linear);

	RJE_SAFE_RELEASE(sRasterizerState_Solid);
	RJE_SAFE_RELEASE(sRasterizerState_CullNone);
	RJE_SAFE_RELEASE(sRasterizerState_CullClockwise);
	RJE_SAFE_RELEASE(sRasterizerState_Wireframe);
}

//////////////////////////////////////////////////////////////////////////

ID3D11RasterizerState*	DX11CommonStates::sRasterizerState_Solid         = nullptr;
ID3D11RasterizerState*	DX11CommonStates::sRasterizerState_Wireframe     = nullptr;
ID3D11RasterizerState*	DX11CommonStates::sRasterizerState_CullNone      = nullptr;
ID3D11RasterizerState*	DX11CommonStates::sRasterizerState_CullClockwise = nullptr;
ID3D11RasterizerState*	DX11CommonStates::sCurrentRasterizerState        = nullptr;

ID3D11SamplerState*	DX11CommonStates::sSamplerState_Anisotropic = nullptr;
ID3D11SamplerState*	DX11CommonStates::sSamplerState_Linear      = nullptr;
ID3D11SamplerState*	DX11CommonStates::sCurrentSamplerState      = nullptr;

ID3D11BlendState*	DX11CommonStates::sBlendState_Opaque               = nullptr;
ID3D11BlendState*	DX11CommonStates::sBlendState_Transparent          = nullptr;
ID3D11BlendState*	DX11CommonStates::sBlendState_AlphaToCoverage      = nullptr;
ID3D11BlendState*	DX11CommonStates::sBlendState_BlendFactor          = nullptr;
ID3D11BlendState*	DX11CommonStates::sBlendState_NoRenderTargetWrites = nullptr;
ID3D11BlendState*	DX11CommonStates::sCurrentBlendState               = nullptr;

ID3D11DepthStencilState* DX11CommonStates::sDepthStencilState_MarkStencil   = nullptr;
ID3D11DepthStencilState* DX11CommonStates::sDepthStencilState_DrawStenciled = nullptr;
ID3D11DepthStencilState* DX11CommonStates::sDepthStencilState_NoDoubleBlend = nullptr;
ID3D11DepthStencilState* DX11CommonStates::sDepthStencilState_Default       = nullptr;
ID3D11DepthStencilState* DX11CommonStates::sDepthStencilState_DepthDisable  = nullptr;
ID3D11DepthStencilState* DX11CommonStates::sDepthStencilState_DepthRead     = nullptr;
ID3D11DepthStencilState* DX11CommonStates::sCurrentDepthStencilState        = nullptr;
