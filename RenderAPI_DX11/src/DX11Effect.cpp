#include "DX11Effect.h"
#include "../../RamJamEngine/include/System.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
{
	mFX = nullptr;

	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	RJE_CHECK_FOR_SUCCESS(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, device, &mFX));
}

Effect::~Effect()
{
	RJE_SAFE_RELEASE(mFX);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	BasicTech         = mFX->GetTechniqueByName("Basic");
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = mFX->GetVariableByName("gFogColor")->AsVector();
	FogEnabled        = mFX->GetVariableByName("gUseFog")->AsScalar();
	AlphaClipEnabled  = mFX->GetVariableByName("gUseAlphaClip")->AsScalar();
	TextureEnabled    = mFX->GetVariableByName("gUseTexture")->AsScalar();
	FogStart          = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLightCount     = mFX->GetVariableByName("gDirLightCount")->AsScalar();
	PointLightCount   = mFX->GetVariableByName("gPointLightCount")->AsScalar();
	DirLights         = mFX->GetVariableByName("gDirLights");
	PointLights       = mFX->GetVariableByName("gPointLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	MaskMap           = mFX->GetVariableByName("gMaskMap")->AsShaderResource();
	TextureSampler    = (ID3DX11EffectSamplerVariable*) mFX->GetVariableByName("gTextureSampler");
}

BasicEffect::~BasicEffect(){}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region SpriteEffect
SpriteEffect::SpriteEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	SpriteTech = mFX->GetTechniqueByName("SpriteTech");
	SpriteMap  = mFX->GetVariableByName("gSpriteTex")->AsShaderResource();
}

SpriteEffect::~SpriteEffect(){}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region ColorEffect
ColorEffect::ColorEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	ColorTech		= mFX->GetTechniqueByName("ColorTech");
	WorldViewProj	= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

ColorEffect::~ColorEffect(){}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Effects

BasicEffect*  DX11Effects::BasicFX  = nullptr;
ColorEffect*  DX11Effects::ColorFX  = nullptr;
SpriteEffect* DX11Effects::SpriteFX = nullptr;

void DX11Effects::InitAll(ID3D11Device* device)
{
	wstring shaderPath = StringToWString(System::Instance()->mDataPath);

	shaderPath += CIniFile::GetValueW("basic",  "shaders", System::Instance()->mResourcesPath);
	BasicFX  = rje_new BasicEffect( device, shaderPath);
	//-------------
	shaderPath = StringToWString(System::Instance()->mDataPath) + CIniFile::GetValueW("sprite", "shaders", System::Instance()->mResourcesPath);
	SpriteFX = rje_new SpriteEffect(device, shaderPath);
	//-------------
	shaderPath = StringToWString(System::Instance()->mDataPath) + CIniFile::GetValueW("color", "shaders", System::Instance()->mResourcesPath);
	ColorFX = rje_new ColorEffect(device, shaderPath);
}

void DX11Effects::DestroyAll()
{
	RJE_SAFE_DELETE(BasicFX);
	RJE_SAFE_DELETE(SpriteFX);
	RJE_SAFE_DELETE(ColorFX);
}
#pragma endregion