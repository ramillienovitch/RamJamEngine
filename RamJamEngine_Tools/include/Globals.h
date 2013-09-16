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

	// Console
#	define COMMAND_MAX_LENGTH		128
#	define HISTORIC_LENGTH			64
#	define LINE_MAX					5

#	define nullchar		'\0'
#	define newline		'\n'
	
	// -----------------------------------------------------------------------

	//************************************************************************
	//	Rendering
	//************************************************************************
	extern BOOL		gFullScreen;
	extern BOOL		gVsyncEnabled;
	extern int		gScreenWidth;
	extern int		gScreenHeight;
	extern BOOL		gEnableMultiSampling;
	extern int		gMultiSamplingCount;
	extern int		gMultisamplingQuality;
	extern BOOL		gUse4xMsaa;
	extern u32		g4xMsaaQuality;

	//************************************************************************
	//	Debug
	//************************************************************************
	extern int		gDebugVerbosity;
	extern BOOL		gShowCursor;

	//************************************************************************
	//	Misc
	//************************************************************************
	extern BOOL		gRunInBackground;
	extern BOOL		gUpdateSceneRuntime;
	namespace Colors
	{
		const float White[]				= {1.0f, 1.0f, 1.0f, 1.0f};
		const float Black[]				= {0.0f, 0.0f, 0.0f, 1.0f};
		const float Red[]				= {1.0f, 0.0f, 0.0f, 1.0f};
		const float Green[]				= {0.0f, 1.0f, 0.0f, 1.0f};
		const float Blue[]				= {0.0f, 0.0f, 1.0f, 1.0f};
		const float Yellow[]			= {1.0f, 1.0f, 0.0f, 1.0f};
		const float Cyan[]				= {0.0f, 1.0f, 1.0f, 1.0f};
		const float Magenta[]			= {1.0f, 0.0f, 1.0f, 1.0f};

		const float Silver[]			= {0.75f, 0.75f, 0.75f, 1.0f};
		const float LightSteelBlue[]	= {0.69f, 0.77f, 0.87f, 1.0f};
	}
}