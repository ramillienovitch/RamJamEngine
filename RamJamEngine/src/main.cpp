
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

	System::Instance()->mHInst = hInstance;
	RJE_ASSERT(System::Instance()->Initialize(nCmdShow));
	System::Instance()->OnResize();

	System::Instance()->Run();

	System::Instance()->Shutdown();
	System::DeleteInstance();


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
