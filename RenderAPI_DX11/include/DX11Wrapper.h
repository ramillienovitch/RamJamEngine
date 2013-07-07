#pragma once

#include "DX11Helper.h"
#include "../../RamJamEngine/include/GraphicAPI.h"

struct DX11Wrapper : GraphicAPI
{
	DX11Wrapper();
	//virtual ~DX11Wrapper();

	DX11Device*				mDX11Device;
	DX11DepthBuffer*		mDX11DepthBuffer;

	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;
	D3D11_VIEWPORT			mScreenViewport;
	D3D_DRIVER_TYPE			md3dDriverType;

	//////////////////////////////////////////////////////////////////////////
	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;
	UINT			mVertexCount;
	UINT			mIndexCount;

	ID3DX11Effect*					mFX;
	ID3DX11EffectTechnique*			mTech;
	ID3DX11EffectMatrixVariable*	mfxWorldViewProj;

	ID3D11InputLayout*		mInputLayout;
	ID3D11RasterizerState*	mRasterizerState;

	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;
	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(HWND hMainWnd, int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt, float theta, float phi, float radius );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();

	float GetHeight(float x, float z) const;
};
