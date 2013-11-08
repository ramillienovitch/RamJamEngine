#pragma once

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <limits>

#if PLATFORM == PLATFORM_WIN32
#	include <Windows.h>
#endif

typedef				__int8				i8;
typedef				__int16				i16;
typedef				__int32				i32;

typedef unsigned	__int8				u8;
typedef unsigned	__int16				u16;
typedef unsigned	__int32				u32;

#if COMPILER == COMPILER_MSVC
typedef				__int64				i64;
typedef unsigned	__int64				u64;
#else
typedef				long long			i64;
typedef unsigned	long long			u64;
#endif

typedef				char				charASCII;
typedef				u8					charUTF8;
typedef				char16_t			charUTF16;
typedef				char32_t			charUTF32;
typedef				wchar_t				charWide;

typedef				void*				Ptr;
typedef				void const*			PtrConst;
typedef				void const* const	PtrConst_Const;
typedef				void* const			Ptr_Const;

typedef				float				f32;
typedef				double				f64;
typedef				long double			f80;


//////////////////////////////////////////////////////////////////////////
//---------------------- Conversion Utilities -------------------------//
#define dtoa _gcvt_s
#define itoa _itoa_s

// Converts an ANSI string to a std::wstring
inline std::wstring AnsiToWString(const char* ansiString)
{
	WCHAR buffer[512];
	if (MultiByteToWideChar(CP_ACP, 0, ansiString, -1, buffer, 512))
		return std::wstring(buffer);
	
	return std::wstring();
}

// Converts an ANSI string to a std::wstring
inline std::wstring StringToWString(const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());

	return wsTmp;
}

// Converts a wstring to an ANSI string
inline std::string WStringToString(const std::wstring &ws)
{
	std::string sTmp(ws.begin(), ws.end());

	return sTmp;
}

// Converts a number to a string
template<typename T>
inline std::wstring ToString(const T& val)
{
	std::wostringstream stream;
	stream << val;
	return stream.str();
}

// Converts a number to an ansi string
template<typename T>
inline std::string ToAnsiString(const T& val)
{
	std::ostringstream stream;
	stream << val;
	return stream.str();
}


//
// The INT_PTR is guaranteed to be the same size as a pointer.  Its
// size with change with pointer size (32/64).  It should be used
// anywhere that a pointer is cast to an integer type. UINT_PTR is
// the unsigned variation.
//
// __int3264 is intrinsic to 64b MIDL but not to old MIDL or to C compiler.
//
#if ( defined(__midl) && (501 < __midl) )

typedef [public] __int3264 INT_PTR, *PINT_PTR;
typedef [public] unsigned __int3264 UINT_PTR, *PUINT_PTR;

typedef [public] __int3264 LONG_PTR, *PLONG_PTR;
typedef [public] unsigned __int3264 ULONG_PTR, *PULONG_PTR;

#else  // midl64
// old midl and C++ compiler

#if defined(_WIN64)
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

#define __int3264   __int64

#else
typedef _W64 int INT_PTR, *PINT_PTR;
typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;

typedef _W64 long LONG_PTR, *PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

#define __int3264   __int32

#endif
#endif // midl64

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#define LOWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
#define HIWORD(_dw)     ((WORD)((((DWORD_PTR)(_dw)) >> 16) & 0xffff))
#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

//////////////////////////////////////////////////////////////////////////
//----------------------- Temporary Typedefs --------------------------//
typedef		::std::string	String;


//////////////////////////////////////////////////////////////////////////
//----------------------- Temporary Defines --------------------------//
#define		HashMap			::std::tr1::unordered_map
#define		HashSet			::std::tr1::unordered_set
