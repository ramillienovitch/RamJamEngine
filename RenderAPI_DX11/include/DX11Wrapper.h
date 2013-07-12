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

	ID3D11RasterizerState*	mRasterizerState_Solid;
	ID3D11RasterizerState*	mRasterizerState_Wireframe;

	DirectionalLight mDirLights[3];
	Material mGridMat;
	Material mBoxMat;
	Material mCylinderMat;
	Material mSphereMat;
	Material mDragonMat;

	XMFLOAT4X4 mSphereWorld[10];
	XMFLOAT4X4 mCylWorld[10];
	XMFLOAT4X4 mBoxWorld;
	XMFLOAT4X4 mGridWorld;
	XMFLOAT4X4 mDragonWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mSphereVertexOffset;
	int mCylinderVertexOffset;
	int mDragonVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;
	UINT mDragonIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mSphereIndexCount;
	UINT mCylinderIndexCount;
	UINT mDragonIndexCount;

	UINT mLightCount;

	XMFLOAT3 mEyePosW;
	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(HWND hMainWnd, int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt, float theta, float phi, float radius );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	void BuildGeometryBuffers();
};
