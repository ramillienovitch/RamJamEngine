#include "DX11Effect.h"

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
	Light1_0Tech      = mFX->GetTechniqueByName("Light1_0");
	Light2_0Tech      = mFX->GetTechniqueByName("Light2_0");
	Light3_0Tech      = mFX->GetTechniqueByName("Light3_0");
	Light1_1Tech      = mFX->GetTechniqueByName("Light1_1");
	Light2_1Tech      = mFX->GetTechniqueByName("Light2_1");
	Light3_1Tech      = mFX->GetTechniqueByName("Light3_1");
	Light1_2Tech      = mFX->GetTechniqueByName("Light1_2");
	Light2_2Tech      = mFX->GetTechniqueByName("Light2_2");
	Light3_2Tech      = mFX->GetTechniqueByName("Light3_2");
	Light1_3Tech      = mFX->GetTechniqueByName("Light1_3");
	Light2_3Tech      = mFX->GetTechniqueByName("Light2_3");
	Light3_3Tech      = mFX->GetTechniqueByName("Light3_3");
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights         = mFX->GetVariableByName("gDirLights");
	PointLights       = mFX->GetVariableByName("gPointLights");
	Mat               = mFX->GetVariableByName("gMaterial");
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Effects

BasicEffect* Effects::BasicFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	BasicFX = new BasicEffect(device, L"..\\..\\RamJamEngine\\data\\shaders\\HLSL\\basic.cso");
}

void Effects::DestroyAll()
{
	RJE_SAFE_DELETE(BasicFX);
}
#pragma endregion