//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "lightHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight	gDirLights[MAX_LIGHT_DIR];
	PointLight			gPointLights[MAX_POINT_DIR];
	float3 gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
}; 

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gMaskMap;

SamplerState gTextureSampler;

// SamplerState samAnisotropic
// {
// 	Filter = ANISOTROPIC;
// 	MaxAnisotropy = 4;
// 
// 	AddressU = WRAP;
// 	AddressV = WRAP;
// };

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	
	return vout;
}
 
float4 PS(VertexOut pin, uniform int gDirLightCount, uniform int gPointLightCount, uniform bool gUseTexure) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye); 

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	if(gUseTexure)
	{
		// Sample texture.
		texColor = gDiffuseMap.Sample( gTextureSampler, pin.Tex ) * gMaskMap.Sample( gTextureSampler, pin.Tex );
	}
	
	//
	// Lighting.
	//

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float4 litColor = texColor;

	// Sum the light contribution from each light source.  
	float4 A, D, S;
	[unroll]
	for(int i = 0; i < gDirLightCount; ++i)
	{
		ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, A, D, S);

		ambient += A;
		diffuse += D;
		spec    += S;
	}

	[unroll]
	for(int j = 0; j < gPointLightCount; ++j)
	{
		ComputePointLight(gMaterial, gPointLights[j], pin.PosW, pin.NormalW, toEye, A, D, S);

		ambient += A;
		diffuse += D;
		spec    += S;
	}

	// Modulate with late add.
	litColor = texColor*(ambient + diffuse) + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}

//--------------------------------------------------------------------------------------------------

technique11 Light0_0
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(0,0, true) ) );
	}
}

technique11 Light0_1
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(0,1, true) ) );
	}
}

technique11 Light0_2
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(0,2, true) ) );
	}
}

technique11 Light0_3
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(0,3, true) ) );
	}
}

technique11 Light1_0
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(1,0, true) ) );
	}
}

technique11 Light2_0
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(2,0, true) ) );
	}
}

technique11 Light3_0
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(3,0, true) ) );
	}
}

technique11 Light1_1
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(1,1, true) ) );
	}
}

technique11 Light2_1
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(2,1, true) ) );
	}
}

technique11 Light3_1
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(3,1, true) ) );
	}
}

technique11 Light1_2
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(1,2, true) ) );
	}
}

technique11 Light2_2
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(2,2, true) ) );
	}
}

technique11 Light3_2
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(3,2, true) ) );
	}
}

technique11 Light1_3
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(1,3, true) ) );
	}
}

technique11 Light2_3
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(2,3, true) ) );
	}
}

technique11 Light3_3
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(3,3, true) ) );
	}
}

technique11 Light1_3NoTex
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS(1,3, false) ) );
	}
}