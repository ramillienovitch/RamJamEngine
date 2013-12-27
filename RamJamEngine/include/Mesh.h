#pragma once

#include "Types.h"
#include "MeshData.h"

//////////////////////////////////////////////////////////////////////////
struct Mesh
{
	u32 mVertexBufferCount;
	u32 mIndexBufferCount;

	void*	mVertexBuffer;
	u32*	mIndexBuffer;

	MeshData::RJE_PrimitiveTopology mPrimitiveType;

// 	void Render();
// 	void Initialize();
// 	void LoadFromMemory();
// 	void LoadFromFile();
};
