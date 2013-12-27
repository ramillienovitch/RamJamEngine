#include "DX11Mesh.h"

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::Initialize(ID3D11Device* device, MeshData::RJE_InputLayout inputLayout)
{
	mDevice      = device;
	mInputLayout = inputLayout;
	//-------
	mVertexBuffer = nullptr;
	mIndexBuffer  = nullptr;
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::Destroy()
{
	delete mVertexData;
	delete mIndexData;
	//-------
	RJE_SAFE_RELEASE(mVertexBuffer);
	RJE_SAFE_RELEASE(mIndexBuffer);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage          = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth      = mByteWidth;
	vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = mVertexData;
	RJE_CHECK_FOR_SUCCESS(mDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuffer));
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage          = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth      = sizeof(u32) * mIndexCount;
	ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mIndexData[0];
	RJE_CHECK_FOR_SUCCESS(mDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadFromFile(std::string filePath)
{
	FILE* fIn;
	fopen_s(&fIn, filePath.c_str(), "rb");

	if(!fIn)
	{
		RJE_MESSAGE_BOX(0, L"model file not found.", 0, 0);
		return;
	}

	u32 modelTriangleCount = 0;
	fread(&mVertexCount,       sizeof(u32), 1, fIn);
	fread(&modelTriangleCount, sizeof(u32), 1, fIn);
	mIndexCount = 3*modelTriangleCount;

	switch (mInputLayout)
	{
	case MeshData::RJE_IL_PosNormalTex:		mByteWidth = (u32) (sizeof(MeshData::PosNormalTex)  * mVertexCount);	break;
	case MeshData::RJE_IL_PosNormTanTex:	mByteWidth = (u32) (sizeof(MeshData::PosNormTanTex) * mVertexCount);	break;
	case MeshData::RJE_IL_PosColor:			mByteWidth = (u32) (sizeof(MeshData::ColorVertex)   * mVertexCount);	break;
	default:	break;
	}

	mVertexData = rje_new char[mByteWidth];
	mIndexData  = rje_new u32[mIndexCount];

	float dummy = 0;
	for(u64 i = 0; i < mVertexCount*8; ++i)		// 3float => Position + 3float => Normal + 2float => Tex
	{
		fread(&dummy, 4, 1, fIn);	memcpy((float*)mVertexData+i, &dummy, 4);
	}
	for(u32 i = 0; i < modelTriangleCount; ++i)
	{
		fread(&mIndexData[i*3+0], sizeof(u32), 1, fIn);
		fread(&mIndexData[i*3+1], sizeof(u32), 1, fIn);
		fread(&mIndexData[i*3+2], sizeof(u32), 1, fIn);
	}

	fclose(fIn);

	//-----------------

	CreateVertexBuffer();
	CreateIndexBuffer();
}
