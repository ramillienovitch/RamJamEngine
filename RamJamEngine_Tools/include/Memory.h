#pragma once

#include "Types.h"
#include "Debug.h"

#include <memory>

#ifdef RJE_DEBUG
#	define RJE_MEMORY_PROFILE
#	define STD_MEMORY_PROFILE
#endif

//////////////////////////////////////////////////////////////////////////
void AddTrack(DWORD addr,  DWORD asize, const char *fname, DWORD lnum);
void RemoveTrack(DWORD addr);
void MemoryReport();

#ifdef RJE_MEMORY_PROFILE
//////////////////////////////////////////////////////////////////////////
FORCEINLINE void* operator new(size_t size, const char* szFileName, int nLine)
{
	void *ptr = (void *)malloc(size);
	if (!ptr)
		throw "operator new() error : Bad Alloc";
	AddTrack((DWORD)ptr, (DWORD)size, szFileName, nLine);
	return(ptr);
}
//-------------
FORCEINLINE void* operator new[](size_t size, const char* szFileName, int nLine)
{
	void *ptr = (void *)malloc(size);
	if (!ptr)
		throw "operator new[]() error : Bad Alloc";

	AddTrack((DWORD)ptr, (DWORD)size, szFileName, nLine);
	return(ptr);
}
//-------------
FORCEINLINE void* operator new(size_t size, void * where, const char* szFileName, int nLine)
{ return where; }

//////////////////////////////////////////////////////////////////////////
FORCEINLINE void operator delete(void* pMem)
{ RemoveTrack((DWORD)pMem); free(pMem); }
//-------------
FORCEINLINE void operator delete[](void* pMem)
{ RemoveTrack((DWORD)pMem); free(pMem); }
//-------------
FORCEINLINE void operator delete(void* pMem, void * where, const char* szFileName, int nLine)  {/* if the operator new fails */}
//-------------
FORCEINLINE void operator delete(void* pMem, const char* szFileName, int nLine) {/* if the operator new fails */}
//-------------
FORCEINLINE void operator delete[](void* pMem, const char* szFileName, int nLine) {/* if the operator new[] fails */}
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef RJE_MEMORY_PROFILE
#	define rje_new new(__FILE__, __LINE__)
#else
#	define rje_new new
#endif
