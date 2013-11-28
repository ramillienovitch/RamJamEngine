#pragma once

#include "DX11Helper.h"

#include "../../RamJamEngine/include/Light.h"

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

	HRESULT SetWorldViewProj(Matrix44& M)                 { return WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetWorld(Matrix44& M)                         { return World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetWorldInvTranspose(Matrix44& M)             { return WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetTexTransform(Matrix44& M)                  { return TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetEyePosW(const Vector3& v)                  { return EyePosW->SetRawValue(&v, 0, sizeof(Vector3)); }
	HRESULT SetFogState(BOOL state)                       { return FogEnabled->SetBool(state != 0); }
	HRESULT SetAlphaClipState(BOOL state)                 { return AlphaClipEnabled->SetBool(state != 0); }
	HRESULT SetTextureState(BOOL state)                   { return TextureEnabled->SetBool(state != 0); }
	HRESULT SetFogColor(const Vector4& v)                 { return FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT SetFogStart(float f)                          { return FogStart->SetFloat(f); }
	HRESULT SetFogRange(float f)                          { return FogRange->SetFloat(f); }
	HRESULT SetDirLightCount(int val)                     { return DirLightCount->SetInt(val); }
	HRESULT SetPointLightCount(int val)                   { return PointLightCount->SetInt(val); }
	HRESULT SetDirLights(const DirectionalLight* lights)  { return DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	HRESULT SetPointLights(const PointLight* lights)      { return PointLights->SetRawValue(lights, 0, 3*sizeof(PointLight)); }
	HRESULT SetMaterial(const Material& mat)              { return Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	HRESULT SetDiffuseMap(ID3D11ShaderResourceView* tex)  { return DiffuseMap->SetResource(tex); }
	HRESULT SetMaskMap(ID3D11ShaderResourceView* tex)     { return MaskMap->SetResource(tex); }
	HRESULT SetSamplerState(ID3D11SamplerState* pSampler) { return TextureSampler->SetSampler(0, pSampler); }

	//-------------------------------------------

	ID3DX11EffectTechnique*					BasicTech;
	//-------
	ID3DX11EffectMatrixVariable*			WorldViewProj;
	ID3DX11EffectMatrixVariable*			World;
	ID3DX11EffectMatrixVariable*			WorldInvTranspose;
	ID3DX11EffectMatrixVariable*			TexTransform;
	//-------
	ID3DX11EffectVectorVariable*			EyePosW;
	ID3DX11EffectVectorVariable*			FogColor;
	//-------
	ID3DX11EffectScalarVariable*			FogEnabled;
	ID3DX11EffectScalarVariable*			AlphaClipEnabled;
	ID3DX11EffectScalarVariable*			TextureEnabled;
	ID3DX11EffectScalarVariable*			FogStart;
	ID3DX11EffectScalarVariable*			FogRange;
	ID3DX11EffectScalarVariable*			DirLightCount;
	ID3DX11EffectScalarVariable*			PointLightCount;
	//-------
	ID3DX11EffectVariable*					DirLights;
	ID3DX11EffectVariable*					PointLights;
	ID3DX11EffectVariable*					Mat;
	//-------
	ID3DX11EffectSamplerVariable*			TextureSampler;
	//-------
	ID3DX11EffectShaderResourceVariable*	DiffuseMap;
	ID3DX11EffectShaderResourceVariable*	MaskMap;
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region SpriteEffect
class SpriteEffect : public Effect
{
public:
	SpriteEffect(ID3D11Device* device, const std::wstring& filename);
	~SpriteEffect();

	void SetSpriteMap(ID3D11ShaderResourceView* tex) { SpriteMap->SetResource(tex); }

	ID3DX11EffectTechnique* SpriteTech;
	ID3DX11EffectShaderResourceVariable* SpriteMap;
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region ColorEffect
class ColorEffect : public Effect
{
public:
	ColorEffect(ID3D11Device* device, const std::wstring& filename);
	~ColorEffect();

	HRESULT SetWorldViewProj(Matrix44& M)	{ return WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique*			ColorTech;
	ID3DX11EffectMatrixVariable*	WorldViewProj;
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Effects
class DX11Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect*  BasicFX;
	static ColorEffect*  ColorFX;
	static SpriteEffect* SpriteFX;
};
#pragma endregion