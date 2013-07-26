#include "DX11Wrapper.h"

DX11Wrapper::DX11Wrapper()
{
	mDX11Device			= nullptr;
	mDX11DepthBuffer	= nullptr;
	mRenderTargetView	= nullptr;
	md3dDriverType		= D3D_DRIVER_TYPE_HARDWARE;
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));

	mVertexBuffer = nullptr;
	mIndexBuffer  = nullptr;
	
	mBoxMap       = nullptr;
	mGridMap      = nullptr;
	mSphereMap    = nullptr;
	mCylinderMap  = nullptr;
	mMaskMap      = nullptr;
	mWhiteMaskMap = nullptr;
	
	mRasterizerState_Solid     = nullptr;
	mRasterizerState_Wireframe = nullptr;

	mDirLightCount   = 1;
	mPointLightCount = 0;
	mEyePosW = XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX Id = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, Id);
	XMStoreFloat4x4(&mView, Id);
	XMStoreFloat4x4(&mProj, Id);

	// Texture transforms
	XMMATRIX gridTexScale   = XMMatrixScaling(5.0f, 5.0f, 0.0f);
	XMMATRIX boxTexScale    = XMMatrixScaling(1.0f, 1.0f, 0.0f);
	XMMATRIX cylTexScale    = boxTexScale;
	XMMATRIX sphereTexScale = boxTexScale;
	XMStoreFloat4x4(&mGridTexTransform,     gridTexScale);
	XMStoreFloat4x4(&mBoxTexTransform,      boxTexScale);
	XMStoreFloat4x4(&mCylinderTexTransform, cylTexScale);
	XMStoreFloat4x4(&mSphereTexTransform,   sphereTexScale);

	// Meshes transforms
	XMMATRIX boxScale = XMMatrixScaling(2.0f, 1.0f, 2.0f);
	XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
	XMStoreFloat4x4(&mBoxWorld, XMMatrixMultiply(boxScale, boxOffset));

	XMMATRIX dragonScale    = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	XMMATRIX dragonOffset   = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMMATRIX dragonRotation = XMMatrixRotationY(-45);
	dragonScale = XMMatrixMultiply(dragonScale, dragonOffset);
	XMStoreFloat4x4(&mModelWorld, XMMatrixMultiply(dragonScale, dragonRotation));

	for(int i = 0; i < 5; ++i)
	{
		XMStoreFloat4x4(&mCylWorld[i*2+0], XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mCylWorld[i*2+1], XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i*5.0f));

		XMStoreFloat4x4(&mSphereWorld[i*2+0], XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mSphereWorld[i*2+1], XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i*5.0f));
	}

	mPointLights[0].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mPointLights[0].Diffuse  = XMFLOAT4(0.1f, 0.1f, 0.75f, 1.0f);
	mPointLights[0].Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLights[0].Att      = XMFLOAT3(0.175f, 0.175f, 0.175f);
	mPointLights[0].Range    = 50.0f;

	mPointLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mPointLights[1].Diffuse  = XMFLOAT4(0.75f, 0.1f, 0.1f, 1.0f);
	mPointLights[1].Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLights[1].Att      = XMFLOAT3(0.175f, 0.175f, 0.175f);
	mPointLights[1].Range    = 50.0f;

	mPointLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mPointLights[2].Diffuse  = XMFLOAT4(0.1f, 0.75f, 0.1f, 1.0f);
	mPointLights[2].Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLights[2].Att      = XMFLOAT3(0.175f, 0.175f, 0.175f);
	mPointLights[2].Range    = 50.0f;

	mDirLights[0].Ambient   = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse   = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient   = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse   = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular  = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient   = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse   = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	// Material Specs
	mGridMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mGridMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mGridMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mCylinderMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mCylinderMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mCylinderMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mSphereMat.Ambient  = XMFLOAT4(0.6f, 0.8f, 0.9f, 1.0f);
	mSphereMat.Diffuse  = XMFLOAT4(0.6f, 0.8f, 0.9f, 1.0f);
	mSphereMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mBoxMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mBoxMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mModelMat.Ambient  = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mModelMat.Diffuse  = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mModelMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

//////////////////////////////////////////////////////////////////////////
void DX11Wrapper::Initialize(HWND hMainWnd, int windowWidth, int windowHeight)
{
	mDX11Device       = new DX11Device;
	mDX11DepthBuffer  = new DX11DepthBuffer;
	mDX11CommonStates = new DX11CommonStates;

	// Create the device and device context.
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
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

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &RJE_GLOBALS::g4xMsaaQuality));
	RJE_ASSERT( RJE_GLOBALS::g4xMsaaQuality > 0 );

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width						= windowWidth;
	sd.BufferDesc.Height					= windowHeight;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	//sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if( RJE_GLOBALS::gUse4xMsaa )
	{
		sd.SampleDesc.Count   = 4;
		sd.SampleDesc.Quality = (RJE_GLOBALS::g4xMsaaQuality)-1;
	}
	else // No MSAA
	{
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = hMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."
	IDXGIDevice* dxgiDevice = nullptr;
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = nullptr;
	RJE_CHECK_FOR_SUCCESS(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = nullptr;
	RJE_CHECK_FOR_SUCCESS(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	RJE_CHECK_FOR_SUCCESS(dxgiFactory->CreateSwapChain(mDX11Device->md3dDevice, &sd, &mSwapChain));
	RJE_CHECK_FOR_SUCCESS(dxgiFactory->MakeWindowAssociation(hMainWnd, DXGI_MWA_NO_WINDOW_CHANGES));

	RJE_SAFE_RELEASE(dxgiDevice);
	RJE_SAFE_RELEASE(dxgiAdapter);
	RJE_SAFE_RELEASE(dxgiFactory);

	ResizeWindow(windowWidth, windowHeight);	// we call the ResizeWindow method here to avoid code duplication.

	// We init the effect first and then the input layouts
	Effects::InitAll(mDX11Device->md3dDevice);
	InputLayouts::InitAll(mDX11Device->md3dDevice);

	// Load textures
	RJE_CHECK_FOR_SUCCESS(CreateDDSTextureFromFile(	mDX11Device->md3dDevice,
													CIniFile::GetValueWchar("box", "textures", "..\\..\\RamJamEngine\\data\\Resources.ini"),
													nullptr,
													&mBoxMap));
	RJE_CHECK_FOR_SUCCESS(CreateDDSTextureFromFile(	mDX11Device->md3dDevice,
													CIniFile::GetValueWchar("grid", "textures", "..\\..\\RamJamEngine\\data\\Resources.ini"),
													nullptr,
													&mGridMap));
	RJE_CHECK_FOR_SUCCESS(CreateDDSTextureFromFile(	mDX11Device->md3dDevice,
													CIniFile::GetValueWchar("sphere", "textures", "..\\..\\RamJamEngine\\data\\Resources.ini"),
													nullptr,
													&mSphereMap));
	RJE_CHECK_FOR_SUCCESS(CreateDDSTextureFromFile(	mDX11Device->md3dDevice,
													CIniFile::GetValueWchar("cylinder", "textures", "..\\..\\RamJamEngine\\data\\Resources.ini"),
													nullptr,
													&mCylinderMap));
	RJE_CHECK_FOR_SUCCESS(CreateDDSTextureFromFile(	mDX11Device->md3dDevice,
													CIniFile::GetValueWchar("mask", "textures", "..\\..\\RamJamEngine\\data\\Resources.ini"),
													nullptr,
													&mMaskMap));
	RJE_CHECK_FOR_SUCCESS(CreateDDSTextureFromFile(	mDX11Device->md3dDevice,
													CIniFile::GetValueWchar("whitemask", "textures", "..\\..\\RamJamEngine\\data\\Resources.ini"),
													nullptr,
													&mWhiteMaskMap));

	BuildGeometryBuffers();

	//////////////////////////////////////////////////////////////////////////
	// Create the common states
	RJE_CHECK_FOR_SUCCESS(mDX11CommonStates->Wireframe(           mDX11Device->md3dDevice, &mRasterizerState_Wireframe));
	RJE_CHECK_FOR_SUCCESS(mDX11CommonStates->CullCounterClockwise(mDX11Device->md3dDevice, &mRasterizerState_Solid));
	mCurrentRasterizerState = mRasterizerState_Solid;

	RJE_CHECK_FOR_SUCCESS(mDX11CommonStates->AnisotropicWrap(mDX11Device->md3dDevice, &mSamplerState_Anisotropic));
	RJE_CHECK_FOR_SUCCESS(mDX11CommonStates->LinearWrap(mDX11Device->md3dDevice, &mSamplerState_Linear));
	mCurrentSamplerState = mSamplerState_Anisotropic;
}

//////////////////////////////////////////////////////////////////////////
void DX11Wrapper::BuildGeometryBuffers()
{
	std::ifstream fin(CIniFile::GetValue("modelpath", "meshes", "..\\..\\RamJamEngine\\data\\Resources.ini"));

	if(!fin)
	{
		RJE_MESSAGE_BOX(0, L"dragon.mesh not found.", 0, 0);
		return;
	}

	UINT dragonVertexCount = 0;
	UINT dragonTriangleCount = 0;
	std::string ignore;

	fin >> ignore >> ignore >> dragonVertexCount;
	fin >> ignore >> ignore >> dragonTriangleCount;
	fin >> ignore;

	mModelIndexCount = 3*dragonTriangleCount;

	GeometryGenerator::MeshData box;
	GeometryGenerator::MeshData grid;
	GeometryGenerator::MeshData sphere;
	GeometryGenerator::MeshData cylinder;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);
	geoGen.CreateGrid(20.0f, 30.0f, 60, 40, grid);
	//geoGen.CreateSphere(0.5f, 20, 20, sphere);
	geoGen.CreateGeosphere(0.5f, 3, sphere);
	geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset      = 0;
	mGridVertexOffset     = (int) box.Vertices.size();
	mSphereVertexOffset   = (int) (mGridVertexOffset + grid.Vertices.size());
	mCylinderVertexOffset = (int) (mSphereVertexOffset + sphere.Vertices.size());
	mModelVertexOffset    = (int) (mCylinderVertexOffset + cylinder.Vertices.size());

	// Cache the index count of each object.
	mBoxIndexCount      = (UINT) box.Indices.size();
	mGridIndexCount     = (UINT) grid.Indices.size();
	mSphereIndexCount   = (UINT) sphere.Indices.size();
	mCylinderIndexCount = (UINT) cylinder.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset      = 0;
	mGridIndexOffset     = mBoxIndexCount;
	mSphereIndexOffset   = mGridIndexOffset + mGridIndexCount;
	mCylinderIndexOffset = mSphereIndexOffset + mSphereIndexCount;
	mModelIndexOffset    = mCylinderIndexOffset + mCylinderIndexCount;

	UINT totalVertexCount = (UINT) box.Vertices.size() +
							(UINT) grid.Vertices.size() +
							(UINT) sphere.Vertices.size() +
							(UINT) cylinder.Vertices.size() +
							dragonVertexCount;

	UINT totalIndexCount =  mBoxIndexCount +
							mGridIndexCount + 
							mSphereIndexCount +
							mCylinderIndexCount +
							mModelIndexCount;


	std::vector<Vertex::PosNormalTex> vertices(totalVertexCount);
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
	UINT k = 0;

	for(size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Tex    = box.Vertices[i].TexC;
	}
	for(size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].Tex    = grid.Vertices[i].TexC;
	}
	for(size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].Tex    = sphere.Vertices[i].TexC;
	}
	for(size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].Tex    = cylinder.Vertices[i].TexC;
	}
	for(size_t i = 0; i < dragonVertexCount; ++i, ++k)
	{
		fin >> vertices[k].Pos.x >> vertices[k].Pos.y >> vertices[k].Pos.z;

		//XMFLOAT4 rnd(RJE::Math::RandF(), RJE::Math::RandF(), RJE::Math::RandF(), 1.0f);
		//vertices[k].Color = rnd;

		fin >> vertices[k].Tex.x >> vertices[k].Tex.y;
		fin >> vertices[k].Normal.x >> vertices[k].Normal.y >> vertices[k].Normal.z;
	}
	std::vector<UINT> dragonIndices(mModelIndexCount);
	for(UINT i = 0; i < dragonTriangleCount; ++i)
	{
		fin >> dragonIndices[i*3+0] >> dragonIndices[i*3+1] >> dragonIndices[i*3+2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT) (sizeof(Vertex::PosNormalTex) * totalVertexCount);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuffer));


	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());
	indices.insert(indices.end(), dragonIndices.begin(), dragonIndices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

//////////////////////////////////////////////////////////////////////////
void DX11Wrapper::UpdateScene( float dt, float theta, float phi, float radius )
{
	// Convert Spherical to Cartesian coordinates.
	float x = radius*sinf(phi)*cosf(theta);
	float z = radius*sinf(phi)*sinf(theta);
	float y = radius*cosf(phi);

	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	DirectX::XMVECTOR pos    = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up     = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);

	// TODO : No Hard Coding Key Events !!!
	//
	// Switch the number of lights based on key presses.
	//
	if( GetAsyncKeyState('0') & 0x8000 )
		mDirLightCount = 0; 

	if( GetAsyncKeyState('1') & 0x8000 )
		mDirLightCount = 1; 

	if( GetAsyncKeyState('2') & 0x8000 )
		mDirLightCount = 2; 

	if( GetAsyncKeyState('3') & 0x8000 )
		mDirLightCount = 3;

	if( GetAsyncKeyState('4') & 0x8000 )
		mPointLightCount = 0; 

	if( GetAsyncKeyState('5') & 0x8000 )
		mPointLightCount = 1; 

	if( GetAsyncKeyState('6') & 0x8000 )
		mPointLightCount = 2; 

	if( GetAsyncKeyState('7') & 0x8000 )
		mPointLightCount = 3;

	if ( GetAsyncKeyState(VK_RETURN) & 0x8000 )
		mCurrentRasterizerState = mRasterizerState_Solid;

	if ( GetAsyncKeyState(VK_BACK) & 0x8000 )
		mCurrentRasterizerState = mRasterizerState_Wireframe;

	if ( GetAsyncKeyState(0x41) & 0x8000 )	// A key
		mCurrentSamplerState = mSamplerState_Anisotropic;

	if ( GetAsyncKeyState(0x5A) & 0x8000 )	// Z key
		mCurrentSamplerState = mSamplerState_Linear;

	static float timer = 0.0f;
	timer += dt*0.5f;

	mPointLights[0].Position.x = 3.0f*cosf(2*RJE::Math::Pi/3 + timer );
	mPointLights[0].Position.z = 3.0f*sinf(2*RJE::Math::Pi/3 + timer );
	mPointLights[0].Position.y = 2.0f + cosf( timer );

	mPointLights[1].Position.x = 3.0f*cosf(-2*RJE::Math::Pi/3 + timer );
	mPointLights[1].Position.z = 3.0f*sinf(-2*RJE::Math::Pi/3 + timer );
	mPointLights[1].Position.y = 2.0f + cosf( timer );

	mPointLights[2].Position.x = 3.0f*cosf( timer );
	mPointLights[2].Position.z = 3.0f*sinf( timer );
	mPointLights[2].Position.y = 2.0f + cosf( timer );
}

//////////////////////////////////////////////////////////////////////////
void DX11Wrapper::DrawScene()
{
	RJE_ASSERT(mDX11Device->md3dImmediateContext);
	RJE_ASSERT(mSwapChain);

	mDX11Device->md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::LightSteelBlue);
	mDX11Device->md3dImmediateContext->ClearDepthStencilView(mDX11DepthBuffer->mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDX11Device->md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTex);
	mDX11Device->md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDX11Device->md3dImmediateContext->RSSetState(mCurrentRasterizerState);

	UINT stride = sizeof(Vertex::PosNormalTex);
	UINT offset = 0;
	mDX11Device->md3dImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDX11Device->md3dImmediateContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	DirectX::XMMATRIX view     = XMLoadFloat4x4(&mView);
	DirectX::XMMATRIX proj     = XMLoadFloat4x4(&mProj);
	DirectX::XMMATRIX viewProj = view*proj;

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetPointLights(mPointLights);
	Effects::BasicFX->SetEyePosW(mEyePosW);
	Effects::BasicFX->SetSamplerState(mCurrentSamplerState);

	// Figure out which technique to use. The model doesn't have any text coordinates so we use another technique
	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light0_0Tech;
	ID3DX11EffectTechnique* activeTechModel = Effects::BasicFX->Light1_3NoTexTech;
	
	// WARNING : Dir & Point Light < 3
	switch(mDirLightCount)
	{
	case 0:
		switch (mPointLightCount)
		{
		case 0:
			activeTech = Effects::BasicFX->Light0_0Tech;
			break;
		case 1:
			activeTech = Effects::BasicFX->Light0_1Tech;
			break;
		case 2:
			activeTech = Effects::BasicFX->Light0_2Tech;
			break;
		case 3:
			activeTech = Effects::BasicFX->Light0_3Tech;
			break;
		}
		break;
	case 1:
		switch (mPointLightCount)
		{
		case 0:
			activeTech = Effects::BasicFX->Light1_0Tech;
			break;
		case 1:
			activeTech = Effects::BasicFX->Light1_1Tech;
			break;
		case 2:
			activeTech = Effects::BasicFX->Light1_2Tech;
			break;
		case 3:
			activeTech = Effects::BasicFX->Light1_3Tech;
			break;
		}
		break;
	case 2:
		switch (mPointLightCount)
		{
		case 0:
			activeTech = Effects::BasicFX->Light2_0Tech;
			break;
		case 1:
			activeTech = Effects::BasicFX->Light2_1Tech;
			break;
		case 2:
			activeTech = Effects::BasicFX->Light2_2Tech;
			break;
		case 3:
			activeTech = Effects::BasicFX->Light2_3Tech;
			break;
		}
		break;
	case 3:
		switch (mPointLightCount)
		{
		case 0:
			activeTech = Effects::BasicFX->Light3_0Tech;
			break;
		case 1:
			activeTech = Effects::BasicFX->Light3_1Tech;
			break;
		case 2:
			activeTech = Effects::BasicFX->Light3_2Tech;
			break;
		case 3:
			activeTech = Effects::BasicFX->Light3_3Tech;
			break;
		}
		break;
	}

	D3DX11_TECHNIQUE_DESC techDesc;
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;

	activeTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{		
		// Draw the grid.
		world             = XMLoadFloat4x4(&mGridWorld);
		worldInvTranspose = DX11Math::InverseTranspose(world);
		worldViewProj     = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&mGridTexTransform));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(mGridMap);
		Effects::BasicFX->SetMaskMap(mMaskMap);

		activeTech->GetPassByIndex(p)->Apply(0, mDX11Device->md3dImmediateContext);
		mDX11Device->md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		world             = XMLoadFloat4x4(&mBoxWorld);
		worldInvTranspose = DX11Math::InverseTranspose(world);
		worldViewProj     = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&mBoxTexTransform));
		Effects::BasicFX->SetMaterial(mBoxMat);
		Effects::BasicFX->SetDiffuseMap(mBoxMap);
		Effects::BasicFX->SetMaskMap(mWhiteMaskMap);

		activeTech->GetPassByIndex(p)->Apply(0, mDX11Device->md3dImmediateContext);
		mDX11Device->md3dImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);

		// Draw the cylinders.
		for(int i = 0; i < 10; ++i)
		{
			world             = XMLoadFloat4x4(&mCylWorld[i]);
			worldInvTranspose = DX11Math::InverseTranspose(world);
			worldViewProj     = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&mCylinderTexTransform));
			Effects::BasicFX->SetMaterial(mCylinderMat);
			Effects::BasicFX->SetDiffuseMap(mCylinderMap);
			Effects::BasicFX->SetMaskMap(mWhiteMaskMap);

			activeTech->GetPassByIndex(p)->Apply(0, mDX11Device->md3dImmediateContext);
			mDX11Device->md3dImmediateContext->DrawIndexed(mCylinderIndexCount, mCylinderIndexOffset, mCylinderVertexOffset);
		}

		// Draw the spheres.
		for(int i = 0; i < 10; ++i)
		{
			world             = XMLoadFloat4x4(&mSphereWorld[i]);
			worldInvTranspose = DX11Math::InverseTranspose(world);
			worldViewProj     = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&mSphereTexTransform));
			Effects::BasicFX->SetMaterial(mSphereMat);
			Effects::BasicFX->SetDiffuseMap(mSphereMap);
			Effects::BasicFX->SetMaskMap(mWhiteMaskMap);

			activeTech->GetPassByIndex(p)->Apply(0, mDX11Device->md3dImmediateContext);
			mDX11Device->md3dImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
		}
	}

	activeTechModel->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the model.
		world             = XMLoadFloat4x4(&mModelWorld);
		worldInvTranspose = DX11Math::InverseTranspose(world);
		worldViewProj     = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mModelMat);

		activeTechModel->GetPassByIndex(p)->Apply(0, mDX11Device->md3dImmediateContext);
		mDX11Device->md3dImmediateContext->DrawIndexed(mModelIndexCount, mModelIndexOffset, mModelVertexOffset);
	}

	if (RJE_GLOBALS::gVsyncEnabled)
	{
		RJE_CHECK_FOR_SUCCESS(mSwapChain->Present(1, 0));
	}
	else
	{
		RJE_CHECK_FOR_SUCCESS(mSwapChain->Present(0, 0));
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11Wrapper::Shutdown()
{
	RJE_SAFE_RELEASE(mVertexBuffer);
	RJE_SAFE_RELEASE(mIndexBuffer);

	RJE_SAFE_RELEASE(mBoxMap);
	RJE_SAFE_RELEASE(mGridMap);
	RJE_SAFE_RELEASE(mSphereMap);
	RJE_SAFE_RELEASE(mCylinderMap);
	RJE_SAFE_RELEASE(mMaskMap);
	RJE_SAFE_RELEASE(mWhiteMaskMap);
	
	RJE_SAFE_RELEASE(mSamplerState_Anisotropic);
	RJE_SAFE_RELEASE(mSamplerState_Linear);

	RJE_SAFE_RELEASE(mRasterizerState_Solid);
	RJE_SAFE_RELEASE(mRasterizerState_Wireframe);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();

	RJE_SAFE_RELEASE(mRenderTargetView);
	mDX11DepthBuffer->Release();
	RJE_SAFE_RELEASE(mSwapChain);
	mDX11Device->Release();

	RJE_SAFE_DELETE(mDX11CommonStates);
	RJE_SAFE_DELETE(mDX11DepthBuffer);
	RJE_SAFE_DELETE(mDX11Device);
}

//////////////////////////////////////////////////////////////////////////
void DX11Wrapper::ResizeWindow(int newSizeWidth, int newSizeHeight)
{
	RJE_ASSERT(mDX11Device->md3dImmediateContext);
	RJE_ASSERT(mDX11Device->md3dDevice);
	RJE_ASSERT(mSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	RJE_SAFE_RELEASE(mRenderTargetView);
	RJE_SAFE_RELEASE(mDX11DepthBuffer->mDepthStencilView);
	RJE_SAFE_RELEASE(mDX11DepthBuffer->mDepthStencilBuffer);

	// Resize the swap chain and recreate the render target view.
	RJE_CHECK_FOR_SUCCESS(mSwapChain->ResizeBuffers(1, newSizeWidth, newSizeHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))
	ID3D11Texture2D* backBuffer;
	RJE_CHECK_FOR_SUCCESS(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	RJE_SAFE_RELEASE(backBuffer);

	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width     = newSizeWidth;
	depthStencilDesc.Height    = newSizeHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if( RJE_GLOBALS::gUse4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = (RJE_GLOBALS::g4xMsaaQuality)-1;
	}
	else // No MSAA
	{
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &(mDX11DepthBuffer->mDepthStencilBuffer)));
	RJE_CHECK_FOR_SUCCESS(mDX11Device->md3dDevice->CreateDepthStencilView(mDX11DepthBuffer->mDepthStencilBuffer, 0, &(mDX11DepthBuffer->mDepthStencilView)));


	// Bind the render target view and depth/stencil view to the pipeline.
	mDX11Device->md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDX11DepthBuffer->mDepthStencilView);

	// Set the viewport transform.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast<float>(newSizeWidth);
	mScreenViewport.Height   = static_cast<float>(newSizeHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mDX11Device->md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(0.25f*RJE::Math::Pi, static_cast<float>(newSizeWidth) / newSizeHeight, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}