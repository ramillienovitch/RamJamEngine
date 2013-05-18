
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
// The RamJamEngine is for learning purposes only.
// I don't pretend to make the next big real-time 3D engine
//
// The RJE is based on the following : 
//  - Architecture : 
//     + Game Engine Architecture (by Jason Gregory - http://www.gameenginebook.com/)
//     + OGRE (Object-Oriented Graphics Rendering Engine) (http://www.ogre3d.org/)
//  - Rendering API : 
//     + Introduction to 3D Game Programming with Direct3D 11.0 (http://www.d3dcoder.net/)
//     + OpenGL Programming Guide: The Official Guide to Learning OpenGL, Version 4.3
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	System*	system	= new System;
	BOOL result;

	if(!system)
		return 0;

	result = system->Initialize(nCmdShow);
	if(result)
		system->Launch();

	RJE_SHUTDOWN(system);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////     TODO     //////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
--
*/