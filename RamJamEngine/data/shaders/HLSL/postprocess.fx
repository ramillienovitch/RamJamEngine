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
	if (gVisualizeDepth)
	{
		float zBuffer = gGbuffer[4].Sample( samAnisotropic, input.Tex ).x;
		return float4(zBuffer, zBuffer, zBuffer, 1.0);
	}

	//////////////////////////////////////////////////////////////////////////
	
	VertexOut vout;
	vout.PosW    = gGbuffer[0].Sample( samAnisotropic, input.Tex ).xyz;
	vout.NormalW = gGbuffer[2].Sample( samAnisotropic, input.Tex ).xyz;

	float4 litColor;

	// Start with a sum of zero.
	float4 albedo  = gUseTexture ? gGbuffer[1].Sample( samAnisotropic, input.Tex ) : float4(1.0,1.0,1.0,1.0);
	float4 diffuse = gAmbientLightColor;
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float3 toEye = gEyePosW - vout.PosW;
	float distToEye = length(toEye); 
	toEye = normalize(toEye);
	float4 D, S;
	Material mat;
	mat.Albedo         = float4(1.0,1.0,1.0,1.0);
	float2 matSpecular = gGbuffer[3].Sample( samAnisotropic, input.Tex ).xy;
	mat.SpecularAmount = matSpecular.x;
	mat.SpecularPower  = matSpecular.y;
	uint totalLights, dummy;

	float3 lit;

	// Directionnal Lighting
	gDirLights.GetDimensions(totalLights, dummy);
	for (uint dirLightIdx = 0; dirLightIdx < totalLights; ++dirLightIdx)
	{
		DirectionalLight light = gDirLights[dirLightIdx];
		ComputeDirectionalLight(mat, light, vout.NormalW, toEye, D, S);

		diffuse += D;
		spec    += S * float4(light.Color, 1.0);
	}
	
	// Point Lighting
	gPointLights.GetDimensions(totalLights, dummy);
	SurfaceData surface;
	surface.positionView   = vout.PosW;
	surface.normal         = vout.NormalW;
	surface.albedo         = albedo;
	surface.specularAmount = mat.SpecularAmount;
	surface.specularPower  = mat.SpecularPower;
	for (uint pointLightIdx = 0; pointLightIdx < totalLights; ++pointLightIdx)
	{
		PointLight light = gPointLights[pointLightIdx];
// 		ComputePointLight(mat, light, vout.PosW, vout.NormalW, toEye, D, S);
// 
// 		diffuse += D;
// 		spec    += S * float4(light.Color, 1.0);
		AccumulateBRDF(surface, light, lit);
	}

	// Spot Lighting
	gSpotLights.GetDimensions(totalLights, dummy);
	for (uint spotLightIdx = 0; spotLightIdx < totalLights; ++spotLightIdx)
	{
		SpotLight light = gSpotLights[spotLightIdx];
		ComputeSpotLight(mat, light, vout.PosW, vout.NormalW, toEye, D, S);

		diffuse += D;
		spec    += S * float4(light.Color, 1.0);
	}

	litColor = float4(lit, 1.0) + albedo*diffuse + spec;

	if( gUseFog )
	{
		float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 

		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

	return litColor;
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
