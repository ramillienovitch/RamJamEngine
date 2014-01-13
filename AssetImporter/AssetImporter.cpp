// AssetImporter.cpp : Defines the entry point for the console application.
//

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

#include <windows.h>
#include <iostream>
#include <fstream>
#include <direct.h>

#define EXPORT_BINARY	1
#define EXPORT_TEXT		0

using namespace std;

typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

namespace GLOBALS
{
	static bool g_computeNormals	= false;
}

//////////////////////////////////////////////////////////////////////////
// ASS IMP
//////////////////////////////////////////////////////////////////////////
bool ImportExportAssImp( const char* );
void ExportToFile( const aiScene* );
void LogLoadError(const char*, const char* );
void GetModelFilename(char*);

//////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	char filename[256];

	// We first get the filename
	GetModelFilename(filename);

	// Then we convert it using AssImp or our custom tool
	ImportExportAssImp(filename);
}

//////////////////////////////////////////////////////////////////////////
void GetModelFilename(char* filename)
{
	bool done;
	std::ifstream fin;

	// Loop until we have a file name.
	done = false;
	while(!done)
	{
		// Ask the user for the filename.
		std::cout << "Enter model filename: ";

		// Read in the filename.
		std::cin >> filename;

		// Attempt to open the file.
		fin.open(filename);

		if(fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			std::cout << std::endl;
			std::cout << "File " << filename << " could not be opened." << std::endl << std::endl;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
bool ImportExportAssImp( const char* pFile )
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example post processing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more post processing than we do in this example.
	const aiScene* scene = importer.ReadFile( pFile,	aiProcess_CalcTangentSpace		|
														aiProcess_Triangulate			|
														aiProcess_JoinIdenticalVertices |
														aiProcess_SortByPType			|
														aiProcess_GenSmoothNormals		|
														aiProcess_FlipUVs				|
														aiProcessPreset_TargetRealtime_MaxQuality);//aiProcess_FlipWindingOrder);//| aiProcess_ConvertToLeftHanded );

	// If the import failed, report it
	if( !scene )
	{
		LogLoadError( pFile, importer.GetErrorString() );
		return false;
	}
	// Now we can access the file's contents.
	ExportToFile( scene );

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

//////////////////////////////////////////////////////////////////////////
void LogLoadError(const char* modelFileName, const char* errorString )
{
	std::ofstream fOut;

	fOut.open("AssImpLoader-error.txt");

	// Write out the error message.
	for(size_t i=0; i<strlen(errorString); ++i)
		fOut << errorString[i];

	fOut.close();

	MessageBox(NULL, L"Error while importing model.  AssImporter-error.txt for details.", (LPCWSTR)modelFileName, MB_OK);
	return;
}

//////////////////////////////////////////////////////////////////////////
void ExportToFile( const aiScene* scene )
{
	//===================================
	// Export Materials (this is greedy !! creates as many materials as needed - one per mesh)
	_mkdir("EXPORT");
	_mkdir("EXPORT\\Materials");
	// delete file extension
	aiString fileName = scene->mRootNode->mName;
	u64 newSize = 0;
	while(fileName.data[newSize] != '.')
	{
		++newSize;
	}
	fileName.length        = newSize;
	fileName.data[newSize] = 0;
	//------
	char buffer[100];
	aiString materialName;
	aiString materialLibName;
	materialName.Append("EXPORT\\Materials\\");
	materialName.Append(fileName.C_Str());
	materialLibName = materialName;
	materialLibName.Append(".matlib");
	std::ofstream o(materialLibName.C_Str());
	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		aiString mat = fileName;
		_itoa_s(iMesh, buffer, 10);
		mat.Append("_");
		mat.Append(buffer);
		mat.Append(".mat\n");
		o << mat.C_Str();
	}
	o.close();
	//===================================

#if EXPORT_TEXT
	std::ofstream fOut;

	fOut.open("EXPORT\\exported_model.txt");

	u32 dummy         = scene->mNumMeshes;
	u32 totalVertices = 0;
	u32 totalFaces    = 0;
	fOut << "Mesh Count: " << dummy << "\n";
	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		dummy = totalVertices;									fOut << "Mesh " << iMesh << " Vertex Start :"	<< dummy << "\n";
		dummy = totalFaces;										fOut << "Mesh " << iMesh << " Face Start :"		<< dummy << "\n";
		dummy = scene->mMeshes[iMesh]->mNumVertices;			fOut << "Mesh " << iMesh << " Vertex Count :"	<< dummy << "\n";
		dummy = scene->mMeshes[iMesh]->mNumFaces;				fOut << "Mesh " << iMesh << " Face Count :"		<< dummy << "\n";
		totalVertices += scene->mMeshes[iMesh]->mNumVertices;
		totalFaces    += scene->mMeshes[iMesh]->mNumFaces;
	}
	fOut << "Total Vertex Count: " << totalVertices << "\n";
	fOut << "Total Face Count: " << totalFaces << "\n";
	fOut << "\nData:\n\n";

	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		int vertexCount = scene->mMeshes[iMesh]->mNumVertices;
		for (int iVert=0 ; iVert<vertexCount ; ++iVert)
		{
			fOut << scene->mMeshes[iMesh]->mVertices[iVert].x << " ";
			fOut << scene->mMeshes[iMesh]->mVertices[iVert].y << " ";
			fOut << scene->mMeshes[iMesh]->mVertices[iVert].z << " ";

			fOut << scene->mMeshes[iMesh]->mNormals[iVert].x << " ";
			fOut << scene->mMeshes[iMesh]->mNormals[iVert].y << " ";
			fOut << scene->mMeshes[iMesh]->mNormals[iVert].z << " ";

			if (scene->mMeshes[iMesh]->HasTangentsAndBitangents())
			{
				fOut << scene->mMeshes[iMesh]->mTangents[iVert].x << " ";
				fOut << scene->mMeshes[iMesh]->mTangents[iVert].y << " ";
				fOut << scene->mMeshes[iMesh]->mTangents[iVert].z << " ";
			}
			else
			{
				fOut << "0.0 0.0 0.0";
			}

			if (scene->mMeshes[iMesh]->HasTextureCoords(0))
			{
				fOut << scene->mMeshes[iMesh]->mTextureCoords[0][iVert].x << " ";
				fOut << scene->mMeshes[iMesh]->mTextureCoords[0][iVert].y << "\n";
			}
			else
			{
				fOut << "0.0 0.0\n";
			}
		}
	}
	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		int indexCount  = scene->mMeshes[iMesh]->mNumFaces;
		for (int iIdx=0 ; iIdx<indexCount ; ++iIdx)
		{
			fOut << scene->mMeshes[iMesh]->mFaces[iIdx].mIndices[0] << " ";
			fOut << scene->mMeshes[iMesh]->mFaces[iIdx].mIndices[1] << " ";
			fOut << scene->mMeshes[iMesh]->mFaces[iIdx].mIndices[2] << "\n";
		}
	}
	fOut.close();

#elif EXPORT_BINARY
	FILE* fOut;
	errno_t err = fopen_s(&fOut, "EXPORT\\exported_model.mesh", "wb");
	if(err != NULL)
	{
		puts("Cannot open file exported_model.mesh");
	}
	std::cout << "Start Export" << std::endl;

	// --- write subsets, total vertices & faces -----------------
	float zero        = 0.0f;
	u32 dummy         = scene->mNumMeshes;
	u32 totalVertices = 0;
	u32 totalFaces    = 0;
	std::fwrite(&dummy, sizeof(u32), 1, fOut);
	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		dummy = totalVertices;									std::fwrite(&dummy, sizeof(u32), 1, fOut);
		dummy = totalFaces;										std::fwrite(&dummy, sizeof(u32), 1, fOut);
		dummy = scene->mMeshes[iMesh]->mNumVertices;			std::fwrite(&dummy, sizeof(u32), 1, fOut);
		dummy = scene->mMeshes[iMesh]->mNumFaces;				std::fwrite(&dummy, sizeof(u32), 1, fOut);
		totalVertices += scene->mMeshes[iMesh]->mNumVertices;
		totalFaces    += scene->mMeshes[iMesh]->mNumFaces;
	}
	std::fwrite(&totalVertices, sizeof(u32), 1, fOut);
	std::fwrite(&totalFaces,    sizeof(u32), 1, fOut);
	// ----------------------------------------------------
	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		int vertexCount = scene->mMeshes[iMesh]->mNumVertices;
		for (int iVert=0 ; iVert<vertexCount ; ++iVert)
		{
			std::fwrite(&scene->mMeshes[iMesh]->mVertices[iVert].x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[iMesh]->mVertices[iVert].y, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[iMesh]->mVertices[iVert].z, sizeof(float), 1, fOut);

			std::fwrite(&scene->mMeshes[iMesh]->mNormals[iVert].x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[iMesh]->mNormals[iVert].y, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[iMesh]->mNormals[iVert].z, sizeof(float), 1, fOut);

			if (scene->mMeshes[iMesh]->HasTangentsAndBitangents())
			{
				std::fwrite(&scene->mMeshes[iMesh]->mTangents[iVert].x, sizeof(float), 1, fOut);
				std::fwrite(&scene->mMeshes[iMesh]->mTangents[iVert].y, sizeof(float), 1, fOut);
				std::fwrite(&scene->mMeshes[iMesh]->mTangents[iVert].z, sizeof(float), 1, fOut);
			}
			else
			{
				std::fwrite(&zero, sizeof(float), 1, fOut);
				std::fwrite(&zero, sizeof(float), 1, fOut);
				std::fwrite(&zero, sizeof(float), 1, fOut);
			}

			if (scene->mMeshes[iMesh]->HasTextureCoords(0))
			{
				std::fwrite(&scene->mMeshes[iMesh]->mTextureCoords[0][iVert].x, sizeof(float), 1, fOut);
				std::fwrite(&scene->mMeshes[iMesh]->mTextureCoords[0][iVert].y, sizeof(float), 1, fOut);
			}
			else
			{
				std::fwrite(&zero, sizeof(float), 1, fOut);
				std::fwrite(&zero, sizeof(float), 1, fOut);
			}
		}
		
	}
	for(u32 iMesh=0 ; iMesh<scene->mNumMeshes ; ++iMesh)
	{
		int indexCount  = scene->mMeshes[iMesh]->mNumFaces;
		for (int iIdx=0 ; iIdx<indexCount ; ++iIdx)
		{
			std::fwrite(&scene->mMeshes[iMesh]->mFaces[iIdx].mIndices[0], sizeof(u32), 1, fOut);
			std::fwrite(&scene->mMeshes[iMesh]->mFaces[iIdx].mIndices[1], sizeof(u32), 1, fOut);
			std::fwrite(&scene->mMeshes[iMesh]->mFaces[iIdx].mIndices[2], sizeof(u32), 1, fOut);
		}
	}
	fclose(fOut);
#endif
	return;
}
