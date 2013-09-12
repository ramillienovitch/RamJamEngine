#pragma once

#include "Types.h"
#include "Debug.h"

#pragma warning(disable: 4291)
//	warning C4291: 'void *operator new(size_t,const char *,int)' :
//	no matching operator delete found; memory will not be freed if initialization throws an exception
//	If you use the operator new without any extra arguments and compile with /GX, /EHs, or /EHa options to enable exception handling,
//	the compiler will generate code to call operator delete if the constructor throws an exception.

//////////////////////////////////////////////////////////////////////////
void AddTrack(DWORD addr,  DWORD asize, const char *fname, DWORD lnum);
void RemoveTrack(DWORD addr);
void MemoryReport();

#ifdef RJE_DEBUG
//////////////////////////////////////////////////////////////////////////
inline void* operator new(size_t size, const char* szFileName, int nLine)
{
	void *ptr = (void *)malloc(size);
	if (!ptr)
		throw "operator new() error : Bad Alloc";
	AddTrack((DWORD)ptr, (DWORD)size, szFileName, nLine);
	return(ptr);
}
inline void* operator new[](size_t size, const char* szFileName, int nLine)
{
	void *ptr = (void *)malloc(size);
	if (!ptr)
		throw "operator new[]() error : Bad Alloc";

	AddTrack((DWORD)ptr, (DWORD)size, szFileName, nLine);
	return(ptr);
}

//////////////////////////////////////////////////////////////////////////
inline void operator delete(void* pMem)
{
	RemoveTrack((DWORD)pMem);
	free(pMem);
}
inline void operator delete[](void* pMem)
{
	RemoveTrack((DWORD)pMem);
	free(pMem);
}
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef RJE_DEBUG
#	define DEBUG_NEW new(__FILE__, __LINE__)
#else
#	define DEBUG_NEW new
#endif
#define new DEBUG_NEW
