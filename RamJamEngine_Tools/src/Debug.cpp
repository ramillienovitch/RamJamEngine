#include "Debug.h"
#include "Globals.h"

//////////////////////////////////////////////////////////////////////////
void DebugPrintf(LPCSTR szFormat, ...)
{
	char strA[MAX_STRING_DBG];
	char strB[MAX_STRING_DBG];

	va_list ap;
	va_start(ap, szFormat);
	vsprintf_s(strA, sizeof(strA), szFormat, ap);
	strA[MAX_STRING_DBG-1] = '\0';
	va_end(ap);

	sprintf_s(strB, sizeof(strB), "%s", strA);

	strB[MAX_STRING_DBG-1] = '\0';

	OutputDebugStringA(strB);
}

//////////////////////////////////////////////////////////////////////////
void DebugPrintVerbose(int verbosity, LPCSTR szFormat, ...)
{
	// Only print when the global verbosity level is high enough.
	if (verbosity <= RJE_GLOBALS::gDebugVerbosity)
	{
		DebugPrintf(szFormat);
	}
}