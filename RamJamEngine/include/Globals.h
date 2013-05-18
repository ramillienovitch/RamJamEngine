#pragma once

#define COMPUTE_NORMALS			1
#define FULL_SCREEN				0
#define MAX_LOADSTRING			100
#define MAX_STRING_DBG			2048
#define RIGHT_HANDED			1
#define SCREEN_DEPTH			1000.0f
#define SCREEN_NEAR				0.1f
#define SCREEN_HEIGHT			720
#define SCREEN_WIDTH			1280
#define SHOW_CURSOR				1
#define USE_QUATERNION_DX11		0
#define VERBOSE_CRITICAL		0
#define VERBOSE_ERROR			1
#define VERBOSE_WARNING			2
#define VERBOSE_MESSAGE			3
#define VERBOSE_ALL				4
#define VSYNC_ENABLED			1

namespace RJE_GLOBALS
{
	static bool	gEnableMultiSampling	= true;
	static int	gMultiSamplingCount		= 1;		// 1 means no multi sampling
	static int	gMultisamplingQuality	= 0;

	// Debug 
	static int	gDebugVerbosity		= 0;
}