#pragma once

#include "Types.h"
#include "MeshData.h"

//////////////////////////////////////////////////////////////////////////
struct Mesh
{
	u32		mVertexCount;
	u32		mIndexCount;
	u32		mByteWidth;

	void*	mVertexData;
	u32*	mIndexData;

	MeshData::RJE_InputLayout			mInputLayout;
	MeshData::RJE_PrimitiveTopology		mPrimitiveTopology;
};
