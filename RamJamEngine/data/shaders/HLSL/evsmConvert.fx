//=======================================================
// Simple Shader for EVSM conversion of the shadow depth
//=======================================================

#ifndef CONVERT_TO_EVSM_FX
#define CONVERT_TO_EVSM_FX

#include "Rendering.hlsl"

struct QuadIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float3 TanL    : TANGENT;
	float2 Tex     : TEXCOORD;
};

struct QuadOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

StructuredBuffer<Partition> gPartitions : register(t1);
Texture2DMS<float, SHADOWAA_SAMPLES> gShadowDepthTextureMS : register(t0);

//////////////////////////////////////////////////////////////////////////
float2 GetEVSMExponents(Partition partition)
{
	float2 lightSpaceExponents = float2(gPositiveExponent, gNegativeExponent);

	// Make sure exponents say consistent in light space regardless of partition
	// scaling. This prevents the exponentials from ever getting too ridiculous
	// and maintains consistency across partitions.
	// Clamp to maximum range of fp32 to prevent overflow/underflow
	return min(lightSpaceExponents / partition.scale.zz, float2(42.0f, 42.0f));
}

// Input depth should be in [0, 1]
float2 WarpDepth(float depth, float2 exponents)
{
	// Rescale depth into [-1, 1]
	depth = 2.0f * depth - 1.0f;
	float pos =  exp( exponents.x * depth);
	float neg = -exp(-exponents.y * depth);
	return float2(pos, neg);
}

//////////////////////////////////////////////////////////////////////////
QuadOut ShadowDepthToEVSMVS(QuadIn vin)
{
	QuadOut vout;

	vout.PosH = float4(vin.PosL, 1.0);
	vout.Tex = vin.Tex;

	return vout;
}

//////////////////////////////////////////////////////////////////////////
float4 ShadowDepthToEVSMPS(QuadOut input) : SV_Target
{
	float sampleWeight = 1.0f / float(SHADOWAA_SAMPLES);
	int2 coords = int2(input.PosH.xy);

	float2 exponents = GetEVSMExponents(gPartitions[gCurrentPartition]);

	// Simple average (box filter) for now
	float4 average = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sample indices to Load() must be literal, so force unroll
	[unroll] for (int i = 0; i < SHADOWAA_SAMPLES; ++i)
	{
		// Convert to EVSM representation
		// NOTE: D3D documentation appears to be wrong on the syntax of Load from Texture2DMS...
		float depth = gShadowDepthTextureMS.Load(coords, i);

		float2 warpedDepth = WarpDepth(depth, exponents);
		average += sampleWeight * float4(warpedDepth.xy, warpedDepth.xy * warpedDepth.xy);
	}

	return average;
}

//-----------------------------------------------------------------------------------

technique11 EVSMConvert
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, ShadowDepthToEVSMVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, ShadowDepthToEVSMPS() ) );
	}
}
#endif
