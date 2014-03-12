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
	DX11CommonStates*		mDX11CommonStates;

	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mBackbufferRTV;
	D3D11_VIEWPORT			mScreenViewport;
	D3D_DRIVER_TYPE			md3dDriverType;

	//---------------
	Depth2D*								mDepthBuffer;
	std::vector<Texture2D*>					mGBuffer;
	std::vector<ID3D11RenderTargetView*>	mGBufferRTV;
	std::vector<ID3D11ShaderResourceView*>	mGBufferSRV;
	StructuredBuffer<uint2Color>*			mLitBuffer;
	ID3D11UnorderedAccessView*				mLitBufferUAV;
	ID3D11ShaderResourceView*				mLitBufferSRV;
	//---------------

	//----------------------
	//------- SDSM --------
	DX11SDSM		mSDSMPartitions;
	u32				mShadowTextureDim;
	D3D11_VIEWPORT	mShadowViewport;
	Depth2D*		mShadowDepthTexture;
	Texture2D*		mShadowEVSMTexture;
	Texture2D*		mShadowEVSMBlurTexture;
	//---------------
	
	//---------------
	BOOL            mbUseFrustumCulling;
	BOOL            mbUseAABB;	// if not, use Bounding Sphere
	BoundingBox     mAABB;
	BoundingFrustum mCameraFrustum;
	u32             mRenderedSubsets;
	u32             mTotalSubsets;
	//---------------

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

	//---------------
	StructuredBuffer<DirectionalLight>*		mDirLights;
	StructuredBuffer<PointLight>*			mPointLights;
	StructuredBuffer<SpotLight>*			mSpotLights;
	//---------------
	float							mPointLightsRadius   [MAX_LIGHTS];
	float							mPointLightsHeight   [MAX_LIGHTS];
	float							mPointLightsAngle    [MAX_LIGHTS];
	float							mPointLightsAnimSpeed[MAX_LIGHTS];
	DirectionalLight				mWorkingDirLights  [MAX_LIGHTS];
	PointLight						mWorkingPointLights[MAX_LIGHTS];
	SpotLight						mWorkingSpotLights [MAX_LIGHTS];
	u32 mDirLightCount,   mDirLightUICount;
	u32 mPointLightCount, mPointLightUICount;
	u32 mSpotLightCount,  mSpotLightUICount;
	Material mLightSphereMat;
	//---------------

	float mBlendFactorR;
	float mBlendFactorG;
	float mBlendFactorB;
	float mBlendFactorA;

	//---------------

	ID3D11Buffer*				mLightSpheresVB;
	ID3D11Buffer*				mLightSpheresIB;
	u32							mLightSphereIndexCount;
	//---------
	ID3D11Buffer*				mScreenQuadVB;
	ID3D11Buffer*				mScreenQuadIB;
	ID3D11Buffer*				mSkyboxVB;
	ID3D11Buffer*				mSkyboxIB;
	ID3D11ShaderResourceView*	mSkyboxSRV;
	ID3D11ShaderResourceView*	mRjeLogo;
	
	//////////////////////////////////////////////////////////////////////////

	virtual void Initialize(int windowWidth, int windowHeight);
	virtual void UpdateScene( float dt );
	virtual void DrawScene();
	virtual void Shutdown();
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight);

	//------------

	virtual void SetWireframe(BOOL state);
	virtual void SetMSAA(u32 MSAAsamples);
	virtual void InstantiateModel(string filename);
	virtual void InstantiatePrimitive(string name);

	//////////////////////////////////////////////////////////////////////////

	void InitSwapChain(u32 msaaSamples);
	void ResizeWindow();
	//---------------
	void BuildLightSpheres();
	void BuildScreenQuad();
	void BuildSkybox();
	void BuildDepthBuffers(  DXGI_SAMPLE_DESC sampleDesc);
	void BuildGBuffer(       DXGI_SAMPLE_DESC sampleDesc);
	void BuildShadowTextures(DXGI_SAMPLE_DESC sampleDesc);
	//---------------
	void DrawLightSpheres(ID3DX11EffectTechnique* activeTech, u32 pass, BOOL bSun = false);
	void DrawGizmos();
	void DrawConsole();
	void DrawProfiler();
	void Draw2dElements();
	//---------------
	void UpdateShadowCamera();
	//---------------
	void ComputeFrustumFlags();
	void ClearFrustumFlags();
	//---------------
	void SetActiveDirLights(  int activeLights);
	void SetActivePointLights(int activeLights);
	void SetActiveSpotLights( int activeLights);

	//////////////////////////////////////////////////////////////////////////

	void RenderForward();
	//-----------
	void RenderGBuffer();
	void ComputeLighting();
	//-----------
	void RenderSkybox(BOOL deferredRendering);
	void RenderLightSpheres();
	void RenderScreenQuad(ID3D11ShaderResourceView* srv, BOOL bMultiSampled=false, u32 width=0, u32 height=0);

	//////////////////////////////////////////////////////////////////////////

	ID3D11ShaderResourceView* ComputeSDSMPartitions();
	void RenderShadowDepth(ID3D11ShaderResourceView* partitionSRV, u32 currentPartition);
	void ConvertToEVSM( ID3D11ShaderResourceView* depthInput, ID3D11RenderTargetView* evsmOutput, ID3D11ShaderResourceView* partitionSRV, u32 partitionIndex);
	void AccumulateLighting( ID3D11RenderTargetView* backBuffer, ID3D11ShaderResourceView* shadowSRV, ID3D11ShaderResourceView* partitionSRV, u32 partitionIndex);
	
	//-----------
	// EVSM Edge softening
	void BoxBlurPass(	ID3D11ShaderResourceView* input, ID3D11RenderTargetView* output, ID3D11ShaderResourceView* partitionSRV,
						u32 partitionIndex, const D3D11_VIEWPORT* viewport, const Vector2& filterSize, unsigned int dimension);
	void BoxBlur(Texture2D* texture, u32 textureElement, Texture2D* temp, u32 partitionIndex, ID3D11ShaderResourceView* partitionSRV, const Vector2& filterSize);
};

//////////////////////////////////////////////////////////////////////////
// AntTweak Bar CallBacks. TODO: find a way to avoid duplication :(
void TW_CALL TwSetWireframe(void *clientData);
void TW_CALL TwClearFrustumFlags(void *clientData);
