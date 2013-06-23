#pragma once

#ifndef RJE_PROFILING
#define RJE_PROFILING 0
#endif

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


#define ARCHITECTURE_32 1
#define ARCHITECTURE_64 2

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define RJE_ARCHITECTURE ARCHITECTURE_64
#else
#   define RJE_ARCHITECTURE ARCHITECTURE_32
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
