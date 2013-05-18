// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

// RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream>
#include <vector>

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
// Core
#include "Globals.h"
#include "Types.h"
#include "Debug.h"
#include "Profiler.h"
#include "Timer.h"
//////////////////////////////////////////////////////////////////////////
// Math
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
//////////////////////////////////////////////////////////////////////////
// RamJam Engine SubCores
#include "System.h"