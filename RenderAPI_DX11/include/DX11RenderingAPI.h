#pragma once

#include "DX11Helper.h"
#include "../../RamJamEngine/include/GraphicAPI.h"


struct DX11RenderingAPI : GraphicAPI
{
	DX11RenderingAPI();
	~DX11RenderingAPI();

	DX11Device*				mDX11Device;
	DX11DepthBuffer*		mDX11DepthBuffer;
	DX11CommonStates*		mDX11CommonStates;

	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;
	D3D11_VIEWPORT			mScreenViewport;
	D3D_DRIVER_TYPE			md3dDriverType;

#if defined(RJE_DEBUG)  
	IDXGIDebug*			md3dDebug;
	ID3D11InfoQueue*	md3dInfoQueue;
#endif

	DX11FontSheet*		mConsoleFont;
	DX11FontSheet*		mProfilerFont;
	DX11SpriteBatch*	mSpriteBatch;

	//////////////////////////////////////////////////////////////////////////

	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;

	ID3D11Buffer*	mVertexBuffer_Gizmo;
	ID3D11Buffer*	mIndexBuffer_Gizmo;

	ID3D11ShaderResourceView* mBoxMap;
	ID3D11ShaderResourceView* mGridMap;
	ID3D11ShaderResourceView* mSphereMap;
	ID3D11ShaderResourceView* mCylinderMap;
	ID3D11ShaderResourceView* mMaskMap;
	ID3D11ShaderResourceView* mWhiteSRV;
	ID3D11ShaderResourceView* mConsoleBackground;
	ID3D11ShaderResourceView* mRjeLogo;

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

	//---------------
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
	//---------------

	//---------------
	Vector3    TEST_VEC_ROTATION;
	Quaternion TEST_QUAT_ROTATION;
	Matrix44   TEST_MAT_ROTATION;
// 	Vector3		mWireBoxPosition;
// 	Quaternion	mWireBoxRotation;
// 	Vector3		mWireBoxScale;
// 	Matrix44	mWireBoxWorld;
	XMFLOAT4X4 mWireBoxWorld;
	XMFLOAT4X4 mAxisWorld;
	int  mWireBoxVertexOffset;
	int  mAxisVertexOffset;
	UINT mWireBoxIndexCount;
	UINT mAxisIndexCount;
	UINT mWireBoxIndexOffset;
	UINT mAxisIndexOffset;
	//---------------

	XMFLOAT3 mEyePosW;

	// Scene Parameters : TODO: Get these out of DX11RenderingAPI !
	UINT mDirLightCount;
	UINT mPointLightCount;
	BOOL mbUseTexture;
	BOOL mbUseBlending;
	BOOL mbUseFog;
	BOOL mbWireframe;
	
	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(HWND hMainWnd, int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	//------------

	virtual void SetWireframe(BOOL state);

	//////////////////////////////////////////////////////////////////////////

	void LoadTexture(string keyName, ID3D11ShaderResourceView** shaderResourceView);
	void Create2DTexture(i32 height, i32 width, RJE_COLOR::Color color, ID3D11ShaderResourceView** textureSRV);
	void BuildGeometryBuffers();
	void BuildGizmosBuffers();

	void DrawGizmos();
	void DrawConsole();
	void DrawProfiler();
	void Draw2dElements();
};
