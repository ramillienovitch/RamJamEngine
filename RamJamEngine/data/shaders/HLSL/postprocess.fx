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
	uint3 gbufferSize;
	gGbuffer[0].GetDimensions(gbufferSize.x, gbufferSize.y, gbufferSize.z);
	uint2 pixelCoords = uint2(input.Tex.x * gbufferSize.x, input.Tex.y * gbufferSize.y);

	SurfaceData surfaceSamples[MSAA_SAMPLES];
	ComputeSurfaceDataFromGBufferAllSamples(gGbuffer, pixelCoords, surfaceSamples);
	bool perSampleShading = RequiresPerSampleShading(surfaceSamples);
	
#if SHADER_DEBUG
	SurfaceData averagedData = AverageMSAASamples(surfaceSamples);
	if (gVisualizePosition)		return float4(averagedData.positionView, 1.0);
	if (gVisualizeAlbedo)		return averagedData.albedo;
	if (gVisualizeNormals)		return float4(averagedData.normal, 1.0);
	if (gVisualizeSpecular)		return float4(averagedData.specularAmount, averagedData.specularPower, 0.0, 1.0);
	//return float4(averagedData.zDepth, averagedData.zDepth, averagedData.zDepth, 1.0);
#endif

	//////////////////////////////////////////////////////////////////////////
	
	float3 toEye = gEyePosW - surfaceSamples[0].positionView;
	float distToEye = length(toEye); 
	toEye = normalize(toEye);

	float3 lit = float3(0.0f, 0.0f, 0.0f);

	[branch] if (gVisualizePerSampleShading && perSampleShading)
	{
		return float4(1.0, 0.0, 0.0, 1.0);
	}
	else
	{
		uint totalLights, dummy;
		gDirLights.GetDimensions(totalLights, dummy);
		for (uint dirLightIdx = 0; dirLightIdx < totalLights; ++dirLightIdx)
		{
			[branch] if (perSampleShading)
			{
				DirectionalLight light = gDirLights[dirLightIdx];
				for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
				{
					AccumulateDirBRDF(surfaceSamples[sample], light, toEye, lit, MSAA_SAMPLES);
				}
			}
			else
			{
				DirectionalLight light = gDirLights[dirLightIdx];
				AccumulateDirBRDF(surfaceSamples[0], light, toEye, lit, 1);
			}
		}
		gPointLights.GetDimensions(totalLights, dummy);
		for (uint pointLightIdx = 0; pointLightIdx < totalLights; ++pointLightIdx)
		{
			[branch] if (perSampleShading)
			{
				PointLight light = gPointLights[pointLightIdx];
				for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
				{
					AccumulatePointBRDF(surfaceSamples[sample], light, toEye, lit, MSAA_SAMPLES);
				}
			}
			else
			{
				PointLight light = gPointLights[pointLightIdx];
				AccumulatePointBRDF(surfaceSamples[0], light, toEye, lit, 1);
			}
		}
		gSpotLights.GetDimensions(totalLights, dummy);
		for (uint spotLightIdx = 0; spotLightIdx < totalLights; ++spotLightIdx)
		{
			[branch] if (perSampleShading)
			{
				SpotLight light = gSpotLights[spotLightIdx];
				for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
				{
					AccumulateSpotBRDF(surfaceSamples[sample], light, toEye, lit, MSAA_SAMPLES);
				}
			}
			else
			{
				SpotLight light = gSpotLights[spotLightIdx];
				AccumulateSpotBRDF(surfaceSamples[0], light, toEye, lit, 1);
			}
		}
	}
	
	float4 litColor = float4(0.0, 0.0, 0.0, 0.0);
	[branch] if (perSampleShading)
	{
		for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
		{
			[branch] if (dot(surfaceSamples[sample].normal, float3(1.0,1.0,1.0)) == 0.0)
				litColor += surfaceSamples[sample].albedo;
			else
				litColor += surfaceSamples[sample].albedo * gAmbientLightColor + float4(lit, 1.0);
		}
		litColor *= rcp(MSAA_SAMPLES);
	}
	else
	{
		[branch] if (dot(surfaceSamples[0].normal, float3(1.0,1.0,1.0)) == 0.0)
			litColor = surfaceSamples[0].albedo;
		else
			litColor = surfaceSamples[0].albedo * gAmbientLightColor + float4(lit, 1.0);
	}
	//float4 litColor = averagedAlbedo * gAmbientLightColor + float4(lit, 1.0);

	[flatten] if( gUseFog )
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
