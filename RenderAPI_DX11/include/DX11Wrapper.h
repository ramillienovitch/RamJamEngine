#pragma once

#include "DX11Helper.h"
#include "../../RamJamEngine/include/GraphicAPI.h"
#include "IniFile.h"


struct DX11Wrapper : GraphicAPI
{
	DX11Wrapper();
	~DX11Wrapper();

	DX11Device*				mDX11Device;
	DX11DepthBuffer*		mDX11DepthBuffer;
	DX11CommonStates*		mDX11CommonStates;

	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;
	D3D11_VIEWPORT			mScreenViewport;
	D3D_DRIVER_TYPE			md3dDriverType;

	//////////////////////////////////////////////////////////////////////////

	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;

	ID3D11ShaderResourceView* mBoxMap;
	ID3D11ShaderResourceView* mGridMap;
	ID3D11ShaderResourceView* mSphereMap;
	ID3D11ShaderResourceView* mCylinderMap;
	ID3D11ShaderResourceView* mMaskMap;
	ID3D11ShaderResourceView* mWhiteMaskMap;

	ID3D11RasterizerState*	mRasterizerState_Solid;
	ID3D11RasterizerState*	mRasterizerState_Wireframe;
	ID3D11RasterizerState*	mCurrentRasterizerState;

	ID3D11SamplerState* mSamplerState_Anisotropic;
	ID3D11SamplerState* mSamplerState_Linear;
	ID3D11SamplerState* mCurrentSamplerState;

	PointLight       mPointLights[3];
	DirectionalLight mDirLights[3];
	Material mBoxMat;
	Material mGridMat;
	Material mCylinderMat;
	Material mSphereMat;
	Material mModelMat;

	XMFLOAT4X4 mBoxTexTransform;
	XMFLOAT4X4 mGridTexTransform;
	XMFLOAT4X4 mSphereTexTransform;
	XMFLOAT4X4 mCylinderTexTransform;
	
	XMFLOAT4X4 mBoxWorld;
	XMFLOAT4X4 mGridWorld;
	XMFLOAT4X4 mSphereWorld[10];
	XMFLOAT4X4 mCylWorld[10];
	XMFLOAT4X4 mModelWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mSphereVertexOffset;
	int mCylinderVertexOffset;
	int mModelVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;
	UINT mModelIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mSphereIndexCount;
	UINT mCylinderIndexCount;
	UINT mModelIndexCount;

	UINT mDirLightCount;
	UINT mPointLightCount;

	XMFLOAT3 mEyePosW;

	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(HWND hMainWnd, int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt, float theta, float phi, float radius );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	//////////////////////////////////////////////////////////////////////////

	void BuildGeometryBuffers();
};
