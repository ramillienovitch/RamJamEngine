//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "PerFrameConstants.hlsl"

cbuffer cbPerObject
{
	float4   gColor;
	float4x4 gWorld; 
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	float4x4 worldViewProj = mul(gWorld, gViewProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
	
	// Just pass vertex color into the pixel shader.
	vout.Color = vin.Color * gColor;
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS() ) );
	}
}
