//==========================================
// Exponential Variance Shadow Mapping Blur
//==========================================

#include "Rendering.hlsl"

// Utility to work out blur metadata
struct BlurData
{
	uint interiorSamplesLoopCount : interiorSamplesLoopCount;
	float interiorSampleWeight : interiorSampleWeight;
	float edgeSampleWeight : edgeSampleWeight;
};

BlurData ComputeBlurData(float filterSizeTexels)
{
	BlurData output;

	// Just draw the diagram to understand... :)
	float sideSamples = 0.5f * filterSizeTexels - 0.5f;
	output.edgeSampleWeight = modf(sideSamples, output.interiorSamplesLoopCount);

	// Normalize weights
	output.interiorSampleWeight = 1.0f / filterSizeTexels;
	output.edgeSampleWeight *= output.interiorSampleWeight;

	return output;
}

cbuffer BoxBlurConstants
{
	float2 mFilterSize;     // Filter size in light view space [0,1] (unaffected by partitioning)
	uint mPartition;        // Which partition this is (for scaling filter regions)
	uint mDimension;        // Which dimension to blur in (0 = horiz, 1 = vert)
}

Texture2DArray					gInputTexture;
StructuredBuffer<Partition>		gPartitions;

struct BoxBlurVSOut
{
	float4 positionViewport : SV_Position;
	float4 positionClip     : positionClip;

	// These are constant over every vertex (no interpolation necessary)
	nointerpolation BlurData blurData;
};

//////////////////////////////////////////////////////////////////////////
BoxBlurVSOut BoxBlurVS(uint vertexID : SV_VertexID)
{
	BoxBlurVSOut output;

	// Parametrically work out vertex location for full screen triangle
	float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
	output.positionClip = float4(grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	output.positionViewport = output.positionClip;

	// Scale filter size into this partition
	float2 filterSize = mFilterSize * gPartitions[mPartition].scale.xy;

	// Rescale blur kernel into texel space
	float2 dimensions;
	float elements;
	gInputTexture.GetDimensions(dimensions.x, dimensions.y, elements);
	float2 filterSizeTexels = filterSize * float2(dimensions);

	// If neither dimension needs blurring, we can skip both passes
	// To do this, we set up a triangle that gets clipped
	// We could do this by writing a NaN or something for position, but since
	// this section of the shader is actually fully determined by uniforms, it will
	// branch the same way for all three vertices. Thus we can simply set the triangle
	// outside the viewport and let clipping handle it.
	// NOTE: Don't bother putting the rest in the else branch or the compiler complains
	// about potentially uninitialized outputs... whatever.
	if (all(filterSizeTexels <= 1.0f))
	{
		output.positionViewport = float4(-10.0f, -10.0f, -10.0f, 1.0f);
	}

	// Rest of the math need only be for the relevant dimension
	float filterSizeDim = (mDimension == 0 ? filterSizeTexels.x : filterSizeTexels.y);
	filterSizeDim = max(filterSizeDim, 1.0f);
	output.blurData = ComputeBlurData(filterSizeDim);

	return output;
}

//////////////////////////////////////////////////////////////////////////
float4 BoxBlurPS(BoxBlurVSOut input) : SV_Target
{
	uint2 sampleOffsetMask = (uint2(0, 1) == mDimension.xx ? uint2(1, 1) : uint2(0, 0));

	// Center sample
	uint2 coords = int2(input.positionViewport.xy);
	float4 average = input.blurData.interiorSampleWeight * gInputTexture.Load(uint4(coords, 0, 0));

	// Other interior samples
	for (uint i = 1; i <= input.blurData.interiorSamplesLoopCount; ++i)
	{
		uint2 offset = i * sampleOffsetMask;
		// Left sample
		average += input.blurData.interiorSampleWeight * gInputTexture.Load(uint4(coords - offset, 0, 0));
		// Right sample
		average += input.blurData.interiorSampleWeight * gInputTexture.Load(uint4(coords + offset, 0, 0));
	}

	// Edge samples
	uint2 offset = (input.blurData.interiorSamplesLoopCount + 1) * sampleOffsetMask;
	// Left sample
	average += input.blurData.edgeSampleWeight * gInputTexture.Load(uint4(coords - offset, 0, 0));
	// Right sample
	average += input.blurData.edgeSampleWeight * gInputTexture.Load(uint4(coords + offset, 0, 0));

	return average;
}

technique11 EVSMBlur
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, BoxBlurVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, BoxBlurPS() ) );
	}
}
