#pragma once

#include "Types.h"

namespace RJE_GLOBALS
{
	#define MAX_STRING_DBG			4096
	#define VERBOSE_CRITICAL		0
	#define VERBOSE_ERROR			1
	#define VERBOSE_WARNING			2
	#define VERBOSE_MESSAGE			3
	#define VERBOSE_ALL				4
	
	// -----------------------------------------------------------------------

	//************************************************************************
	//	Rendering
	//************************************************************************
	static BOOL		gFullScreen;
	static BOOL		gVsyncEnabled;
	static int		gScreenWidth;
	static int		gScreenHeight;
	static BOOL		gEnableMultiSampling;
	static int		gMultiSamplingCount;
	static int		gMultisamplingQuality;
	static BOOL		gUse4xMsaa;
	static u32		g4xMsaaQuality;

	//************************************************************************
	//	Debug
	//************************************************************************
	static int		gDebugVerbosity;
	static BOOL		gShowCursor;

	//************************************************************************
	//	Misc
	//************************************************************************
	static BOOL		gRunInBackground;
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