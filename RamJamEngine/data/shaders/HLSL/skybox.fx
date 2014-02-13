//====================================
// Skybox Shader (Forward & Deferred)
//====================================

#include "Rendering.hlsl"

cbuffer cbWorldViewProj
{
	float4x4 gWorldViewProj;
};

TextureCube gCubeMap;

// used only on deferred path
Texture2DMS<float4, MSAA_SAMPLES>	gGbuffer[4];
StructuredBuffer<uint2>				gLitTexture;

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct SkyboxIn
{
	float3 PosL : POSITION;
};

struct SkyboxOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

//////////////////////////////////////////////////////////////////////////
SkyboxOut SkyboxVS(SkyboxIn vin)
{
	SkyboxOut vout;

	// Set z = w so that z/w = 1 (i.e., skybox always on far plane).
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;

	return vout;
}

//////////////////////////////////////////////////////////////////////////
float4 SkyboxForwardPS(SkyboxOut pin) : SV_Target
{
	uint2 size;
	gCubeMap.GetDimensions(size.x, size.y);
	return (size.x == 0U ? float4(0.0f, 0.0f, 0.0f, 1.0f) : gCubeMap.Sample(samTriLinearSam, pin.PosL));
}

//////////////////////////////////////////////////////////////////////////
float4 SkyboxDeferredPS(SkyboxOut pin) : SV_Target
{
	uint2 coords = pin.PosH.xy;
	float3 lit = float3(0.0f, 0.0f, 0.0f);
	float skyboxSamples = 0.0f;

#if SHADER_DEBUG
	if (gVisualizeAlbedo)   return float4(gGbuffer[0].Load(coords, 0).xyz, 1.0);
	if (gVisualizePosition) return gGbuffer[1].Load(coords, 0);
	if (gVisualizeNormals)  return float4(gGbuffer[2].Load(coords, 0).xyz, 1.0);
	if (gVisualizeDepth)    return float4(gGbuffer[2].Load(coords, 0).w, gGbuffer[2].Load(coords, 0).w, gGbuffer[2].Load(coords, 0).w, 1.0);
	if (gVisualizeSpecular) return float4(gGbuffer[3].Load(coords, 0).xy, 0.0,1.0);
#endif

#if MSAA_SAMPLES <= 1
	[unroll]
#endif
	for (unsigned int sampleIndex = 0; sampleIndex < MSAA_SAMPLES; ++sampleIndex)
	{
		float depth = gGbuffer[2].Load(coords, sampleIndex).w;

		if (depth >= 1.0f)
			++skyboxSamples;
		else
			lit += UnpackRGBA16(gLitTexture[GetFramebufferSampleAddress(coords, sampleIndex)]).xyz;
	}

#if SHADER_DEBUG
	[flatten] if (!gVisualizeLightCount)
#endif
	{
		//If necessary, add skybox contribution
		[branch] if (skyboxSamples > 0)
		{
			float3 skybox = gCubeMap.Sample(samTriLinearSam, pin.PosL).xyz;
			// Tone map and accumulate
			lit += skyboxSamples * skybox;
		}
	}
	// Resolve MSAA samples (simple box filter)
	return float4(lit * rcp(MSAA_SAMPLES), 1.0f);
}

//--------------------------------------------------------------------------------------------------

RasterizerState NoCull
{
	CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
	DepthFunc = LESS_EQUAL;
};

technique11 SkyboxForward
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SkyboxVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, SkyboxForwardPS() ) );

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}

technique11 SkyboxDeferred
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SkyboxVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, SkyboxDeferredPS() ) );

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}
