#pragma once

#include "DX11Helper.h"

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region BasicEffect
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	HRESULT SetWorldViewProj(CXMMATRIX M)                  { return WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetWorld(CXMMATRIX M)                          { return World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetWorldInvTranspose(CXMMATRIX M)              { return WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetTexTransform(CXMMATRIX M)                   { return TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetEyePosW(const XMFLOAT3& v)                  { return EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	HRESULT SetDirLights(const DirectionalLight* lights)   { return DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	HRESULT SetPointLights(const PointLight* lights)       { return PointLights->SetRawValue(lights, 0, 3*sizeof(PointLight)); }
	HRESULT SetMaterial(const Material& mat)               { return Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	HRESULT SetDiffuseMap(ID3D11ShaderResourceView* tex)   { return DiffuseMap->SetResource(tex); }
	HRESULT SetMaskMap(ID3D11ShaderResourceView* tex)      { return MaskMap->SetResource(tex); }
	HRESULT SetSamplerState(ID3D11SamplerState* pSampler)  { return TextureSampler->SetSampler(0, pSampler); }

	ID3DX11EffectTechnique* Light0_0Tech;
	ID3DX11EffectTechnique* Light1_0Tech;
	ID3DX11EffectTechnique* Light2_0Tech;
	ID3DX11EffectTechnique* Light3_0Tech;
	ID3DX11EffectTechnique* Light0_1Tech;
	ID3DX11EffectTechnique* Light1_1Tech;
	ID3DX11EffectTechnique* Light2_1Tech;
	ID3DX11EffectTechnique* Light3_1Tech;
	ID3DX11EffectTechnique* Light0_2Tech;
	ID3DX11EffectTechnique* Light1_2Tech;
	ID3DX11EffectTechnique* Light2_2Tech;
	ID3DX11EffectTechnique* Light3_2Tech;
	ID3DX11EffectTechnique* Light0_3Tech;
	ID3DX11EffectTechnique* Light1_3Tech;
	ID3DX11EffectTechnique* Light2_3Tech;
	ID3DX11EffectTechnique* Light3_3Tech;
	ID3DX11EffectTechnique* Light1_3NoTexTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectSamplerVariable* TextureSampler;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* MaskMap;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};
#pragma endregion