#pragma once

#include "Types.h"

namespace RJE_GLOBALS
{
#	define MAX_STRING_DBG			4096
#	define VERBOSE_CRITICAL			0
#	define VERBOSE_ERROR			1
#	define VERBOSE_WARNING			2
#	define VERBOSE_MESSAGE			3
#	define VERBOSE_ALL				4

	namespace CONSOLE
	{
#		define CONSOLE_HEIGHT			200
#		define COMMAND_MAX_LENGTH		128
#		define HISTORIC_LENGTH			64
#		define LINE_MAX					8
	}

	namespace SCREEN_INFO
	{
#		define SCREEN_WHITE			1
#		define SCREEN_RED			2
#		define SCREEN_GREEN			3
#		define SCREEN_BLUE			4
#		define SCREEN_YELLOW		5
#		define SCREEN_ORANGE		6
#		define SCREEN_ROSE			7
#		define SCREEN_GRAY			8
#		define SCREEN_BLACK			9
	}

#	define nullchar		'\0'
#	define newline		'\n'
	
	// -----------------------------------------------------------------------

	//************************************************************************
	//	Rendering
	//************************************************************************
	extern BOOL		gFullScreen;
	extern int		gScreenWidth;
	extern int		gScreenHeight;

	//************************************************************************
	//	Debug
	//************************************************************************
	extern int		gDebugVerbosity;
	extern BOOL		gShowCursor;

	//************************************************************************
	//	Misc
	//************************************************************************
	extern BOOL		gRunInBackground;
}
