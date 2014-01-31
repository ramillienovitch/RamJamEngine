//===========================================================
// Post Process Shader used to render a textured screen quad
//===========================================================

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
Texture2D	gGbuffer[5] : register(t0);

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
	if (gVisualizePosition)		return gGbuffer[0].Sample( samAnisotropic, input.Tex );
	if (gVisualizeAlbedo)		return gGbuffer[1].Sample( samAnisotropic, input.Tex );
	if (gVisualizeNormals)		return gGbuffer[2].Sample( samAnisotropic, input.Tex );
	if (gVisualizeSpecular)		return gGbuffer[3].Sample( samAnisotropic, input.Tex );
	if (gVisualizeDepth)		return float4(gGbuffer[4].Sample( samAnisotropic, input.Tex ).x, 0.0, 0.0, 1.0);
	return float4(1.0,1.0,1.0,1.0);

	//////////////////////////////////////////////////////////////////////////
	/*
	float4 litColor = texColor;

	// Start with a sum of zero. 
	float4 diffuse     = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec        = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 coloredSpec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.  
	float4 A, D, S;
	Material mat;
	mat.Albedo         = gMatAlbedo;
	mat.SpecularAmount = 0.8;//gMatSpecularAmount;
	mat.SpecularPower  = 16.0;//gMatSpecularPower;
	uint totalLights, dummy;

	// Directionnal Lighting
	gDirLights.GetDimensions(totalLights, dummy);
	for (uint dirLightIdx = 0; dirLightIdx < totalLights; ++dirLightIdx)
	{
		DirectionalLight light = gDirLights[dirLightIdx];
		ComputeDirectionalLight(mat, light, pin.NormalW, toEye, D, S);

		diffuse     += D;
		spec        += S;
		coloredSpec += S * float4(light.Color, 1.0);
	}

	// Point Lighting
	gPointLights.GetDimensions(totalLights, dummy);
	for (uint pointLightIdx = 0; pointLightIdx < totalLights; ++pointLightIdx)
	{
		PointLight light = gPointLights[pointLightIdx];
		ComputePointLight(mat, light, pin.PosW, pin.NormalW, toEye, D, S);

		diffuse     += D;
		spec        += S;
		coloredSpec += S * float4(light.Color, 1.0);
	}

	// Spot Lighting
	gSpotLights.GetDimensions(totalLights, dummy);
	for (uint spotLightIdx = 0; spotLightIdx < totalLights; ++spotLightIdx)
	{
		SpotLight light = gSpotLights[spotLightIdx];
		ComputeSpotLight(mat, light, pin.PosW, pin.NormalW, toEye, D, S);

		diffuse     += D;
		spec        += S;
		coloredSpec += S * float4(light.Color, 1.0);
	}

	if (gVisualizeSpecular)
		return spec;

	// Modulate with late add.
	litColor = texColor*(gAmbientLightColor + diffuse) + coloredSpec;

	//
	// Fogging
	//

	if( gUseFog )
	{
		float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 

		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

	if (gMatUseAlpha)
	{
		// Common to take alpha from diffuse material.
		litColor.a = gMatAlbedo.a * texColor.a;
	}
	else
	{
		litColor.a = 1.0;
	}
	*/
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
