//=============================================================================
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "lightHelper.fx"
#include "Rendering.hlsl"

Texture2D		gDiffuseMap : Texture_Diffuse;
SamplerState	gTextureSampler;

//////////////////////////////////////////////////////////////////////////
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorld);		// Non-uniform scaling needs inverseTranspose !! i.e. mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.TanW    = mul(vin.TanL, (float3x3)gWorld);
		
	// Transform to homogeneous clip space.
	float4x4 worldViewProj = mul(gWorld, gViewProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gDiffuseMapTrf).xy;
	
	return vout;
}
 
//////////////////////////////////////////////////////////////////////////
float4 PS(VertexOut pin) : SV_Target
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
	[flatten] if(gUseTexture)
	{
		// Sample texture.
		uint2 textureDim;
		gDiffuseMap.GetDimensions(textureDim.x, textureDim.y);
		texColor = (textureDim.x == 0U ? float4(1.0f, 1.0f, 1.0f, 1.0f) : gDiffuseMap.Sample( gTextureSampler, pin.Tex ));

		if(gMatUseAlpha)
		{
			// Discard pixel if texture alpha < 0.1.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
			clip(texColor.a - 0.1f);
		}
	}
	
	//
	// Lighting.
	//

	float4 litColor = texColor;
	
	// Start with a sum of zero. 
	float4 diffuse     = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec        = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.  
	float4 D, S;
	Material mat;
	mat.Albedo         = gMatAlbedo;
	mat.SpecularAmount = gMatSpecularAmount;
	mat.SpecularPower  = gMatSpecularPower;
	uint totalLights, dummy;

	// Directionnal Lighting
	gDirLights.GetDimensions(totalLights, dummy);
	for (uint dirLightIdx = 0; dirLightIdx < totalLights; ++dirLightIdx)
	{
		DirectionalLight light = gDirLights[dirLightIdx];
		ComputeDirectionalLight(mat, light, pin.NormalW, toEye, D, S);

		diffuse += D;
		spec    += S * float4(light.Color, 1.0);
	}

	// Point Lighting
	gPointLights.GetDimensions(totalLights, dummy);
	for (uint pointLightIdx = 0; pointLightIdx < totalLights; ++pointLightIdx)
	{
		PointLight light = gPointLights[pointLightIdx];
		ComputePointLight(mat, light, pin.PosW, pin.NormalW, toEye, D, S);

		diffuse += D;
		spec    += S * float4(light.Color, 1.0);
	}

	// Spot Lighting
	gSpotLights.GetDimensions(totalLights, dummy);
	for (uint spotLightIdx = 0; spotLightIdx < totalLights; ++spotLightIdx)
	{
		SpotLight light = gSpotLights[spotLightIdx];
		ComputeSpotLight(mat, light, pin.PosW, pin.NormalW, toEye, D, S);

		diffuse += D;
		spec    += S * float4(light.Color, 1.0);
	}
	
	// Modulate with late add.
	litColor = texColor*(gAmbientLightColor + diffuse) + spec;

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

	return litColor;
}

//////////////////////////////////////////////////////////////////////////
Gbuffer GbufferPS(VertexOut pin)
{
	Gbuffer gbuffer;
	SurfaceData surface = ComputeSurfaceDataFromGeometry(pin, gDiffuseMap, gTextureSampler);

	gbuffer.Position     = surface.position;
	gbuffer.Albedo       = surface.albedo;
	gbuffer.Normal_Depth = float4(surface.normal, 1-(pin.PosH.z/pin.PosH.w));
	gbuffer.Specular     = float2(surface.specularAmount, surface.specularPower);
	return gbuffer;
}


//--------------------------------------------------------------------------------------------------

technique11 Basic
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS() ) );
	}
}

technique11 Deferred
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, GbufferPS() ) );
	}
}
