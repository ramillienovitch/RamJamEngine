#pragma once

#include "Types.h"
#include "MeshData.h"
#include "Material.h"

//////////////////////////////////////////////////////////////////////////
struct Mesh
{
	u32		mVertexTotalCount;
	u32		mIndexTotalCount;
	u32		mByteWidth;
	u32		mDataSize;

	void*	mVertexData;
	u32*	mIndexData;

	struct Subset
	{
		u32 mVertexStart;
		u32 mVertexCount;
		//--------------
		u32 mIndexStart;
		u32 mIndexCount;
		//--------------
		Vector3 mCenter;
		Vector3 mExtents;
		float   mRadius;
		BOOL    mbIsInFrustum;
	};

	Subset*	mSubsets;
	u32		mSubsetCount;

	std::vector<unique_ptr<Material>> mMaterial;

	MeshData::RJE_InputLayout			mInputLayout;
	MeshData::RJE_PrimitiveTopology		mPrimitiveTopology;
};
