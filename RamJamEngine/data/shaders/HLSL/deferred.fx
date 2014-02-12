//=============================================
// Compute Shader for Tiled Deferred Rendering 
//=============================================

#include "lightHelper.fx"
#include "Rendering.hlsl"

Texture2DMS<float4, MSAA_SAMPLES>	gGbuffer[4] : register(t0);
RWStructuredBuffer<uint2>			gLitbuffer  : register(u0);

groupshared uint sMinZ;
groupshared uint sMaxZ;

// Light list for the tile
groupshared uint sTileLightIndices[MAX_LIGHTS];
groupshared uint sTileNumLights;

// List of pixels that require per-sample shading
// We encode two 16-bit x/y coordinates in one uint to save shared memory space
groupshared uint sPerSamplePixels[COMPUTE_SHADER_TILE_GROUP_SIZE];
groupshared uint sNumPerSamplePixels;


//////////////////////////////////////////////////////////////////////////
void WriteSample(uint2 coords, uint sampleIndex, float4 value)
{
	gLitbuffer[GetFramebufferSampleAddress(coords, sampleIndex)] = PackRGBA16(value);
}

// Pack and unpack two <=16-bit coordinates into a single uint
uint PackCoords(uint2 coords)
{
	return coords.y << 16 | coords.x;
}
uint2 UnpackCoords(uint coords)
{
	return uint2(coords & 0xFFFF, coords >> 16);
}

//////////////////////////////////////////////////////////////////////////
[numthreads(16, 16, 1)]
void ComputeShaderTileCS(uint3 groupId          : SV_GroupID,
						 uint3 dispatchThreadId : SV_DispatchThreadID,
						 uint3 groupThreadId    : SV_GroupThreadID)
{
	// NOTE: This is currently necessary rather than just using SV_GroupIndex to work around a compiler bug on Fermi.
	uint groupIndex = groupThreadId.y * COMPUTE_SHADER_TILE_GROUP_DIM + groupThreadId.x;

	// How many total lights?
	uint totalLights, dummy;
	gPointLights.GetDimensions(totalLights, dummy);

	uint2 globalCoords = dispatchThreadId.xy;

	SurfaceData surfaceSamples[MSAA_SAMPLES];
	ComputeSurfaceDataFromGBufferAllSamples(gGbuffer, globalCoords, surfaceSamples);

	float3 toEye = gEyePosW - surfaceSamples[0].position;
	float distToEye = length(toEye); 
	toEye = normalize(toEye);

	// Work out Z bounds for our samples
	float minZSample = gNearFar.y;
	float maxZSample = gNearFar.x;
	{
		[unroll] for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
		{
			// Avoid shading skybox/background or otherwise invalid pixels
			float3 samplePosView = mul(float4(surfaceSamples[sample].position, 1.0f), gView).xyz;
			float viewSpaceZ = samplePosView.z;
			//float viewSpaceZ = surfaceSamples[sample].position.z;
			//bool validPixel = viewSpaceZ >= gNearFar.x && viewSpaceZ < gNearFar.y;
			float depth = surfaceSamples[sample].zDepth;
			bool validPixel = /*depth >= 0.0 &&*/ depth < 1.0;
			[flatten] if (validPixel)
			{
				minZSample = min(minZSample, viewSpaceZ);
				maxZSample = max(maxZSample, viewSpaceZ);
			}
		}
	}

	// Initialize shared memory light list and Z bounds
	if (groupIndex == 0)
	{
		sTileNumLights = 0;
		sNumPerSamplePixels = 0;
		sMinZ = 0x7F7FFFFF;      // Max float
		sMaxZ = 0;
	}

	GroupMemoryBarrierWithGroupSync();

	// NOTE: Can do a parallel reduction here but now that we have MSAA and store sample frequency pixels
	// in shaded memory the increased shared memory pressure actually *reduces* the overall speed of the kernel.
	// Since even in the best case the speed benefit of the parallel reduction is modest on current architectures
	// with typical tile sizes, we have reverted to simple atomics for now.
	// Only scatter pixels with actual valid samples in them
	if (maxZSample >= minZSample)
	{
		InterlockedMin(sMinZ, asuint(minZSample));
		InterlockedMax(sMaxZ, asuint(maxZSample));
	}

	GroupMemoryBarrierWithGroupSync();

	float minTileZ = asfloat(sMinZ);
	float maxTileZ = asfloat(sMaxZ);

	// NOTE: This is all uniform per-tile (i.e. no need to do it per-thread) but fairly inexpensive
	// We could just precompute the frusta planes for each tile and dump them into a constant buffer...
	// They don't change unless the projection matrix changes since we're doing it in view space.
	// Then we only need to compute the near/far ones here tightened to our actual geometry.
	// The overhead of group synchronization/LDS or global memory lookup is probably as much as this
	// little bit of math anyways, but worth testing.

	// Work out scale/bias from [0, 1]
	float2 tileScale = float2(gFramebufferSizeX, gFramebufferSizeY) * rcp(float(2 * COMPUTE_SHADER_TILE_GROUP_DIM));
	float2 tileBias  = tileScale - float2(groupId.xy);

	// Now work out composite projection matrix
	// Relevant matrix columns for this tile frusta
	float4 c1 = float4(gProj._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
	float4 c2 = float4(0.0f, -gProj._22 * tileScale.y, tileBias.y, 0.0f);
	float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

	// Derive frustum planes
	float4 frustumPlanes[6];
	// Sides
	frustumPlanes[0] = c4 - c1;
	frustumPlanes[1] = c4 + c1;
	frustumPlanes[2] = c4 - c2;
	frustumPlanes[3] = c4 + c2;
	// Near/far
	frustumPlanes[4] = float4(0.0f, 0.0f,  1.0f, -minTileZ);
	frustumPlanes[5] = float4(0.0f, 0.0f, -1.0f, maxTileZ);

	// Normalize frustum planes (near/far already normalized)
	[unroll] for (uint i = 0; i < 4; ++i)
	{
		frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
	}

	// Cull lights for this tile
	for (uint lightIndex = groupIndex; lightIndex < totalLights; lightIndex += COMPUTE_SHADER_TILE_GROUP_SIZE)
	{
		PointLight light = gPointLights[lightIndex];

		// Cull: point light sphere vs tile frustum
		bool inFrustum = true;
		[unroll] for (uint i = 0; i < 6; ++i)
		{
			float3 lightPosView = mul(float4(light.Position, 1.0f), gView).xyz;
			float d = dot(frustumPlanes[i], float4(lightPosView, 1.0f));
			//float d = dot(frustumPlanes[i], float4(light.Position, 1.0f));
			inFrustum = inFrustum && (d >= -light.Range);
		}

		[branch] if (inFrustum)
		{
			// Append light to list
			// Compaction might be better if we expect a lot of lights
			uint listIndex;
			InterlockedAdd(sTileNumLights, 1, listIndex);
			sTileLightIndices[listIndex] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();

	uint numLights = sTileNumLights;

	// Only process onscreen pixels (tiles can span screen edges)
	if (all(globalCoords < uint2(gFramebufferSizeX, gFramebufferSizeY)))
	{
		[branch] if (gVisualizeLightCount)
		{
			[unroll] for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
			{
				WriteSample(globalCoords, sample, (float(sTileNumLights) / 255.0f).xxxx);
			}
		}
		else if (numLights > 0)
		{
			bool perSampleShading = RequiresPerSampleShading(surfaceSamples);
			[branch] if (gVisualizePerSampleShading && perSampleShading)
			{
				[unroll] for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
				{
					WriteSample(globalCoords, sample, float4(1, 0, 0, 1));
				}
			}
			else
			{
				float3 lit = float3(0.0f, 0.0f, 0.0f);
					for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex)
					{
						PointLight light = gPointLights[sTileLightIndices[tileLightIndex]];
						AccumulateBRDF(surfaceSamples[0], light, toEye,lit);
					}

					// Write sample 0 result
					WriteSample(globalCoords, 0, float4(lit, 1.0f));

					[branch] if (perSampleShading)
					{
#if DEFER_PER_SAMPLE
						// Create a list of pixels that need per-sample shading
						uint listIndex;
						InterlockedAdd(sNumPerSamplePixels, 1, listIndex);
						sPerSamplePixels[listIndex] = PackCoords(globalCoords);
#else
						// Shade the other samples for this pixel
						for (uint sample = 1; sample < MSAA_SAMPLES; ++sample)
						{
							float3 litSample = float3(0.0f, 0.0f, 0.0f);
							for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex)
							{
								PointLight light = gPointLights[sTileLightIndices[tileLightIndex]];
								AccumulateBRDF(surfaceSamples[sample], light, toEye, litSample);
							}
							WriteSample(globalCoords, sample, float4(litSample, 1.0f));
						}
#endif
					}
					else 
					{
						// Otherwise per-pixel shading, so splat the result to all samples
						[unroll] for (uint sample = 1; sample < MSAA_SAMPLES; ++sample)
						{
							WriteSample(globalCoords, sample, float4(lit, 1.0f));
						}
					}
			}
		}
		else
		{
			// Otherwise no lights affect here so clear all samples
			[unroll] for (uint sample = 0; sample < MSAA_SAMPLES; ++sample)
			{
				WriteSample(globalCoords, sample, float4(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
	}

#if DEFER_PER_SAMPLE && MSAA_SAMPLES > 1
	// NOTE: We were careful to write only sample 0 above if we are going to do sample
	// frequency shading below, so we don't need a device memory barrier here.
	GroupMemoryBarrierWithGroupSync();

	// Now handle any pixels that require per-sample shading
	// NOTE: Each pixel requires MSAA_SAMPLES - 1 additional shading passes
	const uint shadingPassesPerPixel = MSAA_SAMPLES - 1;
	uint globalSamples = sNumPerSamplePixels * shadingPassesPerPixel;

	for (uint globalSample = groupIndex; globalSample < globalSamples; globalSample += COMPUTE_SHADER_TILE_GROUP_SIZE)
	{
		uint listIndex = globalSample / shadingPassesPerPixel;
		uint sampleIndex = globalSample % shadingPassesPerPixel + 1;        // sample 0 has been handled earlier

		uint2 sampleCoords = UnpackCoords(sPerSamplePixels[listIndex]);
		SurfaceData surface = ComputeSurfaceDataFromGBufferSample(gGbuffer, sampleCoords, sampleIndex);

		float3 lit = float3(0.0f, 0.0f, 0.0f);
		for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex)
		{
			PointLight light = gPointLights[sTileLightIndices[tileLightIndex]];
			AccumulateBRDF(surface, light, toEye,lit);
		}
		WriteSample(sampleCoords, sampleIndex, float4(lit, 1.0f));
	}
#endif
}

technique11 TiledDeferred
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ComputeShaderTileCS() ) );
	}
}