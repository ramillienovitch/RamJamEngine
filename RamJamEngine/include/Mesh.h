#pragma once

#include "Types.h"
#include "MeshData.h"

//////////////////////////////////////////////////////////////////////////
struct Mesh
{
	u32		mVertexBufferCount;
	u32		mIndexBufferCount;
	u32		mByteWidth;

	void*	mVertexBuffer;
	u32*	mIndexBuffer;

	MeshData::RJE_InputLayout			mInputLayout;
	MeshData::RJE_PrimitiveTopology		mPrimitiveTopology;
};
