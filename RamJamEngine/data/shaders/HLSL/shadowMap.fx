//======================================================
// Simple shader for shadow map and shadow accumulation
//======================================================

#include "lightHelper.fx"
#include "Rendering.hlsl"

cbuffer cbObject
{
	float4x4 gWVP;
};

Texture2DMS<float4, MSAA_SAMPLES> gGbuffer[4];
StructuredBuffer<Partition>       gPartitions : register(t0);
Texture2DArray                    gShadowTexture : register(t5);

//-------------------
struct ShadowVertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float3 TanL    : TANGENT;
	float2 Tex     : TEXCOORD;
};

//-------------------
struct QuadOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////
float2 GetEVSMExponents(Partition partition)
{
	float2 lightSpaceExponents = float2(gPositiveExponent, gNegativeExponent);

	// Make sure exponents say consistent in light space regardless of partition
	// scaling. This prevents the exponentials from ever getting too ridiculous
	// and maintains consistency across partitions.
	// Clamp to maximum range of fp32 to prevent overflow/underflow
	return min(lightSpaceExponents / partition.scale.zz, float2(42.0f, 42.0f));
	//return float2(42.0f, 42.0f);
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

float3 GetPartitionColor(uint p)
{  
	uint perm = (p % 7) + 1;
	return float3(	perm & 1 ? 0.0f : 1.0f,
					perm & 2 ? 0.0f : 1.0f,
					perm & 4 ? 0.0f : 1.0f);
}

//--------------------------------------------------------------------------------------
// Lookup and filtering
//--------------------------------------------------------------------------------------
SamplerState gShadowSampler
{
	Filter = ANISOTROPIC;

	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MaxAnisotropy = 16;
};

float ChebyshevUpperBound(float2 moments, float mean, float minVariance)
{
	// Compute variance
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, minVariance);

	// Compute probabilistic upper bound
	float d = mean - moments.x;
	float pMax = variance / (variance + (d * d));

	// One-tailed Chebyshev
	return (mean <= moments.x ? 1.0f : pMax);
}

//////////////////////////////////////////////////////////////////////////
float ShadowContribution(float3 texCoord, float3 texCoordDX, float3 texCoordDY, float depth, Partition partition)
{
	float2 exponents = GetEVSMExponents(partition);
	float2 warpedDepth = WarpDepth(depth, exponents);

	float4 occluder = gShadowTexture.SampleGrad(gShadowSampler, float3(texCoord.xy, 0), texCoordDX.xy, texCoordDY.xy);

	// Derivative of warping at depth
	// TODO: Parameterize min depth stddev
	float2 depthScale = 0.0001f * exponents * warpedDepth;
	float2 minVariance = depthScale * depthScale;

	float posContrib = ChebyshevUpperBound(occluder.xz, warpedDepth.x, minVariance.x);
	float negContrib = ChebyshevUpperBound(occluder.yw, warpedDepth.y, minVariance.y);

	float shadowContrib = gUsePositiveExponent > 0 ? posContrib : 1.0f;
	shadowContrib       = gUseNegativeExponent > 0 ? min(shadowContrib, negContrib) : shadowContrib;

	return shadowContrib;
}

//////////////////////////////////////////////////////////////////////////
float4 ShadowVS(ShadowVertexIn vin) : SV_POSITION
{
	float4 position = mul(float4(vin.PosL, 1.0f), gWVP);

	// Scale/bias in NDC space
	Partition partition = gPartitions[gCurrentPartition];
	position.xy *= partition.scale.xy;
	position.x += (2.0f * partition.bias.x + partition.scale.x - 1.0f);
	position.y -= (2.0f * partition.bias.y + partition.scale.y - 1.0f);

	// Push the surface back ever-so-slightly
	// The only reason for this is because of interpolant and position reconstruction
	// precision issues on some GPUs/G-buffer layouts.
	position.z += 0.0001f;

	// Clamp to [0, 1] happens in the viewport transform
	// NOTE: Depth clipping is disabled in this pass (directional light)
	position.z = position.z * partition.scale.z + partition.bias.z;

	return position;
}

//////////////////////////////////////////////////////////////////////////
QuadOut FullscreenVS(ShadowVertexIn vin)
{
	QuadOut vout;

	vout.PosH = float4(vin.PosL, 1.0);
	vout.Tex = vin.Tex;

	return vout;
}

//////////////////////////////////////////////////////////////////////////
float4 AccumulateShadowPS(QuadOut input) : SV_Target
{
	SurfaceData surface = ComputeSurfaceDataFromGBufferSample(gGbuffer, uint2(input.PosH.xy), 0);
	float3 lit = float3(0.0f, 0.0f, 0.0f);

	// Early out if we don't fall into any of the relevant partitions
	float3 samplePosView = mul(float4(surface.position, 1.0f), gView).xyz;
	float viewSpaceZ     = samplePosView.z;
	bool inAnyPartition  = ( viewSpaceZ >= gPartitions[gCurrentPartition].intervalBegin && viewSpaceZ < gPartitions[gCurrentPartition].intervalEnd && surface.zDepth < 1.0);

	if (inAnyPartition) //&& nDotL > 0.0f)
	{
		Partition partition = gPartitions[gCurrentPartition];

		if (viewSpaceZ < partition.intervalEnd)
		{
			float3 lightTexCoord, lightTexCoordDX, lightTexCoordDY;
			ComputeLightTexCoord(surface, lightTexCoord, lightTexCoordDX, lightTexCoordDY);
			float3 texCoord   = lightTexCoord   * partition.scale.xyz + partition.bias.xyz;
			float3 texCoordDX = lightTexCoordDX * partition.scale.xyz;
			float3 texCoordDY = lightTexCoordDY * partition.scale.xyz;
			float depth = clamp(texCoord.z, 0.0f, 1.0f);

			// we use reverse subtract blending were the shadow contributes;
			float shadowContrib = ShadowContribution(texCoord, texCoordDX, texCoordDY, depth, partition);
			shadowContrib = 1-shadowContrib;
			shadowContrib *= 0.2f*gShadowStrength;
			//return float4(shadowContrib.xxx, 1.0);

			float3 contrib = shadowContrib.xxx;
			//float3 contrib = shadowContrib * (nDotL * surface.albedo.xyz);
			//contrib = shadowContrib < 0.9f ? contrib * (shadowContrib*2.0f*gShadowStrength) : contrib * shadowContrib;
			//contrib *= shadowContrib;

			if (gVisualizePartitions)
			{
				contrib = lerp(contrib, GetPartitionColor(gCurrentPartition), 0.25f);
			}

			// Accumulate lighting
			lit += contrib;
		}
	}
	else
	{
		discard;
	}

	return float4(lit, 1.0f);
}

RasterizerState rsShadowMap { DepthClipEnable = false; };

//////////////////////////////////////////////////////////////////////////
technique11 ShadowMap
{
	pass P0
	{
		SetRasterizerState( rsShadowMap );
		SetVertexShader( CompileShader( vs_5_0, ShadowVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( NULL );
	}
}

technique11 AccumulateShadow
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, FullscreenVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, AccumulateShadowPS() ) );
	}
}
