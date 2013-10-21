#pragma comment(lib, "assimp.lib")
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

#include <windows.h>
#include <iostream>
#include <fstream>

#define EXPORT_BINARY	1
#define EXPORT_TEXT		0

using namespace std;


namespace GLOBALS
{
	static bool g_computeNormals	= false;
	static bool g_hasTextCoord		= false;
}

//////////////////////////////////////////////////////////////////////////
// ASS IMP
//////////////////////////////////////////////////////////////////////////
bool ImportExportAssImp( const char* );
void ExportToFile( const aiScene* );
void LogLoadError(const char*, const char* );

//////////////////////////////////////////////////////////////////////////
// OBJ TO RJE
//////////////////////////////////////////////////////////////////////////
void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);
bool OBJtoRJE(char*);

typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;


//////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////
void main ()
{
	char filename[256];

	// We first get the filename
	GetModelFilename(filename);

	// Then we convert it using AssImp or our custom tool
	ImportExportAssImp(filename);
	//OBJtoRJE(filename);
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
	const aiScene* scene = importer.ReadFile( pFile, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenSmoothNormals | aiProcessPreset_TargetRealtime_MaxQuality );//| aiProcess_ConvertToLeftHanded );
	
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
	int vertexCount	= scene->mMeshes[0]->mNumVertices;
	int indexCount	= scene->mMeshes[0]->mNumFaces;

#if EXPORT_TEXT
	std::ofstream fOut;
	
	fOut.open("exported_model.mesh");

	fOut << "Vertex Count: " << scene->mMeshes[0]->mNumVertices << "\n";
	fOut << "Face Count: " << scene->mMeshes[0]->mNumFaces << "\n";
	fOut << "\nData:\n\n";

	if (GLOBALS::g_hasTextCoord)
	{
		for (int iVert=0 ; iVert<vertexCount ; ++iVert)
		{
			fOut << scene->mMeshes[0]->mVertices[iVert].x << " ";
			fOut << scene->mMeshes[0]->mVertices[iVert].y << " ";
			fOut << scene->mMeshes[0]->mVertices[iVert].z << " ";

			fOut << scene->mMeshes[0]->mTextureCoords[0]->x << " ";
			fOut << scene->mMeshes[0]->mTextureCoords[0]->y << " ";

			fOut << scene->mMeshes[0]->mNormals[iVert].x << " ";
			fOut << scene->mMeshes[0]->mNormals[iVert].y << " ";
			fOut << scene->mMeshes[0]->mNormals[iVert].z << "\n";
		}
		for (int iIdx=0 ; iIdx<indexCount ; ++iIdx)
		{
			fOut << scene->mMeshes[0]->mFaces[iIdx].mIndices[0] << " ";
			fOut << scene->mMeshes[0]->mFaces[iIdx].mIndices[1] << " ";
			fOut << scene->mMeshes[0]->mFaces[iIdx].mIndices[2] << "\n";
		}
	}
	else
	{
		for (int iVert=0 ; iVert<vertexCount ; ++iVert)
		{
			fOut << scene->mMeshes[0]->mVertices[iVert].x << " ";
			fOut << scene->mMeshes[0]->mVertices[iVert].y << " ";
			fOut << scene->mMeshes[0]->mVertices[iVert].z << " ";

			fOut << "0.0 0.0 ";

			fOut << scene->mMeshes[0]->mNormals[iVert].x << " ";
			fOut << scene->mMeshes[0]->mNormals[iVert].y << " ";
			fOut << scene->mMeshes[0]->mNormals[iVert].z << "\n";
		}
		for (int iFaces=0 ; iFaces<indexCount ; ++iFaces)
		{
			fOut << scene->mMeshes[0]->mFaces[iFaces].mIndices[0] << " ";
			fOut << scene->mMeshes[0]->mFaces[iFaces].mIndices[1] << " ";
			fOut << scene->mMeshes[0]->mFaces[iFaces].mIndices[2] << "\n";
		}
	}
	fOut.close();

#elif EXPORT_BINARY
	FILE* fOut;

	fOut = fopen("exported_model.rjemesh", "wb");
	if(fOut == NULL)
	{
		puts("Cannot open file exported_model.rjemesh");
	}

	std::fwrite(&scene->mMeshes[0]->mNumVertices, sizeof(unsigned int), 1, fOut);
	std::fwrite(&scene->mMeshes[0]->mNumFaces,    sizeof(unsigned int), 1, fOut);
	if (GLOBALS::g_hasTextCoord)
	{
		for (int iVert=0 ; iVert<vertexCount ; ++iVert)
		{
			std::fwrite(&scene->mMeshes[0]->mVertices[iVert].x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mVertices[iVert].y, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mVertices[iVert].z, sizeof(float), 1, fOut);

			std::fwrite(&scene->mMeshes[0]->mTextureCoords[0]->x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mTextureCoords[0]->y, sizeof(float), 1, fOut);

			std::fwrite(&scene->mMeshes[0]->mNormals[iVert].x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mNormals[iVert].y, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mNormals[iVert].z, sizeof(float), 1, fOut);
		}
		for (int iIdx=0 ; iIdx<indexCount ; ++iIdx)
		{
			std::fwrite(&scene->mMeshes[0]->mFaces[iIdx].mIndices[0], sizeof(unsigned int), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mFaces[iIdx].mIndices[1], sizeof(unsigned int), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mFaces[iIdx].mIndices[2], sizeof(unsigned int), 1, fOut);
		}
	}
	else
	{
		float zero = 0.0f;
		for (int iVert=0 ; iVert<vertexCount ; ++iVert)
		{
			std::fwrite(&scene->mMeshes[0]->mVertices[iVert].x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mVertices[iVert].y, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mVertices[iVert].z, sizeof(float), 1, fOut);

			std::fwrite(&zero, sizeof(float), 1, fOut);
			std::fwrite(&zero, sizeof(float), 1, fOut);

			std::fwrite(&scene->mMeshes[0]->mNormals[iVert].x, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mNormals[iVert].y, sizeof(float), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mNormals[iVert].z, sizeof(float), 1, fOut);
		}
		for (int iIdx=0 ; iIdx<indexCount ; ++iIdx)
		{
			std::fwrite(&scene->mMeshes[0]->mFaces[iIdx].mIndices[0], sizeof(unsigned int), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mFaces[iIdx].mIndices[1], sizeof(unsigned int), 1, fOut);
			std::fwrite(&scene->mMeshes[0]->mFaces[iIdx].mIndices[2], sizeof(unsigned int), 1, fOut);
		}
	}
	fclose(fOut);
#endif
	return;
}

//////////////////////////////////////////////////////////////////////////
bool OBJtoRJE( char* filename )
{
	bool result;

	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;

	// Read in the number of vertices, texture coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if(!result)
		return false;

	// Display the counts to the screen for information purposes.
	std::cout << std::endl;
	std::cout << "Vertices: " << vertexCount << std::endl;
	std::cout << "UVs:      " << textureCount << std::endl;
	std::cout << "Normals:  " << normalCount << std::endl;
	std::cout << "Faces:    " << faceCount << std::endl;

	// Now read the data from the file into the data structures and then output it in our model format.
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if(!result)
		return false;

	// Notify the user the model has been converted.
	std::cout << "\nFile has been converted." << std::endl;
	std::cin >> garbage;

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	std::ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while(!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if(input == 'v')
		{
			fin.get(input);
			if(input == ' ') { vertexCount++; }
			if(input == 't') { textureCount++; }
			if(input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if(input == 'f')
		{
			fin.get(input);
			if(input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	std::ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	std::ofstream fout;


	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if(!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if(!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if(!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if(!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while(!fin.eof())
	{
		if(input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if(input == ' ') 
			{ 
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++; 
			}

			// Read in the texture uv coordinates.
			if(input == 't') 
			{ 
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++; 
			}

			// Read in the normals.
			if(input == 'n') 
			{ 
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++; 
			}
		}

		// Read in the faces.
		if(input == 'f') 
		{
			fin.get(input);
			if(input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	// Open the output file.
	fout.open("model.txt");

	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for(int i=0; i<faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();

	// Release the four data structures.
	if(vertices)
	{
		delete [] vertices;
		vertices = 0;
	}
	if(texcoords)
	{
		delete [] texcoords;
		texcoords = 0;
	}
	if(normals)
	{
		delete [] normals;
		normals = 0;
	}
	if(faces)
	{
		delete [] faces;
		faces = 0;
	}

	return true;
}