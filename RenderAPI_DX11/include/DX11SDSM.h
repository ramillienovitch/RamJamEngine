#pragma once

#include "DX11Helper.h"
#include "..\..\RamJamEngine\include\ShaderDefines.h"

RJE_ALIGNOF(16)
struct SDSMPartitionsConstants
{
	Vector4 mLightSpaceBorder;
	Vector4 mMaxScale;
	float   mDilationFactor;
	u32     mScatterTileDim;
	u32     mReduceTileDim;
};

RJE_ALIGNOF(16)
struct Partition
{
	float intervalBegin;
	float intervalEnd;
	Vector3 scale;
	Vector3 bias;
};

struct BoundsFloat
{
	Vector3 minCoord;
	Vector3 maxCoord;
};

//////////////////////////////////////////////////////////////////////////
struct DX11SDSM
{
	SDSMPartitionsConstants mCurrentConstants;

	StructuredBuffer<Partition>*   mPartitionBuffer;
	StructuredBuffer<BoundsFloat>* mPartitionBounds;
	//---------------
	ID3D11ShaderResourceView* ComputePartitionsFromGBuffer(const Vector3& lightSpaceBorder, const Vector3& maxScale);
	void UpdateShaderConstants(/*SDSMEffect* fx*/);
	void ReducePartitionBounds();
	//---------------
	DX11SDSM();
};
