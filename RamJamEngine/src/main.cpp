
//////////////////////////////////////////////////////////////////////////////////////
//                                                                                  //
//    ______                  ___                 _____            _                //
//    | ___ \                |_  |               |  ___|          (_)               //
//    | |_/ /__ _ _ __ ___     | | __ _ _ __ ___ | |__ _ __   __ _ _ _ __   ___     //
//    |    // _` | '_ ` _ \    | |/ _` | '_ ` _ \|  __| '_ \ / _` | | '_ \ / _ \    //
//    | |\ \ (_| | | | | | /\__/ / (_| | | | | | | |__| | | | (_| | | | | |  __/    //
//    \_| \_\__,_|_| |_| |_\____/ \__,_|_| |_| |_\____/_| |_|\__, |_|_| |_|\___|    //
//                                                            __/ |                 //
//                                                           |___/                  //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// The RamJamEngine is a game/rendering engine for learning purposes only.
// I wanted to learn 3D graphics using OpenGL and DirectX.
// I first began with the Rastertek tutorials (http://www.rastertek.com/)
// but unfortunately, the architecture wasn't robust enough (IMO), so I decided to
// create my own Game/Rendering Engine.
//
// The RJE is based on the following : 
//  - Architecture : 
//     + Game Engine Architecture (by Jason Gregory - http://www.gameenginebook.com/)
//     + OGRE (Object-Oriented Graphics Rendering Engine) (http://www.ogre3d.org/)
//  - Rendering API : 
//     + Introduction to 3D Game Programming with Direct3D 11.0 (http://www.d3dcoder.net/)
//     + OpenGL Programming Guide: The Official Guide to Learning OpenGL, Version 4.3
//////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------

#include "stdafx.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef RJE_DEBUG
	// Enable run-time memory check for debug builds.
#	ifdef STD_MEMORY_PROFILE
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(X);
#	endif

	//try to create a console
	if ( AllocConsole() == 0 )
	{
		RJE_MESSAGE_BOX( 0, L"Can't allocate console!\n", L"Error\n", MB_OK | MB_ICONSTOP );
		return FALSE;
	}
	FILE *stream;
	freopen_s( &stream, "CON", "w", stdout);
	freopen_s( &stream, "CON", "w", stderr);
	freopen_s( &stream, "CON", "r", stdin);

	SetConsoleTitle(L"RamJam Engine Debug Console");
#endif

	//////////////////////////////////////////////////////////////////////////



	System::Instance()->mHInst = hInstance;
	RJE_ASSERT(System::Instance()->Initialize(nCmdShow));

	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	std::string file = (System::Instance()->mDataPath + CIniFile::GetValue("simple", "scenes", System::Instance()->mResourcesPath));
	fopen_s( &pFile, file.c_str() , "r" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize + 1);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	buffer[lSize] = '\0';

	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose (pFile);


	rapidxml::xml_document<> doc;    // character type defaults to char
	doc.parse<0>(buffer);    // 0 means default parse flags

	std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
	rapidxml::xml_node<> *node = doc.first_node("SCENE");
	for (rapidxml::xml_node<> *attr = node->first_node();
		attr; attr = attr->next_sibling())
	{
		std::cout << "Node foobar has node " << attr->name() << " ";
		std::cout << "with value " << attr->value() << "\n";
	}

	std::ofstream myfile;
	myfile.open ("sceneout.xml");

	myfile << doc;

	myfile.close();

	free(buffer);
	System::Instance()->Shutdown();
	System::DeleteInstance();



	//////////////////////////////////////////////////////////////////////////

	/*
	System::Instance()->mHInst = hInstance;
	RJE_ASSERT(System::Instance()->Initialize(nCmdShow));
	System::Instance()->OnResize();

	System::Instance()->Run();

	System::Instance()->Shutdown();
	System::DeleteInstance();
	*/

#ifdef RJE_MEMORY_PROFILE
	MemoryReport();
#endif

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////     TODO     //////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
--
*/
