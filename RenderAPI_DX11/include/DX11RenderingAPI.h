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

	UINT mWindowWidth;
	UINT mWindowHeight;
	UINT MSAA_Samples;

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

	StructuredBuffer<PointLight>*	mPointLights;
	PointLight						mOldPointLights[MAX_LIGHTS];		// Used for reflections
	PointLight						mWorkingPointLights[MAX_LIGHTS];
	DirectionalLight mDirLights[3];
	UINT mDirLightCount;
	UINT mPointLightCount;

	Material mBoxMat;
	Material mGridMat;
	Material mCylinderMat;
	Material mSphereMat;
	Material mModelMat;

	Matrix44 mBoxTexTransform;
	Matrix44 mGridTexTransform;
	Matrix44 mSphereTexTransform;
	Matrix44 mCylinderTexTransform;
	
	Matrix44 mBoxWorld;
	Matrix44 mGridWorld;
	Matrix44 mSphereWorld[10];
	Matrix44 mCylWorld[10];
	Matrix44 mModelWorld;
	TwQuaternion mModelRot;

	Matrix44 mView;
	Matrix44 mProj;

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
	Matrix44 mWireBoxWorld;
	Matrix44 mAxisWorld;
	int  mWireBoxVertexOffset;
	int  mAxisVertexOffset;
	UINT mWireBoxIndexCount;
	UINT mAxisIndexCount;
	UINT mWireBoxIndexOffset;
	UINT mAxisIndexOffset;
	//---------------

	Vector3 mEyePosW;

	// Scene Parameters : TODO: Get these out of DX11RenderingAPI !
	BOOL mbDrawReflections;
	BOOL mbUseTexture;
	BOOL mbUseBlending;
	BOOL mbUseFog;
	BOOL mbWireframe;
	
	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	//------------

	virtual void SetWireframe(BOOL state);
	virtual void SetMSAA(UINT MSAAsamples);

	//////////////////////////////////////////////////////////////////////////

	void InitSwapChain(UINT msaaSamples = 4);
	void ResizeWindow();
	//---------------
	void LoadTexture(string keyName, ID3D11ShaderResourceView** shaderResourceView);
	void Create2DTexture(i32 height, i32 width, RJE_COLOR::Color color, ID3D11ShaderResourceView** textureSRV);
	void BuildGeometryBuffers();
	void BuildGizmosBuffers();
	//---------------
	void DrawGizmos();
	void DrawConsole();
	void DrawProfiler();
	void Draw2dElements();
	//---------------
	void SetActivePointLights(UINT activeLights);
};
