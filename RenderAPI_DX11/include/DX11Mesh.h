#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Mesh : Mesh
{
	static ID3D11Device*		sDevice;
	static ID3D11DeviceContext*	sDeviceContext;
	//--------
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	//--------
	DX11Mesh();
	//--------
	static void SetDevice(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	//--------
	void Render(u32 subset);
	void Destroy();
	//--------
	void LoadMaterialFromFile(Material& material, std::string materialFile);
	//--------

	void LoadBox(float width, float height, float depth);
	void LoadSphere(float radius, u32 sliceCount, u32 stackCount);
	void LoadGeoSphere(float radius, u32 numSubdivisions);
	void LoadCylinder(float bottomRadius, float topRadius, float height, u32 sliceCount, u32 stackCount);
	void LoadGrid(float width, float depth, u32 rows, u32 columns);
	void LoadModelFromFile(std::string filePath);
	
	// Gizmos (can load several gizmos for one mesh using subsets)
	void LoadWireBox(float width, float height, float depth, Color color = Color::White);
	void LoadWireSphere(float radius, Color color = Color::White);
	void LoadWireCone(float length, float angle, Color color = Color::White);
	void LoadWireFrustum(Vector3 right, Vector3 up, Vector3 forward, float fovX, float ratio, float nearPlaneDepth, float farPlaneDepth, Color color = Color::White);
	void LoadAxisArrows(Vector3 right, Vector3 up, Vector3 forward);
	void LoadLine(Vector3 start, Vector3 end, Color color = Color::White);
	void LoadRay(Vector3 start, Vector3 orientation, Color color = Color::White);

	//-----------------------------
private:
	static DX11Mesh* sInstance;
	//--------
	void CreateVertexBuffer(void* vertexData);
	void CreateIndexBuffer(u32* indexData);
	//--------
	void LoadPrimitive(MeshData::Data<MeshData::ColorVertex>& meshData);
	void LoadPrimitive(MeshData::Data<MeshData::PosNormTanTex>& meshData);
};
