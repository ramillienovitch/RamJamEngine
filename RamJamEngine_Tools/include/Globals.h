#pragma once

#include "Types.h"

namespace RJE_GLOBALS
{
	#define MAX_STRING_DBG			2048
	#define VERBOSE_CRITICAL		0
	#define VERBOSE_ERROR			1
	#define VERBOSE_WARNING			2
	#define VERBOSE_MESSAGE			3
	#define VERBOSE_ALL				4
	
	// -----------------------------------------------------------------------

	//************************************************************************
	//	Rendering
	//************************************************************************
	static BOOL		gFullScreen				= false;
	static int		gScreenWidth			= 1280;
	static int		gScreenHeight			= 720;
	static BOOL		gVsyncEnabled			= false;
	static BOOL		gEnableMultiSampling	= true;
	static int		gMultiSamplingCount		= 1;
	static int		gMultisamplingQuality	= 0;
	static BOOL		gUse4xMsaa				= true;
	static u32		g4xMsaaQuality;

	//************************************************************************
	//	Debug
	//************************************************************************
	static int		gDebugVerbosity		= 0;
	static BOOL		gShowCursor			= true;

	//************************************************************************
	//	Misc
	//************************************************************************
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