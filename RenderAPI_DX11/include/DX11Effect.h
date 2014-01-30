#pragma once

#include "DX11Helper.h"

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

//////////////////////////////////////////////////////////////////////////

struct BasicEffect : public Effect
{
	BasicEffect(ID3D11Device* device, const std::string& filename);
	~BasicEffect();

	HRESULT SetWorld(Matrix44& M)                            { return World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetViewProj(Matrix44& M)                         { return ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetProj(Matrix44& M)                             { return Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	HRESULT SetEyePosW(const Vector3& v)                     { return EyePosW->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT UseFaceNormals(BOOL state)                       { return FaceNormals->SetBool(state != 0); }
	HRESULT OnlyAlbedo(BOOL state)                           { return ViewAlbedo->SetBool(state != 0); }
	HRESULT OnlyNormals(BOOL state)                          { return ViewNormals->SetBool(state != 0); }
	HRESULT OnlyDepth(BOOL state)                            { return ViewDepth->SetBool(state != 0); }
	HRESULT OnlySpecular(BOOL state)                         { return ViewSpecular->SetBool(state != 0); }
	HRESULT SetFogState(BOOL state)                          { return FogEnabled->SetBool(state != 0); }
	HRESULT SetAlphaClipState(BOOL state)                    { return AlphaClipEnabled->SetBool(state != 0); }
	HRESULT SetTextureState(BOOL state)                      { return TextureEnabled->SetBool(state != 0); }
	HRESULT SetFogColor(const Vector4& v)                    { return FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT SetFogStart(float f)                             { return FogStart->SetFloat(f); }
	HRESULT SetFogRange(float f)                             { return FogRange->SetFloat(f); }
	HRESULT SetAmbientLight(const Vector4& v)                { return AmbientLight->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	HRESULT SetDirLights(ID3D11ShaderResourceView* lights)   { return DirLights->SetResource(lights); }
	HRESULT SetPointLights(ID3D11ShaderResourceView* lights) { return PointLights->SetResource(lights); }
	HRESULT SetSpotLights(ID3D11ShaderResourceView* lights)  { return SpotLights->SetResource(lights); }
	HRESULT SetSamplerState(ID3D11SamplerState* pSampler)    { return TextureSampler->SetSampler(0, pSampler); }
	HRESULT SetMaterial(Material* mat);
	
	//-------------------------------------------

	ID3DX11EffectTechnique*					BasicTech;
	//-------
	ID3DX11EffectMatrixVariable*			World;
	ID3DX11EffectMatrixVariable*			ViewProj;
	ID3DX11EffectMatrixVariable*			Proj;
	ID3DX11EffectMatrixVariable*			TexTransform;
	//-------
	ID3DX11EffectVectorVariable*			EyePosW;
	ID3DX11EffectVectorVariable*			FogColor;
	//-------
	ID3DX11EffectScalarVariable*			FaceNormals;
	ID3DX11EffectScalarVariable*			ViewAlbedo;
	ID3DX11EffectScalarVariable*			ViewNormals;
	ID3DX11EffectScalarVariable*			ViewDepth;
	ID3DX11EffectScalarVariable*			ViewSpecular;
	//-------
	ID3DX11EffectScalarVariable*			FogEnabled;
	ID3DX11EffectScalarVariable*			AlphaClipEnabled;
	ID3DX11EffectScalarVariable*			TextureEnabled;
	ID3DX11EffectScalarVariable*			FogStart;
	ID3DX11EffectScalarVariable*			FogRange;
	//-------
	ID3DX11EffectSamplerVariable*			TextureSampler;
	//-------
	ID3DX11EffectVectorVariable*			AmbientLight;
	ID3DX11EffectShaderResourceVariable*	DirLights;
	ID3DX11EffectShaderResourceVariable*	PointLights;
	ID3DX11EffectShaderResourceVariable*	SpotLights;
};

//////////////////////////////////////////////////////////////////////////
struct PostProcessEffect : public Effect
{
	PostProcessEffect(ID3D11Device* device, const std::string& filename);
	~PostProcessEffect();

	HRESULT SetTextureMap(ID3D11ShaderResourceView* tex)        { return TextureMap->SetResource(tex); }
	
	ID3DX11EffectTechnique*					PostProcessTech;
	ID3DX11EffectShaderResourceVariable*	TextureMap;
};

//////////////////////////////////////////////////////////////////////////

struct SpriteEffect : public Effect
{
	SpriteEffect(ID3D11Device* device, const std::string& filename);
	~SpriteEffect();

	void SetSpriteMap(ID3D11ShaderResourceView* tex) { SpriteMap->SetResource(tex); }

	ID3DX11EffectTechnique* SpriteTech;
	ID3DX11EffectShaderResourceVariable* SpriteMap;
};

//////////////////////////////////////////////////////////////////////////

struct ColorEffect : public Effect
{
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


//////////////////////////////////////////////////////////////////////////

struct DX11Effects
{
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect*           BasicFX;
	static ColorEffect*           ColorFX;
	static SpriteEffect*          SpriteFX;
	static PostProcessEffect*     PostProcessFX;
};
