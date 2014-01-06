#pragma once

#include "Types.h"
#include "MeshData.h"
#include "Material.h"

//////////////////////////////////////////////////////////////////////////
struct Mesh
{
	u32		mVertexCount;
	u32		mIndexCount;
	u32		mByteWidth;
	u32		mDataSize;

	void*	mVertexData;
	u32*	mIndexData;

	u32 mSubsetCount;

	std::vector<unique_ptr<Material>> mMaterial;

	MeshData::RJE_InputLayout			mInputLayout;
	MeshData::RJE_PrimitiveTopology		mPrimitiveTopology;
};
