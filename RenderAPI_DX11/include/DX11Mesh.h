#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Mesh : Mesh
{
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	//--------
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	//--------
	u32 mSubsetCount;
	//-----------------------------
	void Render();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshData::RJE_InputLayout inputLayout);
	void Destroy();
	//--------
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void LoadFromMemory();
	void LoadFromFile(std::string filePath);
};
