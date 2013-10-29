#pragma once

#include "Types.h"

namespace RJE
{
#	if defined(DEBUG) | defined(_DEBUG)
#	define RJE_DEBUG
#	endif

#	ifdef RJE_DEBUG
#		define RJE_ASSERT(x) _ASSERTE(x)
#	else
#		define RJE_ASSERT(x) (x)
#	endif
#	define RJE_C_ASSERT(x, y)	static_assert(x, y)

#	define RJE_ALIGNOF( X )		__declspec( align( X ) )

#	define RJE_PROFILE_SAMPLE		0
#	define RJE_PROFILE_SAMPLERATE	10
#	define RJE_PROFILE_INTRUSIVE	1
#	define RJE_PROFILE_CPU			1
#	define RJE_PROFILE_GPU			1

#	if (RJE_PROFILE_CPU == 1) & (RJE_PROFILE_GPU == 1)
#	define RJE_PROFILE_ALL
#	endif

#	if (RJE_GRAPHIC_API == DIRECTX_11)
#		define RJE_CHECK_FOR_SUCCESS( X )	if (FAILED(X)) {DXTrace(__FILE__, (DWORD)__LINE__, X, L#X, true);}
#		define RJE_WAIT_FOR_SUCCESS( X )	while (X != S_OK)
#	else
#		define RJE_CHECK_FOR_SUCCESS( X )	if (FAILED(X)) return false;
#		define RJE_WAIT_FOR_SUCCESS( X )	while (!X)
#	endif
#	define RJE_CHECK_WINDOWS_CALL( X )	if (X==0) {RJE_ASSERT(false); GetLastError();}
#	define RJE_CHECK_IF_NULL( X )		if (X==0) return false;
#	define RJE_CHECK_IF_NULLPTR( X )	if (X==nullptr) return false;

#	define RJE_BREAK()						DebugBreak()
#	define RJE_BREAK_PROCESS()				DebugBreakProcess()
#	define RJE_PRINT						DebugPrintf
#	define RJE_PRINT_VERBOSE				DebugPrintVerbose
#	define RJE_PRINT_RAW( X )				OutputDebugStringA( X )
#	define RJE_MESSAGE_BOX( X, Y, Z, W )	MessageBox( X, Y, Z, W )
#	define RJE_MESSAGE_BEEP( X )			MessageBeep( X )

#	define RJE_QUOTE_INPLACE(x)		# x
#	define RJE_QUOTE(x)				RJE_QUOTE_INPLACE(x)
#	define RJE_WARNING(x)			message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

#	define RJE_SAFE_DELETE( X )				if(X) { delete X; X=nullptr; }
#	define RJE_SAFE_DELETE_PTR( X )			if(X) { delete [] X; X=nullptr; }
#	define RJE_SAFE_RELEASE( X )			if(X) { X->Release(); X=nullptr; }
#	define RJE_SHUTDOWN( X )				if(X) { X->Shutdown(); delete X; X=nullptr; }
}

//////////////////////////////////////////////////////////////////////////
void DebugPrintf(LPCSTR szFormat, ...);

//////////////////////////////////////////////////////////////////////////
void DebugPrintVerbose(int verbosity, LPCSTR szFormat, ...);
