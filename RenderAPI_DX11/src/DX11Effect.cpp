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
	Light0_0NoTexTech             = mFX->GetTechniqueByName("Light0_0NoTex");
	Light0_1NoTexTech             = mFX->GetTechniqueByName("Light0_1NoTex");
	Light0_2NoTexTech             = mFX->GetTechniqueByName("Light0_2NoTex");
	Light0_3NoTexTech             = mFX->GetTechniqueByName("Light0_3NoTex");
	Light1_0NoTexTech             = mFX->GetTechniqueByName("Light1_0NoTex");
	Light1_1NoTexTech             = mFX->GetTechniqueByName("Light1_1NoTex");
	Light1_2NoTexTech             = mFX->GetTechniqueByName("Light1_2NoTex");
	Light1_3NoTexTech             = mFX->GetTechniqueByName("Light1_3NoTex");
	Light0_0TexTech               = mFX->GetTechniqueByName("Light0_0Tex");
	Light0_1TexTech               = mFX->GetTechniqueByName("Light0_1Tex");
	Light0_2TexTech               = mFX->GetTechniqueByName("Light0_2Tex");
	Light0_3TexTech               = mFX->GetTechniqueByName("Light0_3Tex");
	Light1_0TexTech               = mFX->GetTechniqueByName("Light1_0Tex");
	Light1_1TexTech               = mFX->GetTechniqueByName("Light1_1Tex");
	Light1_2TexTech               = mFX->GetTechniqueByName("Light1_2Tex");
	Light1_3TexTech               = mFX->GetTechniqueByName("Light1_3Tex");
	Light1_3FogTexTech            = mFX->GetTechniqueByName("Light1_3FogTex");
	Light1_3FogNoTexTech          = mFX->GetTechniqueByName("Light1_3FogNoTex");
	Light1_3AlphaClipTexTech      = mFX->GetTechniqueByName("Light1_3AlphaClipTex");
	Light1_3AlphaClipNoTexTech    = mFX->GetTechniqueByName("Light1_3AlphaClipNoTex");
	Light1_3FogAlphaClipTexTech   = mFX->GetTechniqueByName("Light1_3FogAlphaClipTex");
	Light1_3FogAlphaClipNoTexTech = mFX->GetTechniqueByName("Light1_3FogAlphaClipNoTex");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart          = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = mFX->GetVariableByName("gFogRange")->AsScalar();
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
	BasicFX  = new BasicEffect( device, shaderPath);
	//-------------
	shaderPath = StringToWString(System::Instance()->mDataPath) + CIniFile::GetValueW("sprite", "shaders", System::Instance()->mResourcesPath);
	SpriteFX = new SpriteEffect(device, shaderPath);
	//-------------
	shaderPath = StringToWString(System::Instance()->mDataPath) + CIniFile::GetValueW("color", "shaders", System::Instance()->mResourcesPath);
	ColorFX = new ColorEffect(device, shaderPath);
}

void DX11Effects::DestroyAll()
{
	RJE_SAFE_DELETE(BasicFX);
	RJE_SAFE_DELETE(SpriteFX);
	RJE_SAFE_DELETE(ColorFX);
}
#pragma endregion