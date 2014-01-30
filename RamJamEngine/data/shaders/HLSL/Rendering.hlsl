#ifndef RENDERING_HLSL
#define RENDERING_HLSL

#include "../../../include/ShaderDefines.h"
#include "PerFrameConstants.hlsl"

//-------------------
struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float3 TanL    : TANGENT;
	float2 Tex     : TEXCOORD;
};

//-------------------
struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float3 TanW    : TANGENT;
	float2 Tex     : TEXCOORD;
};

//-------------------
// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
	float3 positionView;         // View space position
	float3 positionViewDX;       // Screen space derivatives
	float3 positionViewDY;       // of view space position
	float3 normal;               // View space normal
	float4 albedo;
	float specularAmount;        // Treated as a multiplier on albedo
	float specularPower;
};

//--------------------------------------------------------------------------------------
cbuffer cbPerObject
{
	float4x4 gWorld;
	//float4x4 gWorldInvTranspose;
	float4x4 gDiffuseMapTrf	: Texture_Diffuse_Trf;
	//-------
	bool   gMatUseAlpha			: Transparency;
	float4 gMatAlbedo			: Albedo;
	float  gMatSpecularAmount	: SpecularAmount;
	float  gMatSpecularPower	: SpecularPower;
};

//--------------------------------------------------------------------------------------
// Utility
//--------------------------------------------------------------------------------------

float3 ComputeFaceNormal(float3 position)
{
	return cross(ddx_coarse(position), ddy_coarse(position));
}

float2 EncodeSphereMap(float3 n)
{
	float oneMinusZ = 1.0f - n.z;
	float p = sqrt(n.x * n.x + n.y * n.y + oneMinusZ * oneMinusZ);
	return n.xy / p * 0.5f + 0.5f;
}

float3 DecodeSphereMap(float2 e)
{
	float2 tmp = e - e * e;
	float f = tmp.x + tmp.y;
	float m = sqrt(4.0f * f - 1.0f);

	float3 n;
	n.xy = m * (e * 4.0f - 2.0f);
	n.z  = 3.0f - 8.0f * f;
	return n;
}

//--------------------------------------------------------------------------------------
// Deferred
//--------------------------------------------------------------------------------------

SurfaceData ComputeSurfaceDataFromGeometry(VertexOut input, Texture2D gTexture, SamplerState sam)
{
	SurfaceData surface;
	surface.positionView = input.PosW;

	// These arguably aren't really useful in this path since they are really only used to
	// derive shading frequencies and composite derivatives but might as well compute them
	// in case they get used for anything in the future.
	// (Like the rest of these values, they will get removed by dead code elimination if
	// they are unused.)
	surface.positionViewDX = ddx_coarse(surface.positionView);
	surface.positionViewDY = ddy_coarse(surface.positionView);

	// Optionally use face normal instead of shading normal
	float3 faceNormal = ComputeFaceNormal(input.PosW);
	surface.normal = normalize(gUseFaceNormals? faceNormal : input.NormalW);

	surface.albedo = gTexture.Sample(sam, input.Tex);
	//surface.albedo.rgb = gUseLightingOnly ? float3(1.0f, 1.0f, 1.0f) : surface.albedo.rgb;
	surface.albedo.rgb = surface.albedo.rgb;

	// Map NULL diffuse textures to white
	uint2 textureDim;
	gTexture.GetDimensions(textureDim.x, textureDim.y);
	surface.albedo = (textureDim.x == 0U ? float4(1.0f, 1.0f, 1.0f, 1.0f) : surface.albedo);

	surface.specularAmount = gMatSpecularAmount;
	surface.specularPower  = gMatSpecularPower;

	return surface;
}
#endif
