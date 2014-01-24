#pragma once

#include "DX11Helper.h"
#include "../../RamJamEngine/include/GraphicAPI.h"
#include "../../RamJamEngine/include/Scene.h"
#include "../../RamJamEngine/include/AntTweakBar.h"
#include "../../RamJamEngine/include/GameObject.h"


//////////////////////////////////////////////////////////////////////////
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

	DX11FontSheet*		mConsoleFont;
	DX11FontSheet*		mProfilerFont;
	DX11SpriteBatch*	mSpriteBatch;

	u32 mWindowWidth;
	u32 mWindowHeight;
	u32 MSAA_Samples;

	// shortcut for System::Instance()->mScene
	Scene& mScene;

	//////////////////////////////////////////////////////////////////////////
	// Used in Tiled Deferred Shading

	std::vector< std::tr1::shared_ptr<Texture2D> >	mGBuffer;
	std::vector<ID3D11RenderTargetView*>			mGBufferRTV;			// Handy cache of list of RT pointers for G-buffer
	std::vector<ID3D11ShaderResourceView*>			mGBufferSRV;			// Handy cache of list of SRV pointers for the G-buffer
	unsigned int mGBufferWidth;
	unsigned int mGBufferHeight;

	// We use a different lit buffer (different bind flags and MSAA handling) depending on whether we
	// write to it from the pixel shader (render target) or compute shader (UAV)
	std::tr1::shared_ptr<StructuredBuffer<FramebufferFlatElement> > mLitBufferCS;

	std::tr1::shared_ptr<Depth2D> mDepthBuffer;

	//////////////////////////////////////////////////////////////////////////

	ID3D11ShaderResourceView* mRjeLogo;

	enum LightMode
	{
		Directional,
		Point,
		Spot
	};

	LightMode mLightMode;
	StructuredBuffer<DirectionalLight>*		mDirLights;
	StructuredBuffer<PointLight>*			mPointLights;
	StructuredBuffer<SpotLight>*			mSpotLights;
	float							mPointLightsRadius[MAX_LIGHTS];
	float							mPointLightsHeight[MAX_LIGHTS];
	float							mPointLightsAngle[MAX_LIGHTS];
	float							mPointLightsAnimSpeed[MAX_LIGHTS];
	DirectionalLight				mWorkingDirLights[MAX_LIGHTS];
	PointLight						mWorkingPointLights[MAX_LIGHTS];
	SpotLight						mWorkingSpotLights[MAX_LIGHTS];
	u32 mDirLightCount;
	u32 mPointLightCount;
	u32 mSpotLightCount;

	Matrix44 mView;
	Matrix44 mProj;

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
	void DrawGizmos();
	void DrawConsole();
	void DrawProfiler();
	void Draw2dElements();
	//---------------
	void SetActiveDirLights(u32 activeLights);
	void SetActivePointLights(u32 activeLights);
	void SetActiveSpotLights(u32 activeLights);
};

//////////////////////////////////////////////////////////////////////////
// AntTweak Bar CallBacks. TODO: find a way to avoid duplication :(
void TW_CALL TwSetWireframe(void *clientData);
