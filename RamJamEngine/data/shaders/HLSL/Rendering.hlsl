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
struct Gbuffer
{
	float3 Position        : SV_TARGET0;
	float4 Albedo          : SV_TARGET1;
	float4 Normal_Specular : SV_TARGET2;
};

//-------------------
// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
	float3 positionView;         // View space position
	float3 positionViewDX;       // Screen space derivatives of view space position
	float3 positionViewDY;
	float3 normal;               // View space normal
	float4 albedo;
	float  specularAmount;       // Treated as a multiplier on albedo
	float  specularPower;
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

SurfaceData ComputeSurfaceDataFromGBufferSample(Texture2DMS<float4, MSAA_SAMPLES> gGbuffer[3], uint2 positionViewport, uint sampleIndex)
{
	// Load the raw data from the GBuffer
	Gbuffer rawData;
	rawData.Position        = gGbuffer[0].Load( positionViewport, sampleIndex).xyz;
	rawData.Albedo          = gGbuffer[1].Load( positionViewport, sampleIndex);
	rawData.Normal_Specular = gGbuffer[2].Load( positionViewport, sampleIndex);

	// Decode into reasonable outputs
	SurfaceData data;

	data.positionView   = rawData.Position;
	data.positionViewDX = ddx_coarse(data.positionView);
	data.positionViewDY = ddy_coarse(data.positionView);

	data.normal = DecodeSphereMap(rawData.Normal_Specular.xy);
	data.albedo = rawData.Albedo;

	data.specularAmount = rawData.Normal_Specular.z;
	data.specularPower  = rawData.Normal_Specular.w;

	return data;
}

void ComputeSurfaceDataFromGBufferAllSamples(Texture2DMS<float4, MSAA_SAMPLES> gGbuffer[3], uint2 positionViewport, out SurfaceData surface[MSAA_SAMPLES])
{
	// Load data for each sample
	// Most of this time only a small amount of this data is actually used so unrolling
	// this loop ensures that the compiler has an easy time with the dead code elimination.
	[unroll] for (uint i = 0; i < MSAA_SAMPLES; ++i)
	{
		surface[i] = ComputeSurfaceDataFromGBufferSample(gGbuffer, positionViewport, i);
	}
}

// Check if a given pixel can be shaded at pixel frequency (i.e. they all come from
// the same surface) or if they require per-sample shading
bool RequiresPerSampleShading(SurfaceData surface[MSAA_SAMPLES])
{
	bool perSample = false;

	const float maxZDelta = abs(surface[0].positionViewDX.z) + abs(surface[0].positionViewDY.z);
	const float minNormalDot = 0.99f;        // Allow ~8 degree normal deviations

	[unroll] for (uint i = 1; i < MSAA_SAMPLES; ++i)
	{
		// Using the position derivatives of the triangle, check if all of the sample depths
		// could possibly have come from the same triangle/surface
		perSample = perSample || abs(surface[i].positionView.z - surface[0].positionView.z) > maxZDelta;

		// Also flag places where the normal is different
		perSample = perSample || dot(surface[i].normal, surface[0].normal) < minNormalDot;
	}

	return perSample;
}

// Average All the sample so we can compute only one BRDF per pixel instead of per sample
// We average only the data used by the lighting computation for performance
SurfaceData AverageMSAASamples(SurfaceData surface[MSAA_SAMPLES])
{
	SurfaceData surfaceOut;
	surfaceOut.positionView   = surface[0].positionView;
	surfaceOut.positionViewDX = surface[0].positionViewDX;
	surfaceOut.positionViewDY = surface[0].positionViewDY;
	surfaceOut.normal         = surface[0].normal;
	surfaceOut.albedo         = surface[0].albedo;
	surfaceOut.specularAmount = surface[0].specularAmount;
	surfaceOut.specularPower  = surface[0].specularPower;
	[unroll] for (uint i = 1; i < MSAA_SAMPLES; ++i)
	{
		surfaceOut.positionView   += surface[i].positionView;
		surfaceOut.normal         += surface[i].normal;
		surfaceOut.albedo         += surface[i].albedo;
		surfaceOut.specularAmount += surface[i].specularAmount;
		surfaceOut.specularPower  += surface[i].specularPower;
	}
	float rcpMSAA = rcp(MSAA_SAMPLES);
	surfaceOut.positionView   *= rcpMSAA;
	surfaceOut.normal         *= rcpMSAA;
	surfaceOut.albedo         *= rcpMSAA;
	surfaceOut.specularAmount *= rcpMSAA;
	surfaceOut.specularPower  *= rcpMSAA;

	return surfaceOut;
}
#endif
