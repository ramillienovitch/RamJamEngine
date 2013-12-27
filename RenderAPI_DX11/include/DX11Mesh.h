#pragma once

#include "DX11Helper.h"
#include "..\..\RamJamEngine\include\Mesh.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Mesh : Mesh
{
	ID3D11Device* mDevice;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	//--------
	void Render();
	void Initialize(ID3D11Device* device, MeshData::RJE_InputLayout inputLayout);
	void Destroy();
	//--------
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void LoadFromMemory();
	void LoadFromFile(std::string filePath);
};
