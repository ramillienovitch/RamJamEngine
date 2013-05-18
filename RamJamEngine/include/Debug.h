#pragma once

#include "stdafx.h"

namespace RJE
{
#	ifndef NDEBUG
#		define RJE_ASSERT(x) _ASSERTE(x)
#	else
#		define RJE_ASSERT(x)
#	endif

#	define RJE_ALIGNOF( X )		__declspec( align( X ) )

#	define RJE_CHECK_FOR_SUCCESS( X )	if (FAILED(X)) {return false;}
#	define RJE_CHECK_IF_NULL( X )		if (X==0) {return false;}
#	define RJE_CHECK_IF_NULLPTR( X )	if (X==nullptr) {return false;}

#	define RJE_BREAK()						DebugBreak()
#	define RJE_BREAK_PROCESS()				DebugBreakProcess()
#	define RJE_PRINT( X )					DebugPrint( X )
#	define RJE_PRINT_RAW( X )				OutputDebugString( (LPCWSTR) X )
#	define RJE_MESSAGE_BOX( X, Y, Z, W )	MessageBox( X, Y, Z, W )

#	define RJE_SAFE_DELETE( X )				if(X) { delete X; X=nullptr; }
#	define RJE_SAFE_DELETE_PTR( X )			if(X) { delete [] X; X=nullptr; }
#	define RJE_SAFE_RELEASE( X )			if(X) { X->Release(); X=nullptr; }
#	define RJE_SHUTDOWN( X )				if(X) { X->Shutdown(); delete X; X=nullptr; }
}

////////////////////////////////////////////////////////////////////////
int VDebugPrint(const char* format, va_list argList);

//////////////////////////////////////////////////////////////////////////
int DebugPrint(const char* format, ...);

//////////////////////////////////////////////////////////////////////////
int DebugPrintVerbose(int verbosity, const char* format, ...);
