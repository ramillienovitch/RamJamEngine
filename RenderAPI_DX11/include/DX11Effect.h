#pragma once

#include "DX11Helper.h"

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::string& filename);
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
	BasicEffect(ID3D11Device* device, const std::string& filename);
	~BasicEffect();

	HRESULT SetWorld(Matrix44& M)                            { return World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetViewProj(Matrix44& M)                         { return ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	//HRESULT SetWorldInvTranspose(Matrix44& M)                { return WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetEyePosW(const Vector3& v)                     { return EyePosW->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT SetFogState(BOOL state)                          { return FogEnabled->SetBool(state != 0); }
	HRESULT SetAlphaClipState(BOOL state)                    { return AlphaClipEnabled->SetBool(state != 0); }
	HRESULT SetTextureState(BOOL state)                      { return TextureEnabled->SetBool(state != 0); }
	HRESULT SetFogColor(const Vector4& v)                    { return FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT SetFogStart(float f)                             { return FogStart->SetFloat(f); }
	HRESULT SetFogRange(float f)                             { return FogRange->SetFloat(f); }
	HRESULT SetDirLights(ID3D11ShaderResourceView* lights)   { return DirLights->SetResource(lights); }
	HRESULT SetPointLights(ID3D11ShaderResourceView* lights) { return PointLights->SetResource(lights); }
	HRESULT SetSpotLights(ID3D11ShaderResourceView* lights)  { return SpotLights->SetResource(lights); }
	HRESULT SetSamplerState(ID3D11SamplerState* pSampler)    { return TextureSampler->SetSampler(0, pSampler); }
	HRESULT SetMaterial(Material& mat);
	
	//-------------------------------------------

	ID3DX11EffectTechnique*					BasicTech;
	//-------
	ID3DX11EffectMatrixVariable*			World;
	ID3DX11EffectMatrixVariable*			ViewProj;
	//ID3DX11EffectMatrixVariable*			WorldInvTranspose;
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
	//-------
	ID3DX11EffectSamplerVariable*			TextureSampler;
	//-------
	ID3DX11EffectShaderResourceVariable*	DirLights;
	ID3DX11EffectShaderResourceVariable*	PointLights;
	ID3DX11EffectShaderResourceVariable*	SpotLights;
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region SpriteEffect
class SpriteEffect : public Effect
{
public:
	SpriteEffect(ID3D11Device* device, const std::string& filename);
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
	ColorEffect(ID3D11Device* device, const std::string& filename);
	~ColorEffect();

	HRESULT SetColor(const Vector4& v)		{ return Color->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT SetWorld(Matrix44& M)			{ return World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetViewProj(Matrix44& M)		{ return ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique*			ColorTech;
	ID3DX11EffectVectorVariable*	Color;
	ID3DX11EffectMatrixVariable*	World;
	ID3DX11EffectMatrixVariable*	ViewProj;
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