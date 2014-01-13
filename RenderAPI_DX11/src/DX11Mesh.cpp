#include "DX11Mesh.h"
#include "..\..\RamJamEngine\include\GeometryGenerator.h"
#include "..\..\RamJamEngine\include\System.h"

//////////////////////////////////////////////////////////////////////////
DX11Mesh*				DX11Mesh::sInstance      = nullptr;
ID3D11Device*			DX11Mesh::sDevice        = nullptr;
ID3D11DeviceContext*	DX11Mesh::sDeviceContext = nullptr;

//////////////////////////////////////////////////////////////////////////
DX11Mesh::DX11Mesh()
{
	mVertexBuffer = nullptr;
	mIndexBuffer  = nullptr;
	//--------
	mSubsets = nullptr;
	mSubsetCount = 1;
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::SetDevice(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	sDevice        = device;
	sDeviceContext = deviceContext;
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::Destroy()
{
	RJE_SAFE_DELETE(mVertexData);
	RJE_SAFE_DELETE(mIndexData);
	//-------
	RJE_SAFE_DELETE_PTR(mSubsets);
	//-------
	RJE_SAFE_RELEASE(mVertexBuffer);
	RJE_SAFE_RELEASE(mIndexBuffer);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::CreateVertexBuffer(void* vertexData)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage          = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth      = mByteWidth;
	vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertexData;
	RJE_CHECK_FOR_SUCCESS(sDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuffer));
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::CreateIndexBuffer(u32* indexData)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage          = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth      = sizeof(u32) * mIndexTotalCount;
	ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indexData[0];
	RJE_CHECK_FOR_SUCCESS(sDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::Render(u32 subset)
{
	u32 stride = mDataSize;
	u32 offset = 0;

	// This must be done before drawing every object concerned and NOT for every object
// 	switch (mInputLayout)
// 	{
// 	case MeshData::RJE_IL_PosNormalTex:		sDeviceContext->IASetInputLayout(DX11InputLayouts::PosNormalTex);		break;
// 	case MeshData::RJE_IL_PosNormTanTex:	sDeviceContext->IASetInputLayout(DX11InputLayouts::PosNormalTanTex);	break;
// 	case MeshData::RJE_IL_PosColor:			sDeviceContext->IASetInputLayout(DX11InputLayouts::PosColor);			break;
// 	default:	break;
// 	}
	
	if(subset==-1)
	{
		sDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		sDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		sDeviceContext->DrawIndexed(mIndexTotalCount, 0, 0);
	}
	else
	{
		sDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		sDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		sDeviceContext->DrawIndexed(mSubsets[subset].mIndexCount, mSubsets[subset].mIndexStart, mSubsets[subset].mVertexStart);
	}
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadMaterialFromFile(std::string materialFile )
{
	unique_ptr<Material> material (new Material);

	// first we set all the properties from the material file
	material->LoadPropertiesFromFile(materialFile);

	// then we load the textures if there is any
	for ( auto it = material->mProperties.begin(); it != material->mProperties.end(); ++it )
	{
		if ((*it)->mType == MaterialPropertyType::Type_Texture)
		{
			std::string texturePathRel = CIniFile::GetValue((*it)->mName, "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
			std::string texturePathAbs = System::Instance()->mDataPath + texturePathRel;
			int slash = (int)texturePathRel.rfind('\\')+1;
			int point = (int)texturePathRel.find('.');
			std::string textureName = texturePathRel.substr(slash, point-slash);

			if (textureName == "NONE")
				material->SetTexture((*it)->mName, DX11TextureManager::Instance()->mTextures["_default"]);
			else
			{
				if (!DX11TextureManager::Instance()->IsTextureLoaded(textureName))
					DX11TextureManager::Instance()->LoadTexture(texturePathAbs, textureName);

				// we load the texture properties
				Vector2 tiling = CIniFile::GetValueVector2("Tiling", "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
				Vector2 offset = CIniFile::GetValueVector2("Offset", "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
				float rotation = CIniFile::GetValueFloat("Rotation", "textures", System::Instance()->mDataPath + "materials\\" + materialFile);

				material->SetTexture((*it)->mName, DX11TextureManager::Instance()->mTextures[textureName], tiling, offset, rotation);
			}
		}
	}

	mMaterial.push_back(std::move(material));
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadMaterialLibraryFromFile(std::string materialLibraryFile)
{
	string material;
	int tokenPos     = (int)materialLibraryFile.rfind("\\");
	string matFolder = materialLibraryFile.substr(0, tokenPos);
	ifstream matLibFile (System::Instance()->mDataPath + "materials\\" + materialLibraryFile);
	if (matLibFile.is_open())
	{
		while ( getline (matLibFile,material) )
		{
			CheckMaterialFile(matFolder + "\\" + material);
			LoadMaterialFromFile(matFolder + "\\" + material);
		}
		matLibFile.close();
	}
	else
	{
		RJE_MESSAGE_BOX(0, L"material library file not found.", 0, 0);
		return;
	}
	matLibFile.close();
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::CheckMaterialFile(string materialFile)
{
	ifstream matFile (System::Instance()->mDataPath + "materials\\" + materialFile);
	if (!matFile.is_open())
	{
		std::ofstream out (System::Instance()->mDataPath + "materials\\" + materialFile);
		out << "[shader]\n";
		out << "Name=basic\n";
		out << "# ----------------------\n";
		out << "[transparency]\n";
		out << "IsOpaque=true\n";
		out << "# ----------------------\n";
		out << "[properties]\n";
		out << "Ambient=1.0|1.0|1.0|1.0\n";
		out << "Diffuse=0.8|0.8|0.8|1.0\n";
		out << "Specular=0.2|0.2|0.2|16.0\n";
		out << "# ----------------------\n";
		out << "[textures]\n";
		out << "Texture_Diffuse=NONE\n";
		out << "Tiling=1.0|1.0\n";
		out << "Offset=0.0|0.0\n";
		out << "Rotation=0.0\n";
		out << "# ----------------------";
		out.close();
	}

}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadModelFromFile(std::string filePath)
{
	FILE* fIn;
	fopen_s(&fIn, filePath.c_str(), "rb");

	if(!fIn)
	{
		RJE_MESSAGE_BOX(0, L"model file not found.", 0, 0);
		return;
	}
	u32 modelTriangleCount = 0;
	fread(&mSubsetCount, sizeof(u32), 1, fIn);
	mSubsets = rje_new Subset[mSubsetCount];
	for (u32 iMesh=0 ; iMesh<mSubsetCount ; ++iMesh)
	{
		fread(&mSubsets[iMesh].mVertexStart, sizeof(u32), 1, fIn);
		fread(&mSubsets[iMesh].mIndexStart,  sizeof(u32), 1, fIn);
		fread(&mSubsets[iMesh].mVertexCount, sizeof(u32), 1, fIn);
		fread(&mSubsets[iMesh].mIndexCount,  sizeof(u32), 1, fIn);
		// multiply by 3 because a triangle has 3 indexes
		mSubsets[iMesh].mIndexStart *= 3;
		mSubsets[iMesh].mIndexCount *= 3;
	}
	fread(&mVertexTotalCount,  sizeof(u32), 1, fIn);
	fread(&modelTriangleCount, sizeof(u32), 1, fIn);
	mIndexTotalCount  = 3*modelTriangleCount;
	mInputLayout = MeshData::RJE_InputLayout::RJE_IL_PosNormTanTex;

	// 	switch (mInputLayout)
	// 	{
	// 	case MeshData::RJE_IL_PosNormalTex:		mDataSize = (u32) sizeof(MeshData::PosNormalTex);		mByteWidth = mDataSize * mVertexTotalCount;	break;
	// 	case MeshData::RJE_IL_PosNormTanTex:	mDataSize = (u32) sizeof(MeshData::PosNormTanTex);		mByteWidth = mDataSize * mVertexTotalCount;	break;
	// 	case MeshData::RJE_IL_PosColor:			mDataSize = (u32) sizeof(MeshData::ColorVertex);		mByteWidth = mDataSize * mVertexTotalCount;	break;
	// 	default:	break;
	// 	}

	mDataSize   = (u32) sizeof(MeshData::PosNormTanTex);
	mByteWidth  = mDataSize * mVertexTotalCount;
	mVertexData = rje_new char[mByteWidth];
	mIndexData  = rje_new u32[mIndexTotalCount];

	float dummy = 0;
	for(u64 i = 0; i < mVertexTotalCount*11; ++i)		// 3float => Position + 3float => Normal + 3float Tangent + 2float => Tex
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

	CreateVertexBuffer(mVertexData);
	CreateIndexBuffer(mIndexData);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadBox(float width, float height, float depth)
{
	MeshData::Data<PosNormTanTex> box;
	GeometryGenerator geoGen;

	geoGen.CreateBox(width, height, depth, box);
	LoadPrimitive(box);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadSphere(float radius, u32 sliceCount, u32 stackCount)
{
	MeshData::Data<PosNormTanTex> sphere;
	GeometryGenerator geoGen;

	geoGen.CreateSphere(radius, sliceCount, stackCount, sphere);
	LoadPrimitive(sphere);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadGeoSphere(float radius, u32 numSubdivisions)
{
	MeshData::Data<PosNormTanTex> sphere;
	GeometryGenerator geoGen;

	geoGen.CreateGeosphere(radius, numSubdivisions, sphere);
	LoadPrimitive(sphere);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadCylinder(float bottomRadius, float topRadius, float height, u32 sliceCount, u32 stackCount)
{
	MeshData::Data<PosNormTanTex> cylinder;
	GeometryGenerator geoGen;

	geoGen.CreateCylinder(bottomRadius, topRadius, height, sliceCount, stackCount, cylinder);
	LoadPrimitive(cylinder);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadGrid(float width, float depth, u32 rows, u32 columns)
{
	MeshData::Data<PosNormTanTex> grid;
	GeometryGenerator geoGen;

	geoGen.CreateGrid(width, depth, rows, columns, grid);
	LoadPrimitive(grid);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadWireBox( float width, float height, float depth, Color color )
{
	MeshData::Data<ColorVertex> box;
	GeometryGenerator geoGen;

	geoGen.CreateWireBox(width, height, depth, box, color);
	LoadPrimitive(box);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadWireSphere( float radius, Color color )
{
	MeshData::Data<ColorVertex> sphere;
	GeometryGenerator geoGen;

	geoGen.CreateWireSphere(radius, sphere, color);
	LoadPrimitive(sphere);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadWireCone( float length, float angle, Color color )
{
	MeshData::Data<ColorVertex> cone;
	GeometryGenerator geoGen;

	geoGen.CreateWireCone(length, angle, cone, color);
	LoadPrimitive(cone);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadWireFrustum( Vector3 right, Vector3 up, Vector3 forward, float fovX, float ratio, float nearPlaneDepth, float farPlaneDepth, Color color )
{
	MeshData::Data<ColorVertex> frustum;
	GeometryGenerator geoGen;

	geoGen.CreateWireFrustum(right, up, forward, fovX, ratio, nearPlaneDepth, farPlaneDepth, frustum, color);
	LoadPrimitive(frustum);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadAxisArrows( Vector3 right, Vector3 up, Vector3 forward)
{
	MeshData::Data<ColorVertex> axis;
	GeometryGenerator geoGen;

	geoGen.CreateAxisArrows(right, up, forward, axis);
	LoadPrimitive(axis);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadLine( Vector3 start, Vector3 end, Color color )
{
	MeshData::Data<ColorVertex> line;
	GeometryGenerator geoGen;

	geoGen.CreateLine(start, end, line, color);
	LoadPrimitive(line);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadRay( Vector3 start, Vector3 orientation, Color color )
{
	MeshData::Data<ColorVertex> ray;
	GeometryGenerator geoGen;

	geoGen.CreateRay(start, orientation, ray, color);
	LoadPrimitive(ray);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadPrimitive(MeshData::Data<ColorVertex>& meshData)
{
	mInputLayout = MeshData::RJE_InputLayout::RJE_IL_PosColor;
	mVertexTotalCount = (u32) meshData.Vertices.size();
	mIndexTotalCount  = (u32) meshData.Indices.size();
	mDataSize    = (u32) sizeof(MeshData::ColorVertex);
	mByteWidth   = mDataSize * mVertexTotalCount;

	mVertexData = rje_new char[mByteWidth];
	mIndexData  = rje_new u32[mIndexTotalCount];

	for (u32 i=0; i<mVertexTotalCount; ++i)
	{
		memcpy((float*)mVertexData+4*i+0, &meshData.Vertices[i].pos.x, 4);
		memcpy((float*)mVertexData+4*i+1, &meshData.Vertices[i].pos.y, 4);
		memcpy((float*)mVertexData+4*i+2, &meshData.Vertices[i].pos.z, 4);
		memcpy((float*)mVertexData+4*i+3, &meshData.Vertices[i].color, 4);
	}
	for (u32 j=0; j<mIndexTotalCount; ++j)
	{
		memcpy(&mIndexData[j], &meshData.Indices[j], 4);
	}
	CreateVertexBuffer(mVertexData);
	CreateIndexBuffer(mIndexData);
}

//////////////////////////////////////////////////////////////////////////
void DX11Mesh::LoadPrimitive(MeshData::Data<PosNormTanTex>& meshData)
{
	mInputLayout = MeshData::RJE_InputLayout::RJE_IL_PosNormTanTex;
	mVertexTotalCount = (u32) meshData.Vertices.size();
	mIndexTotalCount  = (u32) meshData.Indices.size();
	mDataSize    = (u32) sizeof(MeshData::PosNormTanTex);
	mByteWidth   = mDataSize * mVertexTotalCount;

	mVertexData = rje_new char[mByteWidth];
	mIndexData  = rje_new u32[mIndexTotalCount];

	for (u32 i=0; i<mVertexTotalCount; ++i)
	{
		memcpy((float*)mVertexData+11*i+0,  &meshData.Vertices[i].Position.x, 4);
		memcpy((float*)mVertexData+11*i+1,  &meshData.Vertices[i].Position.y, 4);
		memcpy((float*)mVertexData+11*i+2,  &meshData.Vertices[i].Position.z, 4);
		memcpy((float*)mVertexData+11*i+3,  &meshData.Vertices[i].Normal.x,   4);
		memcpy((float*)mVertexData+11*i+4,  &meshData.Vertices[i].Normal.y,   4);
		memcpy((float*)mVertexData+11*i+5,  &meshData.Vertices[i].Normal.z,   4);
		memcpy((float*)mVertexData+11*i+6,  &meshData.Vertices[i].TangentU.x, 4);
		memcpy((float*)mVertexData+11*i+7,  &meshData.Vertices[i].TangentU.y, 4);
		memcpy((float*)mVertexData+11*i+8,  &meshData.Vertices[i].TangentU.z, 4);
		memcpy((float*)mVertexData+11*i+9,  &meshData.Vertices[i].TexC.x,     4);
		memcpy((float*)mVertexData+11*i+10, &meshData.Vertices[i].TexC.y,     4);
	}
	for (u32 j=0; j<mIndexTotalCount; ++j)
	{
		memcpy(&mIndexData[j], &meshData.Indices[j], 4);
	}
	CreateVertexBuffer(mVertexData);
	CreateIndexBuffer(mIndexData);
}
