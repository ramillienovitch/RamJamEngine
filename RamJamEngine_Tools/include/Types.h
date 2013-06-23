#pragma once

#include "Platform.h"

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


//////////////////////////////////////////////////////////////////////////
//----------------------- Temporary Typedefs --------------------------//
typedef		::std::string	String;


//////////////////////////////////////////////////////////////////////////
//----------------------- Temporary Defines --------------------------//
#define		HashMap			::std::tr1::unordered_map
#define		HashSet			::std::tr1::unordered_set
