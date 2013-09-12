#pragma once

#include "DX11Helper.h"

struct DX11CommonStates 
{
	static HRESULT CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult);
	static HRESULT CreateSamplerState(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, _Out_ ID3D11SamplerState** pResult);
	static HRESULT CreateDepthStencilState(	ID3D11Device* device,
											_Out_ ID3D11DepthStencilState** pResult,
											bool depthEnable, bool depthWriteEnable, bool stencilEnable,
											D3D11_COMPARISON_FUNC depthFunc,
											D3D11_COMPARISON_FUNC stencilFunc,
											D3D11_STENCIL_OP backFaceStencilPassOp,
											D3D11_STENCIL_OP backFaceStencilFailOp,
											D3D11_STENCIL_OP backFaceStencilDepthFailOp,
											D3D11_STENCIL_OP frontFaceStencilPassOp,
											D3D11_STENCIL_OP frontFaceStencilFailOp,
											D3D11_STENCIL_OP frontFaceStencilDepthFailOp );
	static HRESULT CreateBlendState(ID3D11Device* device,
									_Out_ ID3D11BlendState** pResult,
									BOOL alphaToCoverageEnable,
									BOOL independentBlendEnable,
									BOOL blendEnable,
									D3D11_BLEND srcBlend,
									D3D11_BLEND srcBlendAlpha,
									D3D11_BLEND destBlend,
									D3D11_BLEND destBlendAlpha,
									D3D11_BLEND_OP blendOp,
									D3D11_BLEND_OP blendOpAlpha);

	// Blend states.
	static HRESULT Opaque(              ID3D11Device*, ID3D11BlendState**);
	static HRESULT Tranparent(          ID3D11Device*, ID3D11BlendState**);
	static HRESULT AlphaToCoverage(     ID3D11Device*, ID3D11BlendState**);
	static HRESULT AlphaBlend(          ID3D11Device*, ID3D11BlendState**);
	static HRESULT BlendFactor(         ID3D11Device*, ID3D11BlendState**);
	static HRESULT NonPremultiplied(    ID3D11Device*, ID3D11BlendState**);
	static HRESULT NoRenderTargetWrites(ID3D11Device*, ID3D11BlendState**);

	// Depth stencil states.
	static HRESULT DepthNone(     ID3D11Device*, ID3D11DepthStencilState**);
	static HRESULT DepthDefault(  ID3D11Device*, ID3D11DepthStencilState**);
	static HRESULT DepthDisable(  ID3D11Device*, ID3D11DepthStencilState**);
	static HRESULT DepthRead(     ID3D11Device*, ID3D11DepthStencilState**);
	static HRESULT MarkReflection(ID3D11Device*, ID3D11DepthStencilState**);
	static HRESULT DrawReflection(ID3D11Device*, ID3D11DepthStencilState**);
	static HRESULT NoDoubleBlend( ID3D11Device*, ID3D11DepthStencilState**);

	// Rasterizer states.
	static HRESULT CullNone(            ID3D11Device*, ID3D11RasterizerState**);
	static HRESULT CullClockwise(       ID3D11Device*, ID3D11RasterizerState**);
	static HRESULT CullCounterClockwise(ID3D11Device*, ID3D11RasterizerState**);
	static HRESULT Wireframe(           ID3D11Device*, ID3D11RasterizerState**);

	// Sampler states.
	static HRESULT PointWrap(       ID3D11Device*, ID3D11SamplerState**);
	static HRESULT PointClamp(      ID3D11Device*, ID3D11SamplerState**);
	static HRESULT LinearWrap(      ID3D11Device*, ID3D11SamplerState**);
	static HRESULT LinearClamp(     ID3D11Device*, ID3D11SamplerState**);
	static HRESULT AnisotropicWrap( ID3D11Device*, ID3D11SamplerState**);
	static HRESULT AnisotropicClamp(ID3D11Device*, ID3D11SamplerState**);

	//////////////////////////////////////////////////////////////////////////

	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11RasterizerState*	sRasterizerState_Solid;
	static ID3D11RasterizerState*	sRasterizerState_CullNone;
	static ID3D11RasterizerState*	sRasterizerState_CullClockwise;
	static ID3D11RasterizerState*	sRasterizerState_Wireframe;
	static ID3D11RasterizerState*	sCurrentRasterizerState;

	static ID3D11SamplerState*	sSamplerState_Anisotropic;
	static ID3D11SamplerState*	sSamplerState_Linear;
	static ID3D11SamplerState*	sCurrentSamplerState;

	static ID3D11BlendState*	sBlendState_Opaque;
	static ID3D11BlendState*	sBlendState_Transparent;
	static ID3D11BlendState*	sBlendState_AlphaToCoverage;
	static ID3D11BlendState*	sBlendState_BlendFactor;
	static ID3D11BlendState*	sBlendState_NoRenderTargetWrites;
	static ID3D11BlendState*	sCurrentBlendState;

	static ID3D11DepthStencilState* sDepthStencilState_MarkReflection;
	static ID3D11DepthStencilState* sDepthStencilState_DrawReflection;
	static ID3D11DepthStencilState* sDepthStencilState_NoDoubleBlend;
	static ID3D11DepthStencilState* sDepthStencilState_DepthDisable;
	static ID3D11DepthStencilState* sDepthStencilState_Default;
	static ID3D11DepthStencilState* sCurrentDepthStencilState;
};