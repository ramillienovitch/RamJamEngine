//===========================================================
// Post Process Shader used to render a textured screen quad
//===========================================================

#include "../../../include/ShaderDefines.h"

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

Texture2D							gTexture;
Texture2DMS<float4, MSAA_SAMPLES>	gTextureMS;

uint gTextureSizeX;
uint gTextureSizeY;

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
float4 PostProcessPSMS(QuadOut input) : SV_Target
{	
	float4 color;
	uint3 textureDim;
	gTextureMS.GetDimensions(textureDim.x, textureDim.y, textureDim.z);

	uint2 sampleCoords = uint2(input.Tex.x*gTextureSizeX, input.Tex.y*gTextureSizeY);
	[unroll] for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
	{
		color = gTextureMS.Load( sampleCoords, sample);
	}
	color *= rcp(MSAA_SAMPLES);

	color = (textureDim.x == 0U ? float4(1.0f, 1.0f, 1.0f, 1.0f) : color);
	return color;
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


technique11 PostProcessMS
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, PostProcessVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PostProcessPSMS() ) );
	}
}
