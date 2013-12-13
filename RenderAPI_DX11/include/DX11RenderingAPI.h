#pragma once

#include "DX11Helper.h"
#include "../../RamJamEngine/include/GraphicAPI.h"
#include "../../RamJamEngine/include/Scene.h"
#include "../../RamJamEngine/include/AntTweakBar.h"


struct DX11RenderingAPI : GraphicAPI
{
	DX11RenderingAPI(Scene& scene);
	virtual ~DX11RenderingAPI();

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

	DX11MaterialLoader  mMaterialLoader;
	DX11FontSheet*		mConsoleFont;
	DX11FontSheet*		mProfilerFont;
	DX11SpriteBatch*	mSpriteBatch;

	u32 mWindowWidth;
	u32 mWindowHeight;
	u32 MSAA_Samples;

	// shortcut for System::Instance()->mScene
	Scene& mScene;

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
	ID3D11ShaderResourceView* mRjeLogo;

	StructuredBuffer<PointLight>*	mPointLights;
	PointLight						mOldPointLights[MAX_LIGHTS];		// Used for reflections
	PointLight						mWorkingPointLights[MAX_LIGHTS];
	DirectionalLight mDirLights[3];
	u32 mDirLightCount;
	u32 mPointLightCount;

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

	u32 mBoxIndexOffset;
	u32 mGridIndexOffset;
	u32 mSphereIndexOffset;
	u32 mCylinderIndexOffset;
	u32 mModelIndexOffset;

	u32 mBoxIndexCount;
	u32 mGridIndexCount;
	u32 mSphereIndexCount;
	u32 mCylinderIndexCount;
	u32 mModelIndexCount;
	//---------------

	//---------------
	Matrix44 mWireBoxWorld;
	Matrix44 mAxisWorld;
	int  mWireBoxVertexOffset;
	int  mAxisVertexOffset;
	u32 mWireBoxIndexCount;
	u32 mAxisIndexCount;
	u32 mWireBoxIndexOffset;
	u32 mAxisIndexOffset;
	//---------------

	float mBlendFactorR;
	float mBlendFactorG;
	float mBlendFactorB;
	float mBlendFactorA;

	//---------------
	Vector3 mEyePosW;
	
	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	//------------

	virtual void SetWireframe(BOOL state);
	virtual void SetMSAA(u32 MSAAsamples);

	//////////////////////////////////////////////////////////////////////////

	void InitSwapChain(u32 msaaSamples = 4);
	void ResizeWindow();
	//---------------
	void BuildGeometryBuffers();
	void BuildGizmosBuffers();
	//---------------
	void DrawGizmos();
	void DrawConsole();
	void DrawProfiler();
	void Draw2dElements();
	//---------------
	void SetActivePointLights(u32 activeLights);
};

//////////////////////////////////////////////////////////////////////////
// AntTweak Bar CallBacks. TODO: find a way to avoid duplication :(
void TW_CALL TwSetWireframe(void *clientData);
