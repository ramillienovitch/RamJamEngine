#pragma once

#include "Types.h"

namespace RJE
{
#	if defined(DEBUG) | defined(_DEBUG)
#		define RJE_ASSERT(x) _ASSERTE(x)
#	else
#		define RJE_ASSERT(x) (x)
#	endif

#	define RJE_ALIGNOF( X )		__declspec( align( X ) )

#	if (RJE_GRAPHIC_API == DIRECTX_11)
#		define RJE_CHECK_FOR_SUCCESS( X )	if (FAILED(X)) {DXTrace(__FILE__, (DWORD)__LINE__, X, L#X, true);}
#	else
#		define RJE_CHECK_FOR_SUCCESS( X )	if (FAILED(X)) return false;
#	endif
#	define RJE_CHECK_IF_NULL( X )		if (X==0) return false;
#	define RJE_CHECK_IF_NULLPTR( X )	if (X==nullptr) return false;

#	define RJE_BREAK()						DebugBreak()
#	define RJE_BREAK_PROCESS()				DebugBreakProcess()
#	define RJE_PRINT( X )					DebugPrint( X )
#	define RJE_PRINT_RAW( X )				OutputDebugString( (LPCWSTR) X )
#	define RJE_MESSAGE_BOX( X, Y, Z, W )	MessageBox( X, Y, Z, W )

#	define RJE_QUOTE_INPLACE(x)		# x
#	define RJE_QUOTE(x)				RJE_QUOTE_INPLACE(x)
#	define RJE_WARNING(x)			message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

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