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
// GBuffer and related common utilities and structures
struct GBuffer
{
	float4 normal_specular	: SV_Target0;
	float4 albedo			: SV_Target1;
	float2 positionZGrad	: SV_Target2;
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
Texture2DMS<float4, MSAA_SAMPLES>	gGBufferTextures[4] : register(t0);		// normal/spec - albedo - positionZ - depth
Texture2D							gDiffuseMap		: Texture_Diffuse;
SamplerState						gTextureSampler;

//--------------------------------------------------------------------------------------
// Utility
//--------------------------------------------------------------------------------------

float linstep(float min, float max, float v)
{
	return saturate((v - min) / (max - min));
}

// - RGBA 16-bit per component packed into a uint2 per texel
float4 UnpackRGBA16(uint2 e)
{
	return float4(f16tof32(e), f16tof32(e >> 16));
}
uint2 PackRGBA16(float4 c)
{
	return f32tof16(c.rg) | (f32tof16(c.ba) << 16);
}

// Linearize the given 2D address + sample index into our flat framebuffer array
uint GetFramebufferSampleAddress(uint2 coords, uint sampleIndex)
{
	// Major ordering: Row (x), Col (y), MSAA sample
	return (sampleIndex * mFramebufferDimensions.y + coords.y) * mFramebufferDimensions.x + coords.x;
}

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

SurfaceData ComputeSurfaceDataFromGeometry(VertexOut input)
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

	surface.albedo = gDiffuseMap.Sample(gTextureSampler, input.Tex);
	surface.albedo.rgb = gUseLightingOnly ? float3(1.0f, 1.0f, 1.0f) : surface.albedo.rgb;

	// Map NULL diffuse textures to white
	uint2 textureDim;
	gDiffuseMap.GetDimensions(textureDim.x, textureDim.y);
	surface.albedo = (textureDim.x == 0U ? float4(1.0f, 1.0f, 1.0f, 1.0f) : surface.albedo);

	// We don't really have art asset-related values for these, so set them to something
	// reasonable for now... the important thing is that they are stored in the G-buffer for
	// representative performance measurement.
	surface.specularAmount = gMatSpecularAmount;
	surface.specularPower  = gMatSpecularPower;

	return surface;
}

float3 ComputePositionViewFromZ(float2 positionScreen, float viewSpaceZ)
{
	float2 screenSpaceRay = float2(positionScreen.x / gProj._11, positionScreen.y / gProj._22);

	float3 positionView;
	positionView.z = viewSpaceZ;
	// Solve the two projection equations
	positionView.xy = screenSpaceRay.xy * positionView.z;

	return positionView;
}

SurfaceData ComputeSurfaceDataFromGBufferSample(uint2 positionViewport, uint sampleIndex)
{
	// Load the raw data from the GBuffer
	GBuffer rawData;
	rawData.normal_specular = gGBufferTextures[0].Load(positionViewport.xy, sampleIndex).xyzw;
	rawData.albedo = gGBufferTextures[1].Load(positionViewport.xy, sampleIndex).xyzw;
	rawData.positionZGrad = gGBufferTextures[2].Load(positionViewport.xy, sampleIndex).xy;
	float zBuffer = gGBufferTextures[3].Load(positionViewport.xy, sampleIndex).x;

	float2 gbufferDim;
	uint dummy;
	gGBufferTextures[0].GetDimensions(gbufferDim.x, gbufferDim.y, dummy);

	// Compute screen/clip-space position and neighbour positions
	// NOTE: Mind DX11 viewport transform and pixel center!
	// NOTE: This offset can actually be precomputed on the CPU but it's actually slower to read it from
	// a constant buffer than to just recompute it.
	float2 screenPixelOffset = float2(2.0f, -2.0f) / gbufferDim;
	float2 positionScreen    = (float2(positionViewport.xy) + 0.5f) * screenPixelOffset.xy + float2(-1.0f, 1.0f);
	float2 positionScreenX   = positionScreen + float2(screenPixelOffset.x, 0.0f);
	float2 positionScreenY   = positionScreen + float2(0.0f, screenPixelOffset.y);

	// Decode into reasonable outputs
	SurfaceData data;

	// Unproject depth buffer Z value into view space
	float viewSpaceZ = gProj._43 / (zBuffer - gProj._33);

	data.positionView = ComputePositionViewFromZ(positionScreen, viewSpaceZ);
	data.positionViewDX = ComputePositionViewFromZ(positionScreenX, viewSpaceZ + rawData.positionZGrad.x) - data.positionView;
	data.positionViewDY = ComputePositionViewFromZ(positionScreenY, viewSpaceZ + rawData.positionZGrad.y) - data.positionView;

	data.normal = DecodeSphereMap(rawData.normal_specular.xy);
	data.albedo = rawData.albedo;

	data.specularAmount = rawData.normal_specular.z;
	data.specularPower  = rawData.normal_specular.w;

	return data;
}

void ComputeSurfaceDataFromGBufferAllSamples(uint2 positionViewport, out SurfaceData surface[MSAA_SAMPLES])
{
	// Load data for each sample
	// Most of this time only a small amount of this data is actually used so unrolling
	// this loop ensures that the compiler has an easy time with the dead code elimination.
	[unroll] for (uint i = 0; i < MSAA_SAMPLES; ++i)
	{
		surface[i] = ComputeSurfaceDataFromGBufferSample(positionViewport, i);
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

#endif