//========================================================
// Compute Shaders for Sample Distribution Shadow Mapping 
//========================================================

#include "Rendering.hlsl"

cbuffer SDSMPartitionsConstants
{
	float4 mLightSpaceBorder;
	float4 mMaxScale;
	float  mDilationFactor;
	uint   mReduceTileDim;
}

Texture2DMS<float4, MSAA_SAMPLES>  gGbuffer[4];
RWStructuredBuffer<Partition>      gPartitions : register(u6);
RWStructuredBuffer<PartitionUint>  gPartitionsUint : register(u6);
StructuredBuffer<Partition>        gPartitionsReadOnly : register(t6);
RWStructuredBuffer<BoundsUint>     gPartitionBoundsUint : register(u7);
StructuredBuffer<BoundsFloat>      gPartitionBoundsReadOnly : register(t7);

//////////////////////////////////////////////////////////////////////////
// Utility function that uses partitionsBounds to generate zoom regions (scale/bias)
void ComputePartitionDataFromBounds(BoundsFloat bounds, out float3 scale, out float3 bias)
{
	float3 minTexCoord = bounds.minCoord;
	float3 maxTexCoord = bounds.maxCoord;

	// Ensure our partitions have enough of a border for edge softening (blurring)
	// Also ensure our partition depth ranges stay far enough away from the geometry
	minTexCoord -= mLightSpaceBorder.xyz;
	maxTexCoord += mLightSpaceBorder.xyz;

	scale = 1.0f / (maxTexCoord - minTexCoord);
	bias = -minTexCoord * scale;

	// Dilate
	// TODO: This isn't necessarily required... we could instead just clamp mLightSpaceBorder
	// to something fairly small to cover anisotropic kernels, etc. in addition to
	// the blur. Leave it here for now though as the %-based dilation is convenient.
	float oneMinusTwoFactor = 1.0f - 2.0f * mDilationFactor;
	scale *= oneMinusTwoFactor;
	bias = mDilationFactor + oneMinusTwoFactor * bias;

	// Clamp scale (but remain centered)
	float3 center = float3(0.5f, 0.5f, 0.5f);
	float3 clampedScale = min(scale, mMaxScale.xyz);
	bias = (clampedScale / scale) * (bias - center) + center;
	scale = clampedScale;

	// Detect empty partitions
	if (scale.x < 0.0f)
	{
		// Set it to a tiny region that shouldn't be overlapped by any geometry
		// NOTE: We could do this a bit more cleanly by just skipping empty partitions
		// entirely (on the CPU...) but this works for now.
		// TODO: Does this interact poorly with blur kernel scaling?
		scale = asfloat(0x7F7FFFFF).xxx;
		bias = scale;
	}
}

//////////////////////////////////////////////////////////////////////////
// partition index should be in range [0, PARTITIONS] inclusive
float LogPartitionFromRange(uint partition, float minZ, float maxZ)
{
	float z = maxZ;   // Exclusive on this end
	if (partition < PARTITIONS)
	{
		float ratio = maxZ / minZ;
		float power = float(partition) * (1.0f / float(PARTITIONS));
		z = minZ * pow(ratio, power);
	}
	return z;
}

//////////////////////////////////////////////////////////////////////////
// Reset bounds to [0, maxFloat]
BoundsUint EmptyBoundsUint()
{
	BoundsUint b;
	b.minCoord = uint(0x7F7FFFFF).xxx;      // Float max
	b.maxCoord = uint(0).xxx;               // NOTE: Can't be negative!!
	return b;
}

//////////////////////////////////////////////////////////////////////////
BoundsFloat BoundsUintToFloat(BoundsUint u)
{
	BoundsFloat f;
	f.minCoord.x = asfloat(u.minCoord.x);
	f.minCoord.y = asfloat(u.minCoord.y);
	f.minCoord.z = asfloat(u.minCoord.z);
	f.maxCoord.x = asfloat(u.maxCoord.x);
	f.maxCoord.y = asfloat(u.maxCoord.y);
	f.maxCoord.z = asfloat(u.maxCoord.z);
	return f;
}

//////////////////////////////////////////////////////////////////////////
BoundsFloat EmptyBoundsFloat()
{
	return BoundsUintToFloat(EmptyBoundsUint());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

groupshared float sMinZ[REDUCE_ZBOUNDS_BLOCK_SIZE];
groupshared float sMaxZ[REDUCE_ZBOUNDS_BLOCK_SIZE];

//////////////////////////////////////////////////////////////////////////
// Clear out all the intervals just in case
[numthreads(PARTITIONS, 1, 1)]
void ClearZBoundsCS(uint groupIndex : SV_GroupIndex)
{
	gPartitionsUint[groupIndex].intervalBegin = 0x7F7FFFFF;       // Float max
	gPartitionsUint[groupIndex].intervalEnd = 0;
}

//////////////////////////////////////////////////////////////////////////
[numthreads(REDUCE_ZBOUNDS_BLOCK_DIM, REDUCE_ZBOUNDS_BLOCK_DIM, 1)]
void ReduceZBoundsFromGBufferCS(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID, uint  groupIndex : SV_GroupIndex)
{
	uint3 gbufferDim;
	gGbuffer[0].GetDimensions(gbufferDim.x, gbufferDim.y, gbufferDim.z);

	// Initialize stack copy of reduction data for this thread
	float minZ = gNearFar.y;
	float maxZ = gNearFar.x;

	// Loop over tile and reduce into local memory
	{
		uint2 tileStart = groupId.xy * mReduceTileDim.xx + groupThreadId.xy;
		for (uint tileY = 0; tileY < mReduceTileDim; tileY += REDUCE_ZBOUNDS_BLOCK_DIM)
		{
			for (uint tileX = 0; tileX < mReduceTileDim; tileX += REDUCE_ZBOUNDS_BLOCK_DIM)
			{
				uint2 globalCoords = tileStart + uint2(tileX, tileY);
				SurfaceData data = ComputeSurfaceDataFromGBufferSample(gGbuffer, globalCoords, 0);
				float3 samplePosView = mul(float4(data.position, 1.0f), gView).xyz;
				float viewSpaceZ = samplePosView.z;
				if (viewSpaceZ >= gNearFar.x && viewSpaceZ < gNearFar.y)
				{
					minZ = min(minZ, viewSpaceZ);
					maxZ = max(maxZ, viewSpaceZ);
				}
			}
		}
	}

	// Copy result to shared memory for reduction
	sMinZ[groupIndex] = minZ;
	sMaxZ[groupIndex] = maxZ;

	GroupMemoryBarrierWithGroupSync();

	// Reduce our local memory data set to one element
	// TODO: Switch to local atomics for last few iterations
	for (uint offset = (REDUCE_ZBOUNDS_BLOCK_SIZE >> 1); offset > 0; offset >>= 1)
	{
		if (groupIndex < offset)
		{
			sMinZ[groupIndex] = min(sMinZ[groupIndex], sMinZ[offset + groupIndex]);
			sMaxZ[groupIndex] = max(sMaxZ[groupIndex], sMaxZ[offset + groupIndex]);
		}
		GroupMemoryBarrierWithGroupSync();
	}

	// Now write out the result from this pass to the partition data
	// We'll fill in the intermediate intervals in a subsequent pass
	if (groupIndex == 0)
	{
		// Just use scatter atomics for now... we can switch to a global reduction if necessary later
		// Note that choosing good tile dimensions to "just" fill the machine should keep this efficient
		InterlockedMin(gPartitionsUint[0].intervalBegin, asuint(sMinZ[0]));
		InterlockedMax(gPartitionsUint[PARTITIONS - 1].intervalEnd, asuint(sMaxZ[0]));
	}
}

//////////////////////////////////////////////////////////////////////////
[numthreads(PARTITIONS, 1, 1)]
void ComputePartitionsFromZBoundsCS(uint groupIndex : SV_GroupIndex)
{
	// Grab min/max Z from previous reduction
	float minZ = gPartitions[0].intervalBegin;
	float maxZ = gPartitions[PARTITIONS - 1].intervalEnd;

	// Work out partition intervals
	// NOTE: Ensure that it still covers the whole range of the framebuffer (expand first and last)
	// This does not affect the solution at all since we derive the bounds based on the samples, not the
	// partition frusta.
	gPartitions[groupIndex].intervalBegin = groupIndex == 0 ?                gNearFar.x : LogPartitionFromRange(groupIndex, minZ, maxZ);
	gPartitions[groupIndex].intervalEnd   = groupIndex == (PARTITIONS - 1) ? gNearFar.y : LogPartitionFromRange(groupIndex + 1, minZ, maxZ);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

groupshared float3 sBoundsMin[REDUCE_BOUNDS_SHARED_MEMORY_ARRAY_SIZE];
groupshared float3 sBoundsMax[REDUCE_BOUNDS_SHARED_MEMORY_ARRAY_SIZE];

//////////////////////////////////////////////////////////////////////////
[numthreads(PARTITIONS, 1, 1)]
void ComputeCustomPartitionsCS(uint groupIndex : SV_GroupIndex)
{
	// Compute scale/bias and update bounds in partition UAV
	float3 scale, bias;
	ComputePartitionDataFromBounds(gPartitionBoundsReadOnly[groupIndex], scale, bias);

	gPartitions[groupIndex].scale = scale;
	gPartitions[groupIndex].bias  = bias;
}

//////////////////////////////////////////////////////////////////////////
[numthreads(PARTITIONS, 1, 1)]
void ClearPartitionBoundsCS(uint  groupIndex : SV_GroupIndex)
{
	gPartitionBoundsUint[groupIndex] = EmptyBoundsUint();
}

//////////////////////////////////////////////////////////////////////////
[numthreads(REDUCE_BOUNDS_BLOCK_X, REDUCE_BOUNDS_BLOCK_Y, 1)]
void ReduceBoundsFromGBufferCS(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID, uint  groupIndex : SV_GroupIndex)
{
	uint3 gbufferDim;
	gGbuffer[0].GetDimensions(gbufferDim.x, gbufferDim.y, gbufferDim.z);

	// Initialize stack copy of partition data for this thread
	BoundsFloat boundsReduce[PARTITIONS];
	{
		[unroll] for (uint partition = 0; partition < PARTITIONS; ++partition)
		{
			boundsReduce[partition] = EmptyBoundsFloat();
		}
	}

	// Loop over tile and reduce into local memory
	float nearZ = gPartitionsReadOnly[0].intervalBegin;
	float farZ  = gPartitionsReadOnly[PARTITIONS - 1].intervalEnd;
	{
		uint2 tileStart = groupId.xy * mReduceTileDim.xx + groupThreadId.xy;
		for (uint tileY = 0; tileY < mReduceTileDim; tileY += REDUCE_BOUNDS_BLOCK_Y)
		{
			for (uint tileX = 0; tileX < mReduceTileDim; tileX += REDUCE_BOUNDS_BLOCK_X)
			{
				// Sample/compute surface data
				uint2 globalCoords = tileStart + uint2(tileX, tileY);
				SurfaceData data     = ComputeSurfaceDataFromGBufferSample(gGbuffer, globalCoords, 0);
				float3 samplePosView = mul(float4(data.position, 1.0f), gView).xyz;
				float viewSpaceZ     = samplePosView.z;
				// Drop samples that fall outside the view frustum (clear color, etc)
				if (viewSpaceZ >= nearZ && viewSpaceZ < farZ)
				{
					uint partition = 0;
					[unroll] for (uint i = 0; i < (PARTITIONS - 1); ++i)
					{
						[flatten] if (viewSpaceZ >= gPartitionsReadOnly[i].intervalEnd)
						{
							++partition;
						}
					}

					// Update relevant partition data for this thread
					// This avoids the need for atomics since we're the only thread accessing this data
					float3 lightTexCoord, dummy;
					ComputeLightTexCoord(data, lightTexCoord, dummy, dummy);
					boundsReduce[partition].minCoord = min(boundsReduce[partition].minCoord, lightTexCoord.xyz);
					boundsReduce[partition].maxCoord = max(boundsReduce[partition].maxCoord, lightTexCoord.xyz);
				}
			}
		}
	}

	// Copy result to shared memory for reduction
	{
		[unroll] for (uint partition = 0; partition < PARTITIONS; ++partition)
		{
			uint index = (groupIndex * PARTITIONS + partition);
			sBoundsMin[index] = boundsReduce[partition].minCoord;
			sBoundsMax[index] = boundsReduce[partition].maxCoord;
		}
	}

	GroupMemoryBarrierWithGroupSync();

	// Now reduce our local memory data set to one element
	for (uint offset = (REDUCE_BOUNDS_SHARED_MEMORY_ARRAY_SIZE >> 1); offset >= PARTITIONS; offset >>= 1)
	{
		for (uint i = groupIndex; i < offset; i += REDUCE_BOUNDS_BLOCK_SIZE)
		{
			sBoundsMin[i] = min(sBoundsMin[i], sBoundsMin[offset + i]);
			sBoundsMax[i] = max(sBoundsMax[i], sBoundsMax[offset + i]);
		}
		GroupMemoryBarrierWithGroupSync();
	}

	// Now write out the result from this pass
	if (groupIndex < PARTITIONS)
	{
		InterlockedMin(gPartitionBoundsUint[groupIndex].minCoord.x, asuint(sBoundsMin[groupIndex].x));
		InterlockedMin(gPartitionBoundsUint[groupIndex].minCoord.y, asuint(sBoundsMin[groupIndex].y));
		InterlockedMin(gPartitionBoundsUint[groupIndex].minCoord.z, asuint(sBoundsMin[groupIndex].z));
		InterlockedMax(gPartitionBoundsUint[groupIndex].maxCoord.x, asuint(sBoundsMax[groupIndex].x));
		InterlockedMax(gPartitionBoundsUint[groupIndex].maxCoord.y, asuint(sBoundsMax[groupIndex].y));
		InterlockedMax(gPartitionBoundsUint[groupIndex].maxCoord.z, asuint(sBoundsMax[groupIndex].z));
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


technique11 ClearZBounds
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ClearZBoundsCS() ) );
	}
}

technique11 ReduceZBoundsFromGBuffer
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ReduceZBoundsFromGBufferCS() ) );
	}
}

technique11 ComputePartitionsFromZBounds
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ComputePartitionsFromZBoundsCS() ) );
	}
}

technique11 ComputeCustomPartitions
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ComputeCustomPartitionsCS() ) );
	}
}

technique11 ClearPartitionBounds
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ClearPartitionBoundsCS() ) );
	}
}

technique11 ReduceBoundsFromGBuffer
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, ReduceBoundsFromGBufferCS() ) );
	}
}