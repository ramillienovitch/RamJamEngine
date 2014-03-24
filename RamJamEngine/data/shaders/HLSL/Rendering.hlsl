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
	float4 Albedo       : SV_TARGET0;
	float3 Position     : SV_TARGET1;
	float4 Normal_Depth : SV_TARGET2;
	float2 Specular     : SV_TARGET3;
};

//-------------------
// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
	float3 position;         // World space position
	float3 positionDX;       // world space derivatives of world space position
	float3 positionDY;
	float3 normal;               // View space normal
	float4 albedo;
	float  specularAmount;       // Treated as a multiplier on albedo
	float  specularPower;
	float  zDepth;
};

//--------------------------------------------------------------------------------------
struct Partition
{
	float intervalBegin;
	float intervalEnd;

	// These are given in texture coordinate [0, 1] space
	float3 scale;
	float3 bias;
};

// This version of the structure is useful for atomic operations (asuint(), etc)
// Must match layout of structure above!!
struct PartitionUint
{
	uint intervalBegin;
	uint intervalEnd;

	// These are given in texture coordinate [0, 1] space
	uint3 scale;
	uint3 bias;
};

//-------------------
// Uint version of the bounds structure for atomic usage
// NOTE: This version cannot represent negative numbers!
struct BoundsUint
{
	uint3 minCoord;
	uint3 maxCoord;
};

// Float version of structure for convenient
// NOTE: We still tend to maintain the non-negative semantics of the above for consistency
struct BoundsFloat
{
	float3 minCoord;
	float3 maxCoord;
};

//--------------------------------------------------------------------------------------
cbuffer cbPerObject
{
	float4x4 gWorld;
	//float4x4 gWorldInvTranspose;
	float4x4 gDiffuseMapTrf	: Texture_Diffuse_Trf;
	float4x4 gNormalMapTrf	: Normal_Map_Trf;
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

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f;

		// Build orthonormal basis.
		float3 N = unitNormalW;
		float3 T = normalize(tangentW - dot(tangentW, N)*N);
		float3 B = cross(N, T);

		float3x3 TBN = float3x3(T, B, N);

		// Transform from tangent space to world space.
		float3 bumpedNormalW = mul(normalT, TBN);

		return bumpedNormalW;
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
	return (sampleIndex * gFramebufferSizeY + coords.y) * gFramebufferSizeX + coords.x;
}

//--------------------------------------------------------------------------------------
// Deferred
//--------------------------------------------------------------------------------------

SurfaceData ComputeSurfaceDataFromGeometry(VertexOut input, Texture2D gTexture, Texture2D gNormalMap, SamplerState sam)
{
	SurfaceData surface;
	//surface.position = mul(float4(input.PosW, 1.0f), gView).xyz;
	surface.position = input.PosW;

	// These arguably aren't really useful in this path since they are really only used to
	// derive shading frequencies and composite derivatives but might as well compute them
	// in case they get used for anything in the future.
	// (Like the rest of these values, they will get removed by dead code elimination if
	// they are unused.)
	surface.positionDX = ddx_coarse(surface.position);
	surface.positionDY = ddy_coarse(surface.position);

	// Optionally use face normal instead of shading normal
	float3 normal;
	if (gUseFaceNormals)
		normal = normalize(ComputeFaceNormal(input.PosW));
	else if (gUseNormalMaps)
	{
		uint2 textureDim;
		gNormalMap.GetDimensions(textureDim.x, textureDim.y);
		if (textureDim.x == 0U)
			normal = normalize(input.NormalW);
		else
		{
			float3 sample = (textureDim.x == 0U ? float3(1.0f, 1.0f, 1.0f) : gNormalMap.Sample(sam, input.Tex).xyz);
			normal = NormalSampleToWorldSpace(sample, normalize(input.NormalW), input.TanW);
		}
	}
	else
		normal = normalize(input.NormalW);

	surface.normal = normal;

	// Map NULL diffuse textures to white
	uint2 textureDim;
	gTexture.GetDimensions(textureDim.x, textureDim.y);
	surface.albedo = (textureDim.x == 0U || !gUseTexture ? float4(1.0f, 1.0f, 1.0f, 1.0f) : gTexture.Sample(sam, input.Tex) * gMatAlbedo);

	surface.specularAmount = gMatSpecularAmount;
	surface.specularPower  = gMatSpecularPower;

	return surface;
}

SurfaceData ComputeSurfaceDataFromGBufferSample(Texture2DMS<float4, MSAA_SAMPLES> gGbuffer[4], uint2 positionViewport, uint sampleIndex)
{
	// Load the raw data from the GBuffer
	Gbuffer rawData;
	rawData.Albedo       = gGbuffer[0].Load( positionViewport, sampleIndex);
	rawData.Position     = gGbuffer[1].Load( positionViewport, sampleIndex).xyz;
	rawData.Normal_Depth = gGbuffer[2].Load( positionViewport, sampleIndex);
	rawData.Specular     = gGbuffer[3].Load( positionViewport, sampleIndex).xy;

	// Decode into reasonable outputs
	SurfaceData data;

	data.position = rawData.Position;
	
	// Since ddx/ddx_coarse/ddx_fine are available only in the pixel shader, we have to approximate them ourselves.
	// we use this simple formula (thanks Soufiane Hamilton !)
	// dx = 0.5*(img[x+1][y]-img[x-1][y]
	// dy = 0.5*(img[x][y+1]-img[x][y-1]
	data.positionDX = 0.5f*(gGbuffer[1].Load( uint2(positionViewport.x+1, positionViewport.y), sampleIndex).xyz - gGbuffer[1].Load( uint2(positionViewport.x-1, positionViewport.y), sampleIndex).xyz);
	data.positionDY = 0.5f*(gGbuffer[1].Load( uint2(positionViewport.x, positionViewport.y+1), sampleIndex).xyz - gGbuffer[1].Load( uint2(positionViewport.x, positionViewport.y-1), sampleIndex).xyz);
	//data.positionDX = ddx_coarse(data.position);
	//data.positionDY = ddy_coarse(data.position);

	data.normal = rawData.Normal_Depth.xyz;
	data.albedo = rawData.Albedo;

	data.specularAmount = rawData.Specular.x;
	data.specularPower  = rawData.Specular.y;

	data.zDepth = rawData.Normal_Depth.w;

	return data;
}

void ComputeSurfaceDataFromGBufferAllSamples(Texture2DMS<float4, MSAA_SAMPLES> gGbuffer[4], uint2 positionViewport, out SurfaceData surface[MSAA_SAMPLES])
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

	const float maxZDelta = abs(surface[0].positionDX.z) + abs(surface[0].positionDY.z);
	const float minNormalDot = 0.99f;        // Allow ~8 degree normal deviations

	[unroll] for (uint i = 0; i < MSAA_SAMPLES; ++i)
	{
		// Using the position derivatives of the triangle, check if all of the sample depths
		// could possibly have come from the same triangle/surface
		perSample = perSample || abs(surface[i].position.z - surface[0].position.z) > maxZDelta;

		// Also flag places where the normal is different
		perSample = perSample || dot(surface[i].normal, surface[0].normal) < minNormalDot;

		// We use the normal value as a mask to display the skybox do not use per sample shading on skybox
		perSample = perSample && !(dot(surface[i].normal, float3(1.0,1.0,1.0)) == 0.0);
	}

	return perSample;
}

// Average All the sample so we can compute only one BRDF per pixel instead of per sample
// We average only the data used by the lighting computation for performance
SurfaceData AverageMSAASamples(SurfaceData surface[MSAA_SAMPLES])
{
	SurfaceData surfaceOut;
	surfaceOut.position       = surface[0].position;
	surfaceOut.positionDX     = surface[0].positionDX;
	surfaceOut.positionDY     = surface[0].positionDY;
	surfaceOut.normal         = surface[0].normal;
	surfaceOut.albedo         = surface[0].albedo;
	surfaceOut.specularAmount = surface[0].specularAmount;
	surfaceOut.specularPower  = surface[0].specularPower;
	surfaceOut.zDepth         = surface[0].zDepth;
	[unroll] for (uint i = 1; i < MSAA_SAMPLES; ++i)
	{
		surfaceOut.position       += surface[i].position;
		surfaceOut.normal         += surface[i].normal;
		surfaceOut.albedo         += surface[i].albedo;
		surfaceOut.specularAmount += surface[i].specularAmount;
		surfaceOut.specularPower  += surface[i].specularPower;
		surfaceOut.zDepth         += surface[i].zDepth;
	}
	float rcpMSAA = rcp(MSAA_SAMPLES);
	surfaceOut.position       *= rcpMSAA;
	surfaceOut.normal         *= rcpMSAA;
	surfaceOut.albedo         *= rcpMSAA;
	surfaceOut.specularAmount *= rcpMSAA;
	surfaceOut.specularPower  *= rcpMSAA;
	surfaceOut.zDepth         *= rcpMSAA;

	return surfaceOut;
}

float3 ProjectIntoLightTexCoord(float3 positionView)
{
	float4 positionLight = mul(float4(positionView, 1.0f), gCameraViewToLightProj);
	float3 texCoord = (positionLight.xyz / positionLight.w) * float3(0.5f, -0.5f, 1.0f) + float3(0.5f, 0.5f, 0.0f);
	return texCoord;
}

// Get the light space Texture Coordinates from the world space position and the view matrix
void ComputeLightTexCoord(SurfaceData data, out float3 lightTexCoord, out float3 lightTexCoordDX, out float3 lightTexCoordDY)
{
// 	float3 positionView   = mul(float4(data.position, 1.0f), view).xyz;
// 	float3 positionViewDX = mul(float4(data.positionDX, 1.0f), view).xyz;
// 	float3 positionViewDY = mul(float4(data.positionDY, 1.0f), view).xyz;
	float deltaPixels = 2.0f;
	lightTexCoord   = (ProjectIntoLightTexCoord(data.position));
	lightTexCoordDX = (ProjectIntoLightTexCoord(data.position + deltaPixels * data.positionDX) - lightTexCoord) / deltaPixels;
	lightTexCoordDY = (ProjectIntoLightTexCoord(data.position + deltaPixels * data.positionDY) - lightTexCoord) / deltaPixels;
}

#endif
