#include "DX11RenderingAPI.h"
#include "../../RamJamEngine/include/System.h"


DX11RenderingAPI::~DX11RenderingAPI() {}

//////////////////////////////////////////////////////////////////////////
DX11RenderingAPI::DX11RenderingAPI(Scene& scene) : mScene(scene)
{
	mDX11Device    = nullptr;
	mDepthBuffer   = nullptr;
	mSwapChain     = nullptr;
	mBackbufferRTV = nullptr;
	md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));
	//-----------
	VSyncEnabled = false;
	//-----------
	mConsoleFont  = nullptr;
	mProfilerFont = nullptr;
	mSpriteBatch  = nullptr;
	//-----------
	mRjeLogo  = nullptr;
	mSkyboxVB = nullptr;
	//-----------
	mScreenQuadVB = nullptr;
	mScreenQuadIB = nullptr;
	mSkyboxVB     = nullptr;
	mSkyboxIB     = nullptr;
	//-----------
	mBlendFactorR = 0.5f;
	mBlendFactorG = 0.5f;
	mBlendFactorB = 0.5f;
	mBlendFactorA = 1.0f;
	//-----------
	
	// Light Specs
	mLightMode = LightMode::Point;
	mDirLights   = nullptr;
	mPointLights = nullptr;
	mSpotLights  = nullptr;
	Vector4 black = Color(Color::Black).GetVector4RGBANorm();
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		float radius = RJE::Math::Rand(0.0f,1.0f);
		mPointLightsRadius[i]    = sqrt(radius) * 15.0f;
		mPointLightsHeight[i]    = RJE::Math::Rand(0.0f,10.0f);
		mPointLightsAngle[i]     = RJE::Math::Rand(0.0f,RJE::Math::Pi_Two_f);
		mPointLightsAnimSpeed[i] = RJE::Math::Rand(0.1f,2.0f);
		//--------
		mWorkingDirLights[i].Color     = Vector3(0.5f, 0.5f, 0.5f);
		mWorkingDirLights[i].Direction = Vector3(0.57735f, -0.57735f, 0.57735f);
		//--------
		mWorkingPointLights[i].Color     = Color::GetRandomVector3RGBNorm();
		mWorkingPointLights[i].Range     = RJE::Math::Rand(1.0f,8.0f);
		mWorkingPointLights[i].Intensity = mWorkingPointLights[i].Range * 0.1f;
		//--------
		mWorkingSpotLights[i].Color     = Vector3(0.5f, 0.5f, 0.5f);
		mWorkingSpotLights[i].Spot      = RJE::Math::Deg2Rad_f * 45.0f;
		mWorkingSpotLights[i].Range     = 10.0f;
		mWorkingSpotLights[i].Intensity = 2.0f;
		mWorkingSpotLights[i].Position  = RJE::Math::Rand(0,1000) * Vector3::RandUnitSphere();
		mWorkingSpotLights[i].Direction = RJE::Math::Rand(0,1000) * Vector3::RandUnitSphere();
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::Initialize(int windowWidth, int windowHeight)
{
	mDX11Device       = rje_new DX11Device;
	mDX11CommonStates = rje_new DX11CommonStates;

	mWindowWidth  = windowWidth;
	mWindowHeight = windowHeight;

	// Create the device and device context.
	u32 createDeviceFlags = 0;
#ifdef RJE_DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	RJE_CHECK_FOR_SUCCESS( D3D11CreateDevice(	0,					// default adapter
												md3dDriverType,
												0,					// no software device
												createDeviceFlags,
												0, 0,				// default feature level array
												D3D11_SDK_VERSION,
												&mDX11Device->md3dDevice,
												&featureLevel,
												&mDX11Device->md3dImmediateContext));

	if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		RJE_MESSAGE_BOX(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return;
	}

	//////////////////////////////////////////////////////////////////////////

	PROFILE_GPU_INIT(mDX11Device->md3dDevice, mDX11Device->md3dImmediateContext);

	//////////////////////////////////////////////////////////////////////////
	
	//------- Not used for now
// #ifdef RJE_DEBUG
// 	if( SUCCEEDED( mDX11Device->md3dDevice->QueryInterface( __uuidof(ID3D11Debug), (void**)&md3dDebug ) ) )
// 	{
// 		if( SUCCEEDED( md3dDebug->QueryInterface( __uuidof(ID3D11InfoQueue), (void**)&md3dInfoQueue ) ) )
// 		{
// 			md3dInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_CORRUPTION, true );
// 			md3dInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_ERROR, true );
// 
// 			D3D11_MESSAGE_ID hide [] =
// 			{
// 				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
// 				// Add more message IDs here as needed
// 			};
// 
// 			D3D11_INFO_QUEUE_FILTER filter;
// 			memset( &filter, 0, sizeof(filter) );
// 			filter.DenyList.NumIDs = _countof(hide);
// 			filter.DenyList.pIDList = hide;
// 			md3dInfoQueue->AddStorageFilterEntries( &filter );
// 			md3dInfoQueue->Release();
// 		}
// 		md3dDebug->Release();
// 	}
// #endif

	//--------------------
	InitSwapChain();
	//--------------------

	IDXGIDevice*  dxgiDevice  = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;

	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	RJE_CHECK_FOR_SUCCESS(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	DXGI_ADAPTER_DESC adapterDesc;
	dxgiAdapter->GetDesc(&adapterDesc);
	memcpy_s(System::Instance()->mGpuDescription, 128*sizeof(WCHAR), adapterDesc.Description, 128*sizeof(WCHAR));
	// Remove white space at the end
	for(size_t i=wcslen(System::Instance()->mGpuDescription)-1 ; i>0 ; --i)
	{
		if(System::Instance()->mGpuDescription[i] == ' ')
			System::Instance()->mGpuDescription[i] = nullchar;
		else break;
	}
	System::Instance()->mGpuDedicatedVRAM = (adapterDesc.DedicatedVideoMemory/1024)/1024;
	System::Instance()->mGpuSharedVRAM    = (adapterDesc.SharedSystemMemory/1024)/1024;

	RJE_SAFE_RELEASE(dxgiDevice);
	RJE_SAFE_RELEASE(dxgiAdapter);

	// We init the effect first and then the input layouts
	DX11Effects     ::InitAll(mDX11Device->md3dDevice);
	DX11InputLayouts::InitAll(mDX11Device->md3dDevice);
	DX11CommonStates::InitAll(mDX11Device->md3dDevice);

	DX11TextureManager::Instance()->Initialize(mDX11Device->md3dDevice);

	// Init the 2d elements
	mProfilerFont = rje_new DX11FontSheet();
	mConsoleFont  = rje_new DX11FontSheet();
	mSpriteBatch  = rje_new DX11SpriteBatch();
	RJE_CHECK_FOR_SUCCESS(mConsoleFont-> Initialize(mDX11Device->md3dDevice, L"Consolas", 16.0f, FontSheet::FontStyleRegular, true));
	RJE_CHECK_FOR_SUCCESS(mProfilerFont->Initialize(mDX11Device->md3dDevice, L"Consolas", 16.0f, FontSheet::FontStyleBold,    true));
	RJE_CHECK_FOR_SUCCESS(mSpriteBatch-> Initialize(mDX11Device->md3dDevice, mDX11Device->md3dImmediateContext));

	DX11TextureManager::Instance()->LoadTexture("rje_logo", &mRjeLogo);
	DX11TextureManager::Instance()->LoadTexture("skybox", &mSkyboxSRV);
	DX11TextureManager::Instance()->Create2DTextureFixedColor(1, RJE_COLOR::Color::TransDarkGray, "_transparentGray");
	DX11TextureManager::Instance()->Create2DTextureFixedColor(1, RJE_COLOR::Color::White,         "_default");
	//----------
	DX11Mesh::SetDevice(mDX11Device->md3dDevice, mDX11Device->md3dImmediateContext);
	DX11Drawable::SetShader(DX11Effects::BasicFX);
	DX11Drawable::SetShaderGizmo(DX11Effects::ColorFX);
	//-----------
	SetActivePointLights(0);
	SetActiveDirLights(1);
	SetActiveSpotLights(0);
	//-----------
	BuildScreenQuad();
	BuildSkybox();

	//////////////////////////////////////////////////////////////////////////
	// AntTweak Bars
	if (!TwInit(TW_DIRECT3D11, mDX11Device->md3dDevice))
		MessageBoxA(System::Instance()->mHWnd, TwGetLastError(), "AntTweakBar initialization failed", MB_OK|MB_ICONERROR);
	//-----------
	// Create a tweak bar
	TwBar *bar = TwNewBar("Scene");
	TwDefine("Scene iconified=true ");
	int barSize[2] = {300, 400};
	TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);
	TwAddVarRW(bar, "V-Sync Enabled", TW_TYPE_BOOLCPP, &VSyncEnabled, NULL);
	TwAddVarRW(bar, "Render Deferred",  TW_TYPE_BOOLCPP, &mScene.mbDeferredRendering, NULL);
	TwAddSeparator(bar, NULL, NULL); //===============================================
	TwAddButton(bar, "Toggle Wireframe", TwSetWireframe, this, NULL);
	TwAddSeparator(bar, NULL, NULL); //===============================================
	TwAddVarRW(bar, "Ambient Color", TW_TYPE_COLOR4F, &mScene.mAmbientLightColor, NULL);
	TwAddSeparator(bar, NULL, NULL); //===============================================
	TwAddVarRW(bar, "Use Texture",  TW_TYPE_BOOLCPP, &mScene.mbUseTexture, NULL);
	TwAddVarRW(bar, "Use Blending", TW_TYPE_BOOLCPP, &mScene.mbUseBlending, NULL);
	TwAddSeparator(bar, NULL, NULL); //===============================================
	TwAddVarRW(bar, "Show Position Only",  TW_TYPE_BOOLCPP, &mScene.mbOnlyPosition,         NULL);
	TwAddVarRW(bar, "Show Albedo Only",    TW_TYPE_BOOLCPP, &mScene.mbOnlyAlbedo,           NULL);
	TwAddVarRW(bar, "Show Normal Only",    TW_TYPE_BOOLCPP, &mScene.mbOnlyNormals,          NULL);
	TwAddVarRW(bar, "Show Specular Only ", TW_TYPE_BOOLCPP, &mScene.mbOnlySpecular,         NULL);
	TwAddVarRW(bar, "Per Sample Shading",  TW_TYPE_BOOLCPP, &mScene.mbViewPerSampleShading, NULL);
	TwAddVarRW(bar, "Use Face Normals",    TW_TYPE_BOOLCPP, &mScene.mbUseFaceNormals,       NULL);
	TwAddVarRW(bar, "Use Fog",             TW_TYPE_BOOLCPP, &mScene.mbUseFog,               NULL);
	TwAddVarRW(bar, "Fog Color", TW_TYPE_COLOR4F, &mScene.mFogColor, NULL);
	TwAddVarRW(bar, "Fog Start", TW_TYPE_FLOAT, &mScene.mFogStart, "min=10 max=100");
	TwAddVarRW(bar, "Fog Range", TW_TYPE_FLOAT, &mScene.mFogRange, "min=20 max=500");
	TwAddSeparator(bar, NULL, NULL); //===============================================
	TwAddVarRW(bar, "Blend Factor Red",   TW_TYPE_FLOAT, &mBlendFactorR, "min=0 max=1 step=0.05");
	TwAddVarRW(bar, "Blend Factor Green", TW_TYPE_FLOAT, &mBlendFactorG, "min=0 max=1 step=0.05");
	TwAddVarRW(bar, "Blend Factor Blue",  TW_TYPE_FLOAT, &mBlendFactorB, "min=0 max=1 step=0.05");
	TwAddVarRW(bar, "Blend Factor Alpha", TW_TYPE_FLOAT, &mBlendFactorA, "min=0 max=1 step=0.05");
	//-----------
	TwBar *gameObjectBar = TwNewBar("GameObject");
	TwDefine("GameObject iconified=true ");
	TwSetParam(gameObjectBar, NULL, "size",  TW_PARAM_INT32, 2, barSize);
	TwAddVarRO(gameObjectBar, "Name",        TW_TYPE_STDSTRING, &mScene.mGameObjectEditorName,  "opened=true");
	TwAddVarRW(gameObjectBar, "Position",    TW_TYPE_DIR3F,     &mScene.mGameObjectEditorPos,   "opened=true");
	TwAddVarRW(gameObjectBar, "Rotation",    TW_TYPE_QUAT4F,    &mScene.mGameObjectEditorRot,   "opened=true");
	TwAddVarRW(gameObjectBar, "Scale",       TW_TYPE_DIR3F,     &mScene.mGameObjectEditorScale, "opened=true");
	TwAddVarRW(gameObjectBar, "Gizmo Color", TW_TYPE_COLOR32,   &mScene.mGameObjectEditorColor, "opened=true colorOrder=argb");
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::InitSwapChain(u32 msaaSamples /*=4*/)
{
	HWND hMainWnd = System::Instance()->mHWnd;
	MSAA_Samples = msaaSamples;

	RJE_SAFE_RELEASE(mSwapChain);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count   = MSAA_Samples;
	sampleDesc.Quality = 0;
	//-----------
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width						= mWindowWidth;
	sd.BufferDesc.Height					= mWindowHeight;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	//-----------
	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = hMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;
	sd.SampleDesc   = sampleDesc;

	//-----------

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain:
	// This function is being called with a device from a different IDXGIFactory."
	IDXGIDevice*  dxgiDevice  = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	RJE_CHECK_FOR_SUCCESS(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	RJE_CHECK_FOR_SUCCESS(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	RJE_CHECK_FOR_SUCCESS(dxgiFactory->CreateSwapChain(mDX11Device->md3dDevice, &sd, &mSwapChain));
	RJE_CHECK_FOR_SUCCESS(dxgiFactory->MakeWindowAssociation(hMainWnd, DXGI_MWA_NO_WINDOW_CHANGES));

	RJE_SAFE_RELEASE(dxgiDevice);
	RJE_SAFE_RELEASE(dxgiAdapter);
	RJE_SAFE_RELEASE(dxgiFactory);
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::UpdateScene( float dt )
{
	//-------------------------------------
	Transform tempTrf;
	tempTrf.Rotation									= mScene.mGameObjectEditorRot;
	tempTrf.Scale										= mScene.mGameObjectEditorScale;
	tempTrf.Position									= mScene.mGameObjectEditorPos;
	mScene.mGameObjectEditorTransform->Position			= mScene.mGameObjectEditorPos;
	mScene.mGameObjectEditorTransform->Rotation			= mScene.mGameObjectEditorRot;
	mScene.mGameObjectEditorTransform->Scale			= mScene.mGameObjectEditorScale;
	mScene.mGameObjectEditorTransform->WorldMat			= tempTrf.WorldMatrix();
	mScene.mGameObjectEditorTransform->WorldMatNoScale	= tempTrf.WorldMatrixNoScale();
	mScene.mGameObjects[0]->mDrawable.mGizmoColor		= mScene.mGameObjectEditorColor;
	//-------------------------------------
	
	// Inputs modifiers : TODO: Get these out of DX11RenderingAPI !
	if (!Console::Instance()->IsActive())
	{
		if (Input::Instance()->GetKeyboardDown(Keyboard1))	mLightMode = LightMode::Directional;
		if (Input::Instance()->GetKeyboardDown(Keyboard2))	mLightMode = LightMode::Point;
		if (Input::Instance()->GetKeyboardDown(Keyboard3))	mLightMode = LightMode::Spot;
		if (Input::Instance()->GetKeyboardDown(A))			DX11CommonStates::sCurrentSamplerState    = DX11CommonStates::sSamplerState_Anisotropic;
		if (Input::Instance()->GetKeyboardDown(L))			DX11CommonStates::sCurrentSamplerState    = DX11CommonStates::sSamplerState_Linear;
		if (Input::Instance()->GetKeyboardDown(U))			DX11CommonStates::sCurrentBlendState      = DX11CommonStates::sBlendState_BlendFactor;
		if (Input::Instance()->GetKeyboardDown(I))			DX11CommonStates::sCurrentBlendState      = DX11CommonStates::sBlendState_AlphaToCoverage;
		if (Input::Instance()->GetKeyboardDown(O))			DX11CommonStates::sCurrentBlendState      = DX11CommonStates::sBlendState_Transparent;
		if (Input::Instance()->GetKeyboardDown(P))			DX11CommonStates::sCurrentBlendState      = DX11CommonStates::sBlendState_Opaque;
		if (Input::Instance()->GetKeyboardDown(Add))
		{
			if (mLightMode == LightMode::Directional)
			{
				if (mDirLightCount < MAX_LIGHTS)
					SetActiveDirLights(mDirLightCount + 1);
			}
			else if (mLightMode == LightMode::Point)
			{
				if (mPointLightCount < MAX_LIGHTS)
					SetActivePointLights(mPointLightCount + 1);
			}
			else if (mLightMode == LightMode::Spot)
			{
				if (mSpotLightCount < MAX_LIGHTS)
					SetActiveSpotLights(mSpotLightCount + 1);
			}
		}
		if (Input::Instance()->GetKeyboardDown(Subtract))
		{
			if (mLightMode == LightMode::Directional)
			{
				if (mDirLightCount > 0)
					SetActiveDirLights(mDirLightCount - 1);
			}
			else if (mLightMode == LightMode::Point)
			{
				if (mPointLightCount > 0)
					SetActivePointLights(mPointLightCount - 1);
			}
			else if (mLightMode == LightMode::Spot)
			{
				if (mSpotLightCount > 0)
					SetActiveSpotLights(mSpotLightCount - 1);
			}
		}
	}

	static float timer = 0.0f;
	timer += dt*0.5f;

	// Copy light list into shader buffer
	if (mDirLightCount > 0)
	{
		DirectionalLight* light = mDirLights->MapDiscard(mDX11Device->md3dImmediateContext);
		for (u32 i = 0; i < mDirLightCount; ++i)
		{
// 			mWorkingDirLights[i].Direction.x = (i+1)*cosf(2*i + RJE::Math::Pi_f - timer );
// 			mWorkingDirLights[i].Direction.y = 2.0f + cosf( timer );
// 			mWorkingDirLights[i].Direction.z = (i+1)*sinf(2*i + RJE::Math::Pi_f - timer );
			light[i] = mWorkingDirLights[i];
		}
		mDirLights->Unmap(mDX11Device->md3dImmediateContext);
	}
	if (mPointLightCount > 0)
	{
		PointLight* light = mPointLights->MapDiscard(mDX11Device->md3dImmediateContext);
		for (u32 i = 0; i < mPointLightCount; ++i)
		{
			mWorkingPointLights[i].Position.x = mPointLightsRadius[i] * cosf(mPointLightsAngle[i] + timer * mPointLightsAnimSpeed[i] );
			mWorkingPointLights[i].Position.y = mPointLightsHeight[i];
			mWorkingPointLights[i].Position.z = mPointLightsRadius[i] * sinf(mPointLightsAngle[i] + timer * mPointLightsAnimSpeed[i] );
			light[i] = mWorkingPointLights[i];
		}
		mPointLights->Unmap(mDX11Device->md3dImmediateContext);
	}
	if (mSpotLightCount > 0)
	{
		SpotLight* light = mSpotLights->MapDiscard(mDX11Device->md3dImmediateContext);
		for (u32 i = 0; i < mSpotLightCount; ++i)
		{
			mWorkingSpotLights[i].Position.x = (i+1)*cosf(2*i + RJE::Math::Pi_f + timer );
			mWorkingSpotLights[i].Position.y = 2.0f + cosf( timer );
			mWorkingSpotLights[i].Position.z =(i+1)*sinf(2*i + RJE::Math::Pi_f + timer );
			light[i] = mWorkingSpotLights[i];
		}
		mSpotLights->Unmap(mDX11Device->md3dImmediateContext);
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::DrawScene()
{
	RJE_ASSERT(mDX11Device->md3dImmediateContext);
	RJE_ASSERT(mSwapChain);

	PROFILE_GPU_START(L"Render Scene");
	PROFILE_GPU_START_DEEP(L"DEEP Scene");

	if (mScene.mbDeferredRendering)
	{
		RenderGBuffer();
		RenderSkybox();
		RenderPostProcess();
	}
	else
	{
		RenderForward();
		RenderSkybox();
	}

	float blendFactor[4] = {mBlendFactorR, mBlendFactorG, mBlendFactorB, mBlendFactorA};
	mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sRasterizerState_Solid);
	mDX11Device->md3dImmediateContext->OMSetBlendState(DX11CommonStates::sBlendState_AlphaToCoverage, blendFactor, 0xffffffff);

	if (Console::Instance()->IsActive())
		DrawConsole();
	else if (Profiler::Instance()->IsActive())
		DrawProfiler();
	else if (!mScene.mbWireframe)
		Draw2dElements();

	//-------------------------------------------------------------------------

	PROFILE_GPU_END(L"Render Scene");
	PROFILE_GPU_END_DEEP(L"DEEP Scene");
	PROFILE_GPU_END_FRAME();

	//-------------------------------------------------------------------------

	// Draw the AntTweak GUI
	TwDraw();

	//-------------------------------------------------------------------------

	PROFILE_CPU("Present");
	if (VSyncEnabled)
	{
		RJE_CHECK_FOR_SUCCESS(mSwapChain->Present(1, 0));
	}
	else
	{
		RJE_CHECK_FOR_SUCCESS(mSwapChain->Present(0, 0));
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::RenderForward()
{
	PROFILE_CPU("Render Forward");
	PROFILE_GPU_START(L"Render Forward");

	mDX11Device->md3dImmediateContext->OMSetRenderTargets(1, &mBackbufferRTV, mDepthBuffer->GetDepthStencil());
	mDX11Device->md3dImmediateContext->ClearRenderTargetView(mBackbufferRTV, DirectX::Colors::Black);
	mDX11Device->md3dImmediateContext->ClearDepthStencilView(mDepthBuffer->GetDepthStencil(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	DrawGizmos();

	//-------------------------------------------------------------------------
	mDX11Device->md3dImmediateContext->IASetInputLayout(DX11InputLayouts::PosNormalTanTex);
	mDX11Device->md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float blendFactor[4] = {mBlendFactorR, mBlendFactorG, mBlendFactorB, mBlendFactorA};

	mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sCurrentRasterizerState);

	// Set constants
	Matrix44 view     = mCamera->mView;
	Matrix44 proj     = *(mCamera->mCurrentProjectionMatrix);

	// Set per frame constants.
	DX11Effects::BasicFX->SetViewProj(view*proj);
	DX11Effects::BasicFX->SetProj(proj);
	DX11Effects::BasicFX->SetEyePosW(mCamera->mTrf.Position);
	DX11Effects::BasicFX->UseFaceNormals(mScene.mbUseFaceNormals);
	DX11Effects::BasicFX->SetAmbientLight(mScene.mAmbientLightColor);
	DX11Effects::BasicFX->SetSamplerState(DX11CommonStates::sCurrentSamplerState);
	DX11Effects::BasicFX->SetFogColor(      mScene.mFogColor);
	DX11Effects::BasicFX->SetFogStart(      mScene.mFogStart);
	DX11Effects::BasicFX->SetFogRange(      mScene.mFogRange);
	DX11Effects::BasicFX->SetFogState(      mScene.mbUseFog);
	DX11Effects::BasicFX->SetAlphaClipState(mScene.mbUseBlending);
	DX11Effects::BasicFX->SetTextureState(  mScene.mbUseTexture);
	DX11Effects::BasicFX->SetDirLights(mDirLights->GetShaderResource());
	DX11Effects::BasicFX->SetPointLights(mPointLights->GetShaderResource());
	DX11Effects::BasicFX->SetSpotLights(mSpotLights->GetShaderResource());

	ID3DX11EffectTechnique* activeTech = DX11Effects::BasicFX->BasicTech;

	D3DX11_TECHNIQUE_DESC techDesc;

	activeTech->GetDesc( &techDesc );
	for(u32 p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the opaque geometry
		for(const unique_ptr<GameObject>& gameobject : mScene.mGameObjects)
		{
			if (gameobject->mDrawable.mMesh)
				gameobject->mDrawable.Render(activeTech->GetPassByIndex(p));
		}
		// Draw the transparent geometry
		for(const unique_ptr<GameObject>& gameobject_transparent : mScene.mGameObjects)
		{
			if (mScene.mbUseBlending)
				mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sRasterizerState_CullNone);
			//mDX11Device->md3dImmediateContext->OMSetBlendState(DX11CommonStates::sCurrentBlendState, blendFactor, 0xffffffff);

			if (gameobject_transparent->mDrawable.mMesh)
				gameobject_transparent->mDrawable.Render(activeTech->GetPassByIndex(p), false);
		}

		// Restore default render states
		mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sCurrentRasterizerState);
		mDX11Device->md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}

	PROFILE_GPU_END(L"Render Forward");
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::RenderGBuffer()
{
	PROFILE_CPU("Render G Buffer");
	PROFILE_GPU_START(L"Render G Buffer");

	mDX11Device->md3dImmediateContext->OMSetRenderTargets(static_cast<u32>(mGBufferRTV.size()), &mGBufferRTV.front(), mDepthBuffer->GetDepthStencil());
	for (ID3D11RenderTargetView* rtv : mGBufferRTV)
		mDX11Device->md3dImmediateContext->ClearRenderTargetView(rtv, DirectX::Colors::Black);
	mDX11Device->md3dImmediateContext->ClearDepthStencilView(mDepthBuffer->GetDepthStencil(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	DrawGizmos();

	mDX11Device->md3dImmediateContext->IASetInputLayout(DX11InputLayouts::PosNormalTanTex);
	mDX11Device->md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float blendFactor[4] = {mBlendFactorR, mBlendFactorG, mBlendFactorB, mBlendFactorA};

	mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sCurrentRasterizerState);

	// Set constants
	Matrix44 view     = mCamera->mView;
	Matrix44 proj     = *(mCamera->mCurrentProjectionMatrix);

	// Set per frame constants.
	DX11Effects::BasicFX->SetViewProj(view*proj);
	DX11Effects::BasicFX->SetProj(proj);
	DX11Effects::BasicFX->SetSamplerState(DX11CommonStates::sCurrentSamplerState);
	DX11Effects::BasicFX->UseFaceNormals(mScene.mbUseFaceNormals);

	//ID3DX11EffectTechnique* activeTech = DX11Effects::BasicFX->BasicTech;
	ID3DX11EffectTechnique* activeTech = DX11Effects::BasicFX->DeferredTech;

	D3DX11_TECHNIQUE_DESC techDesc;

	activeTech->GetDesc( &techDesc );
	for(u32 p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the opaque geometry
		for(const unique_ptr<GameObject>& gameobject : mScene.mGameObjects)
		{
			if (gameobject->mDrawable.mMesh)
				gameobject->mDrawable.Render(activeTech->GetPassByIndex(p));
		}
		// Draw the transparent geometry
		for(const unique_ptr<GameObject>& gameobject_transparent : mScene.mGameObjects)
		{
			if (mScene.mbUseBlending)
				mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sRasterizerState_CullNone);
			//mDX11Device->md3dImmediateContext->OMSetBlendState(DX11CommonStates::sCurrentBlendState, blendFactor, 0xffffffff);

			if (gameobject_transparent->mDrawable.mMesh)
				gameobject_transparent->mDrawable.Render(activeTech->GetPassByIndex(p), false);
		}

		// Restore default render states
		mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sCurrentRasterizerState);
		mDX11Device->md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}

	PROFILE_GPU_END(L"Render G Buffer");
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::RenderPostProcess()
{
	PROFILE_CPU("Render Post Process");
	PROFILE_GPU_START(L"Render Post Process");

	UINT stride = sizeof(PosNormTanTex);
	UINT offset = 0;

	mDX11Device->md3dImmediateContext->OMSetRenderTargets(1, &mBackbufferRTV, 0);
	mDX11Device->md3dImmediateContext->ClearRenderTargetView(mBackbufferRTV, DirectX::Colors::Black);
	mDX11Device->md3dImmediateContext->RSSetState(DX11CommonStates::sRasterizerState_Solid);

	DX11Effects::PostProcessFX->SetEyePosW(mCamera->mTrf.Position);
	DX11Effects::PostProcessFX->OnlyPosition( mScene.mbOnlyPosition);
	DX11Effects::PostProcessFX->OnlyAlbedo(   mScene.mbOnlyAlbedo);
	DX11Effects::PostProcessFX->OnlyNormals(  mScene.mbOnlyNormals);
	DX11Effects::PostProcessFX->OnlySpecular( mScene.mbOnlySpecular);
	DX11Effects::PostProcessFX->SetPerSampleShading(mScene.mbViewPerSampleShading);
	DX11Effects::PostProcessFX->SetFogColor(      mScene.mFogColor);
	DX11Effects::PostProcessFX->SetFogStart(      mScene.mFogStart);
	DX11Effects::PostProcessFX->SetFogRange(      mScene.mFogRange);
	DX11Effects::PostProcessFX->SetFogState(      mScene.mbUseFog);
	DX11Effects::PostProcessFX->SetTextureState(  mScene.mbUseTexture);
	DX11Effects::PostProcessFX->SetAmbientLight(mScene.mAmbientLightColor);
	DX11Effects::PostProcessFX->SetDirLights(  mDirLights->GetShaderResource());
	DX11Effects::PostProcessFX->SetPointLights(mPointLights->GetShaderResource());
	DX11Effects::PostProcessFX->SetSpotLights( mSpotLights->GetShaderResource());

	ID3DX11EffectTechnique* PostProcessTech = DX11Effects::PostProcessFX->ResolveDeferredTech;
	D3DX11_TECHNIQUE_DESC techDesc;

	PostProcessTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mDX11Device->md3dImmediateContext->IASetVertexBuffers(0, 1, &mScreenQuadVB, &stride, &offset);
		mDX11Device->md3dImmediateContext->IASetIndexBuffer(mScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);

		//DX11Effects::PostProcessFX->SetTextureMap(mRjeLogo);
		DX11Effects::PostProcessFX->SetGuffer(mGBufferSRV);

		PostProcessTech->GetPassByIndex(p)->Apply(0, mDX11Device->md3dImmediateContext);
		mDX11Device->md3dImmediateContext->DrawIndexed(6, 0, 0);
	}
	ID3D11ShaderResourceView* nullViews[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	mDX11Device->md3dImmediateContext->PSSetShaderResources(0, 5, nullViews);

	PROFILE_GPU_END(L"Render Post Process");
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::RenderSkybox()
{
	Vector3 eyePos = mCamera->mTrf.Position;
	Matrix44 T = Matrix44::Translation(eyePos);

	Matrix44 view = mCamera->mView;
	Matrix44 proj = *(mCamera->mCurrentProjectionMatrix);
	Matrix44 WVP  = T*view*proj;

	DX11Effects::SkyboxFX->SetWorldViewProj(WVP);
	DX11Effects::SkyboxFX->SetCubeMap(mSkyboxSRV);

	u32 stride = sizeof(PosNormTanTex);
	u32 offset = 0;
	mDX11Device->md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkyboxVB, &stride, &offset);
	mDX11Device->md3dImmediateContext->IASetIndexBuffer(mSkyboxIB, DXGI_FORMAT_R32_UINT, 0);
	mDX11Device->md3dImmediateContext->IASetInputLayout(DX11InputLayouts::PosNormalTanTex);
	mDX11Device->md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc;
	DX11Effects::SkyboxFX->SkyboxForwardTech->GetDesc( &techDesc );

	for(u32 p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = DX11Effects::SkyboxFX->SkyboxForwardTech->GetPassByIndex(p);

		pass->Apply(0, mDX11Device->md3dImmediateContext);

		mDX11Device->md3dImmediateContext->DrawIndexed(36, 0, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::DrawGizmos()
{
	PROFILE_CPU("Draw Gizmos");

	PROFILE_GPU_START(L"Render Gizmos");

	mDX11Device->md3dImmediateContext->IASetInputLayout(DX11InputLayouts::PosColor);
	mDX11Device->md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	// Set constants
	Matrix44 view     = mCamera->mView;
	Matrix44 proj     = *(mCamera->mCurrentProjectionMatrix);
	DX11Effects::ColorFX->SetViewProj(view*proj);
	
	// Draw the gizmo geometry
	for(const unique_ptr<GameObject>& gizmo : mScene.mGameObjects)
	{
		if (gizmo->mDrawable.mGizmo)
			gizmo->mDrawable.RenderGizmo(DX11Effects::ColorFX->ColorTech->GetPassByIndex(0));
	}

	PROFILE_GPU_END(L"Render Gizmos");
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::DrawConsole()
{
	PROFILE_CPU("Draw Console");

	char cmd[COMMAND_MAX_LENGTH];
	Console::Instance()->GetCommand(cmd);

	int consoleElevation = (int)Console::Instance()->mConsoleElevation;
	POINT textPos = {10, 5 + (LINE_MAX-Console::Instance()->GetLineCount()) * mConsoleFont->GetCharHeight() - consoleElevation};
	POINT cmdPos  = {10, CONSOLE_HEIGHT-25 - consoleElevation};

	CD3D11_RECT rect( 0, -consoleElevation, System::Instance()->mScreenWidth, CONSOLE_HEIGHT-consoleElevation);
	CD3D11_RECT rectLogo( System::Instance()->mScreenWidth - 220, 40-consoleElevation, System::Instance()->mScreenWidth - 30, 160-consoleElevation);
	
	if (mScene.mbDeferredRendering)
	{
		mSpriteBatch->DrawTexture2D(DX11TextureManager::Instance()->mTextures["_transparentGray"], mDX11Device->md3dImmediateContext, rect,     0xffffffff);
		mSpriteBatch->DrawTexture2D(mRjeLogo,                                                      mDX11Device->md3dImmediateContext, rectLogo, 0xffffffff);
	}
	
	mSpriteBatch->DrawInfoText(      *mConsoleFont, Console::Instance()->mConsoleBuffer, textPos);
	mSpriteBatch->DrawConsoleCommand(*mConsoleFont, cmd, cmdPos);

	if (!mScene.mbDeferredRendering)
	{
		mSpriteBatch->DrawTexture2D(mRjeLogo,                                                      mDX11Device->md3dImmediateContext, rectLogo, 0xffffffff);
		mSpriteBatch->DrawTexture2D(DX11TextureManager::Instance()->mTextures["_transparentGray"], mDX11Device->md3dImmediateContext, rect,     0xffffffff);
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::DrawProfiler()
{
	PROFILE_CPU("Draw Profiler");

	POINT profileStatsPos = {System::Instance()->mScreenWidth - 550, 20};
	POINT profileInfoPos  = {20, 70};

	std::wstring stats;
	stats =  L" CPU : " + AnsiToWString(System::Instance()->mCpuDescription) + L"\n";
	stats += L" GPU : " + wstring(System::Instance()->mGpuDescription)       + L"\n";
	stats += L" RAM : " + ToString(System::Instance()->mTotalSystemRAM)      + L" MB\n";
	stats += L"VRAM : " + ToString(System::Instance()->mGpuDedicatedVRAM)    + L" MB (Dedicated)\n";
	stats += L"VRAM : " + ToString(System::Instance()->mGpuSharedVRAM)       + L" MB (Shared)";
	mSpriteBatch->DrawString(*mProfilerFont, stats, profileStatsPos, XMCOLOR(0xffffff00));
	//---------------
	profileStatsPos.x = profileStatsPos.y = 20;
	stats =  L"FPS : "        + ToString(System::Instance()->fps);
	stats += L" (Min:"        + ToString(System::Instance()->minfps);
	stats += L" - Max:"       + ToString(System::Instance()->maxfps) + L")\n";
	stats += L"Time/Frame : " + ToString(System::Instance()->mspf) + L" ms";
	stats += L" (frame "      + ToString(System::Instance()->mTotalFrames) + L")";
	mSpriteBatch->DrawString(*mProfilerFont, stats, profileStatsPos, XMCOLOR(0xffffff00));
	//-----------------------------
	if (Profiler::Instance()->GetState() == PROFILER_STATES::E_GPU)
	{
#if RJE_PROFILE_GPU
		DX11Profiler::sInstance.GetProfilerInfo();
		mSpriteBatch->DrawInfoText(*mProfilerFont, DX11Profiler::sInstance.mProfileInfoString, profileInfoPos);
		profileInfoPos.x += 350;
		profileInfoPos.y = 125;
		mSpriteBatch->DrawInfoText(*mProfilerFont, DX11Profiler::sInstance.mProfileDeepInfoString, profileInfoPos);
#else
		mSpriteBatch->DrawInfoText(*mProfilerFont, "GPU Profiling Not Enabled", profileInfoPos);
#endif
	}
	else 
	{
#if RJE_PROFILE_CPU
		Profiler::Instance()->GetProfilerInfo();
		mSpriteBatch->DrawInfoText(*mProfilerFont, Profiler::Instance()->mProfileInfoString, profileInfoPos);
#else
		mSpriteBatch->DrawInfoText(*mProfilerFont, "CPU and Memory Profiling Not Enabled", profileInfoPos);
#endif
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::Draw2dElements()
{
	POINT textPos = {10, 10};

	std::wstring lightStat;
	if (mLightMode == LightMode::Point)			lightStat = L" Number of point lights : "       + ToString(mPointLightCount);
	if (mLightMode == LightMode::Directional)	lightStat = L" Number of directional lights : " + ToString(mDirLightCount);
	if (mLightMode == LightMode::Spot)			lightStat = L" Number of spot lights : "        + ToString(mSpotLightCount);
	mSpriteBatch->DrawString(*mProfilerFont, lightStat, textPos, XMCOLOR(0xffffffff));
}


//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::SetActiveDirLights(u32 activeLights)
{
	mDirLightCount = activeLights;
	RJE_SAFE_DELETE(mDirLights);
	mDirLights = rje_new StructuredBuffer<DirectionalLight>(mDX11Device->md3dDevice, activeLights, D3D11_BIND_SHADER_RESOURCE, true);
}
//-------------------------
void DX11RenderingAPI::SetActivePointLights(u32 activeLights)
{
	mPointLightCount = activeLights;
	RJE_SAFE_DELETE(mPointLights);
	mPointLights = rje_new StructuredBuffer<PointLight>(mDX11Device->md3dDevice, activeLights, D3D11_BIND_SHADER_RESOURCE, true);
}
//-------------------------
void DX11RenderingAPI::SetActiveSpotLights(u32 activeLights)
{
	mSpotLightCount = activeLights;
	RJE_SAFE_DELETE(mSpotLights);
	mSpotLights = rje_new StructuredBuffer<SpotLight>(mDX11Device->md3dDevice, activeLights, D3D11_BIND_SHADER_RESOURCE, true);
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::Shutdown()
{
	//-----------
	// Terminate AntTweak GUI
	TwTerminate();
	//-----------
	
	PROFILE_GPU_EXIT();

	DX11TextureManager::DeleteInstance();
	RJE_SAFE_RELEASE(mRjeLogo);

	DX11Effects     ::DestroyAll();
	DX11InputLayouts::DestroyAll();
	DX11CommonStates::DestroyAll();

	RJE_SAFE_DELETE(mDirLights);
	RJE_SAFE_DELETE(mPointLights);
	RJE_SAFE_DELETE(mSpotLights);

	RJE_SAFE_RELEASE(mScreenQuadVB);
	RJE_SAFE_RELEASE(mScreenQuadIB);
	RJE_SAFE_RELEASE(mSkyboxVB);
	RJE_SAFE_RELEASE(mSkyboxIB);
	RJE_SAFE_RELEASE(mSkyboxSRV);

	RJE_SAFE_DELETE(mSpriteBatch);
	RJE_SAFE_DELETE(mConsoleFont);
	RJE_SAFE_DELETE(mProfilerFont);

	RJE_SAFE_RELEASE(mBackbufferRTV);
	RJE_SAFE_RELEASE(mSwapChain);

	//------- Not used for now
// #ifdef RJE_DEBUG
// 	DXGI_DEBUG_RLO_FLAGS dbg_flags = DXGI_DEBUG_RLO_ALL;
// 	HRESULT result;
// 	// TODO: find why this is always returning E_INVALIDARG
// 	result = md3dDebug->ReportLiveObjects( DXGI_DEBUG_ALL, dbg_flags );
// #endif

	mDX11Device->Release();

	for(Texture2D* tex2D : mGBuffer)
		RJE_SAFE_DELETE(tex2D);

	RJE_SAFE_DELETE(mDepthBuffer);
	RJE_SAFE_DELETE(mDX11CommonStates);
	RJE_SAFE_DELETE(mDX11Device);
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::BuildScreenQuad()
{
	MeshData::Data<PosNormTanTex> quad;

	GeometryGenerator geoGen;
	geoGen.CreateFullscreenQuad(quad);

	std::vector<PosNormTanTex> vertices(quad.Vertices.size());

	for(u32 i = 0; i < quad.Vertices.size(); ++i)
	{
		vertices[i].Position = quad.Vertices[i].Position;
		vertices[i].Normal   = quad.Vertices[i].Normal;
		vertices[i].TexC     = quad.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage          = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth      = (u32) (sizeof(PosNormTanTex) * quad.Vertices.size());
	vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateBuffer(&vbd, &vinitData, &mScreenQuadVB));
	//-----------
	D3D11_BUFFER_DESC ibd;
	ibd.Usage          = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth      = (u32) (sizeof(u32) * quad.Indices.size());
	ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &quad.Indices[0];
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateBuffer(&ibd, &iinitData, &mScreenQuadIB));
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::BuildSkybox()
{
	MeshData::Data<PosNormTanTex> skyboxData;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1, 1, 1, skyboxData);

	std::vector<PosNormTanTex> vertices(skyboxData.Vertices.size());

	for(u32 i = 0; i < skyboxData.Vertices.size(); ++i)
	{
		vertices[i].Position = skyboxData.Vertices[i].Position;
		vertices[i].Normal   = skyboxData.Vertices[i].Normal;
		vertices[i].TexC     = skyboxData.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage          = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth      = (u32) (sizeof(PosNormTanTex) * skyboxData.Vertices.size());
	vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkyboxVB));
	//-----------
	D3D11_BUFFER_DESC ibd;
	ibd.Usage          = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth      = (u32) (sizeof(u32) * skyboxData.Indices.size());
	ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &skyboxData.Indices[0];
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkyboxIB));
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::BuildDepthBuffer(DXGI_SAMPLE_DESC sampleDesc)
{
	RJE_SAFE_DELETE(mDepthBuffer);
	mDepthBuffer = rje_new Depth2D( mDX11Device->md3dDevice, mWindowWidth, mWindowHeight, D3D11_BIND_DEPTH_STENCIL, sampleDesc, MSAA_Samples > 1);
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::BuildGBuffer(DXGI_SAMPLE_DESC sampleDesc)
{
	u32 gBufferWidth  = mWindowWidth;
	u32 gBufferHeight = mWindowHeight;

	// Create/recreate any textures related to screen size
	for (std::size_t i = 0; i < mGBuffer.size(); ++i)
	{
		RJE_SAFE_DELETE(mGBuffer[i]);
	}
	mGBuffer.resize(0);
	mGBufferRTV.resize(0);
	mGBufferSRV.resize(0);

	// === G-Buffer ===
	// Position / Albedo / Normal / Specular
	mGBuffer.push_back(rje_new Texture2D( mDX11Device->md3dDevice, gBufferWidth, gBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM     , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, sampleDesc));
	mGBuffer.push_back(rje_new Texture2D( mDX11Device->md3dDevice, gBufferWidth, gBufferHeight, DXGI_FORMAT_R32G32B32A32_FLOAT , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, sampleDesc));
	mGBuffer.push_back(rje_new Texture2D( mDX11Device->md3dDevice, gBufferWidth, gBufferHeight, DXGI_FORMAT_R32G32B32A32_FLOAT , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, sampleDesc));
	mGBuffer.push_back(rje_new Texture2D( mDX11Device->md3dDevice, gBufferWidth, gBufferHeight, DXGI_FORMAT_R32G32_FLOAT ,       D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, sampleDesc));

	// Set up GBuffer resource list
	mGBufferRTV.resize(mGBuffer.size(), 0);
	mGBufferSRV.resize(mGBuffer.size(), 0);
	//mGBufferSRV.resize(mGBuffer.size()+1, 0);
	for (std::size_t i = 0; i < mGBuffer.size(); ++i)
	{
		mGBufferRTV[i] = mGBuffer[i]->GetRenderTarget();
		mGBufferSRV[i] = mGBuffer[i]->GetShaderResource();
	}
	// Depth buffer is the last SRV that we use for reading
	//mGBufferSRV.back() = mDepthBuffer->GetShaderResource();
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::ResizeWindow() {ResizeWindow(mWindowWidth, mWindowHeight);}
//-------------------------------------------
void DX11RenderingAPI::ResizeWindow(int newSizeWidth, int newSizeHeight)
{
	mWindowWidth  = newSizeWidth;
	mWindowHeight = newSizeHeight;

	RJE_ASSERT(mDX11Device->md3dImmediateContext);
	RJE_ASSERT(mDX11Device->md3dDevice);
	RJE_ASSERT(mSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	RJE_SAFE_RELEASE(mBackbufferRTV);

	// Resize the swap chain and recreate the render target view.
	RJE_CHECK_FOR_SUCCESS(mSwapChain->ResizeBuffers(1, newSizeWidth, newSizeHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))
	ID3D11Texture2D* backBuffer;
	RJE_CHECK_FOR_SUCCESS(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateRenderTargetView(backBuffer, 0, &mBackbufferRTV));
	RJE_SAFE_RELEASE(backBuffer);

	// Set the viewport transform.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast<float>(newSizeWidth);
	mScreenViewport.Height   = static_cast<float>(newSizeHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mDX11Device->md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count   = MSAA_Samples;
	sampleDesc.Quality = 0;
	BuildDepthBuffer(sampleDesc);
	BuildGBuffer(sampleDesc);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	mCamera->mSettings.AspectRatio = (float)newSizeWidth / (float)newSizeWidth;
	mCamera->UpdateProjMatrix((float)newSizeWidth, (float)newSizeHeight);
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::SetWireframe(BOOL state)
{
	if (state)
	{
		mScene.mbUseBlending = false;
		mScene.mbWireframe   = true;
		DX11CommonStates::sCurrentRasterizerState = DX11CommonStates::sRasterizerState_Wireframe;
		DX11CommonStates::sCurrentBlendState      = DX11CommonStates::sBlendState_Opaque;
	}
	else
	{
		mScene.mbWireframe = false;
		DX11CommonStates::sCurrentRasterizerState = DX11CommonStates::sRasterizerState_Solid;
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11RenderingAPI::SetMSAA(u32 MSAASamples)
{
	InitSwapChain(MSAASamples);
	System::Instance()->OnResize();
}


//////////////////////////////////////////////////////////////////////////
// AntTweak CallBacks
//////////////////////////////////////////////////////////////////////////
void TW_CALL TwSetWireframe(void *clientData)
{
	DX11RenderingAPI* api = static_cast<DX11RenderingAPI*>(clientData);
	api->SetWireframe(!api->mScene.mbWireframe);
}