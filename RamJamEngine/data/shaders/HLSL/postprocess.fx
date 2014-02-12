//===========================================================
// Post Process Shader used to render a textured screen quad
//===========================================================

#include "lightHelper.fx"
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

Texture2D	gTexture;
Texture2DMS<float4, MSAA_SAMPLES>	gGbuffer[4] : register(t0);
StructuredBuffer<uint2> gLitTexture : register(t1);

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//////////////////////////////////////////////////////////////////////////
QuadOut PostProcessVS(QuadIn vin)
{
	QuadOut vout;

	vout.PosH = float4(vin.PosL, 1.0);
	vout.Tex = vin.Tex;

	return vout;
}

//////////////////////////////////////////////////////////////////////////
float4 PostProcessPS(QuadOut input) : SV_Target
{	
	float4 color;
	uint2 textureDim;
	gTexture.GetDimensions(textureDim.x, textureDim.y);
	color = (textureDim.x == 0U ? float4(1.0f, 1.0f, 1.0f, 1.0f) : gTexture.Sample( samAnisotropic, input.Tex ));
	return color;
}

//////////////////////////////////////////////////////////////////////////
float4 ResolveDeferredPS(QuadOut input) : SV_Target
{
	uint2 coords = input.PosH.xy;
	float3 lit = float3(0.0f, 0.0f, 0.0f);
#if MSAA_SAMPLES <= 1
		[unroll]
#endif
	for (unsigned int sampleIndex = 0; sampleIndex < MSAA_SAMPLES; ++sampleIndex)
	{
		lit += UnpackRGBA16(gLitTexture[GetFramebufferSampleAddress(coords, sampleIndex)]).xyz;
	}

	// Resolve MSAA samples (simple box filter)
	return float4(lit * rcp(MSAA_SAMPLES), 1.0f);
}

//--------------------------------------------------------------------------------------------------

technique11 PostProcess
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, PostProcessVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PostProcessPS() ) );
	}
}

technique11 ResolveDeferred
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, PostProcessVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, ResolveDeferredPS() ) );
	}
}
