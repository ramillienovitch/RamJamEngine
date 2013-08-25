#pragma once

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

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
