#pragma once

#ifndef RJE_DOUBLE_PRECISION
#define RJE_DOUBLE_PRECISION 0
#endif

#ifndef RJE_CONFIG_BIG_ENDIAN
#define RJE_CONFIG_BIG_ENDIAN 0
#endif
#define ENDIAN_LITTLE	1
#define ENDIAN_BIG		2

#if RJE_CONFIG_BIG_ENDIAN
#    define RJE_ENDIAN ENDIAN_BIG
#else
#    define RJE_ENDIAN ENDIAN_LITTLE
#endif

#define PLATFORM_WIN32		1
#define PLATFORM_LINUX		2
#define PLATFORM_PS3		3
#define PLATFORM_X360		4

#define COMPILER_MSVC		1
#define COMPILER_GNUC		2
#define COMPILER_BORL		3
#define COMPILER_WINSCW		4
#define COMPILER_GCCE		5
#define COMPILER_CLANG		6

#define ARCHITECTURE_32 1
#define ARCHITECTURE_64 2

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define RJE_ARCHITECTURE ARCHITECTURE_64
#else
#   define RJE_ARCHITECTURE ARCHITECTURE_32
#endif


//////////////////////////////////////////////////////////////////////////
// Compiler
#if defined( __GCCE__ )
#	define COMPILER COMPILER_GCCE
#	define COMP_VER _MSC_VER
#elif defined( __WINSCW__ )
#	define COMPILER COMPILER_WINSCW
#	define COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#	define COMPILER COMPILER_MSVC
#	define COMP_VER _MSC_VER
#elif defined( __clang__ )
#	define COMPILER COMPILER_CLANG
#	define COMP_VER (((__clang_major__)*100) + (__clang_minor__*10) + __clang_patchlevel__)
#elif defined( __GNUC__ )
#	define COMPILER COMPILER_GNUC
#	define COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
#elif defined( __BORLANDC__ )
#	define COMPILER COMPILER_BORL
#	define COMP_VER __BCPLUSPLUS__
#	define __FUNCTION__ __FUNC__ 
#else
#	pragma error "No known compiler. Abort! Abort!"
#endif

//////////////////////////////////////////////////////////////////////////
// Force Inline
#if COMPILER == COMPILER_MSVC
#	if COMP_VER >= 1200
#		define FORCEINLINE __forceinline
#	endif
#elif defined(__MINGW32__)
#	if !defined(FORCEINLINE)
define FORCEINLINE __inline
#	endif
#else
#	define FORCEINLINE __inline
#endif

//////////////////////////////////////////////////////////////////////////
// Platform
#if defined( __WIN32__ ) || defined( _WIN32 )
#	define PLATFORM PLATFORM_WIN32
//#elif defined ( PS3 )
//#	define PLATFORM PLATFORM_PS3
//#elif defined ( X360 )
//#	define PLATFORM PLATFORM_X360
#else
#   define PLATFORM PLATFORM_LINUX
#endif

//////////////////////////////////////////////////////////////////////////
// Application Programming Interface
#define DIRECTX_11 1
#define OPENGL_4_3 2

#ifndef USE_DIRECTX_11		// i.e. !USE_OPENGL_4_3
#define USE_DIRECTX_11 1
#endif

#if USE_DIRECTX_11 == 1
#	define RJE_GRAPHIC_API DIRECTX_11
#else
#	define RJE_GRAPHIC_API OPENGL_4_3
#endif
