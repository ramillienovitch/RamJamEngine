#pragma once

#include "DX11Helper.h"

struct DX11CommonStates 
{
	HRESULT CreateBlendState(ID3D11Device* device, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, _Out_ ID3D11BlendState** pResult);
	HRESULT CreateDepthStencilState(ID3D11Device* device, bool enable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult);
	HRESULT CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult);
	HRESULT CreateSamplerState(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, _Out_ ID3D11SamplerState** pResult);

	// Blend states.
	HRESULT Opaque(          ID3D11Device*, ID3D11BlendState**);
	HRESULT AlphaBlend(      ID3D11Device*, ID3D11BlendState**);
	HRESULT Additive(        ID3D11Device*, ID3D11BlendState**);
	HRESULT NonPremultiplied(ID3D11Device*, ID3D11BlendState**);

	// Depth stencil states.
	HRESULT DepthNone(   ID3D11Device*, ID3D11DepthStencilState**);
	HRESULT DepthDefault(ID3D11Device*, ID3D11DepthStencilState**);
	HRESULT DepthRead(   ID3D11Device*, ID3D11DepthStencilState**);

	// Rasterizer states.
	HRESULT CullNone(            ID3D11Device*, ID3D11RasterizerState**);
	HRESULT CullClockwise(       ID3D11Device*, ID3D11RasterizerState**);
	HRESULT CullCounterClockwise(ID3D11Device*, ID3D11RasterizerState**);
	HRESULT Wireframe(           ID3D11Device*, ID3D11RasterizerState**);

	// Sampler states.
	HRESULT PointWrap(       ID3D11Device*, ID3D11SamplerState**);
	HRESULT PointClamp(      ID3D11Device*, ID3D11SamplerState**);
	HRESULT LinearWrap(      ID3D11Device*, ID3D11SamplerState**);
	HRESULT LinearClamp(     ID3D11Device*, ID3D11SamplerState**);
	HRESULT AnisotropicWrap( ID3D11Device*, ID3D11SamplerState**);
	HRESULT AnisotropicClamp(ID3D11Device*, ID3D11SamplerState**);
};