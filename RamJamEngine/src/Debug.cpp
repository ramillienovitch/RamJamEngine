#include "stdafx.h"

////////////////////////////////////////////////////////////////////////
int VDebugPrint(const char* format, va_list argList)
{
	char sBuffer[MAX_STRING_DBG];
	int charsWritten = vsnprintf_s(sBuffer, MAX_STRING_DBG-1, format, argList);
	sBuffer[MAX_STRING_DBG-1] = '\0'; // be sure to NIL-terminate

	// Now that we have a formatted string, call the Win32 API.
	RJE_PRINT_RAW(sBuffer);
	return charsWritten;
}

//////////////////////////////////////////////////////////////////////////
int DebugPrint(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	int charsWritten = VDebugPrint(format, argList);
	va_end(argList);
	return charsWritten;
}

//////////////////////////////////////////////////////////////////////////
int DebugPrintVerbose(int verbosity, const char* format, ...)
{
	// Only print when the global verbosity level is high enough.
	if (verbosity <= RJE_GLOBALS::gDebugVerbosity)
	{
		va_list argList;
		va_start(argList, format);
		int charsWritten = VDebugPrint(format, argList);
		va_end(argList);
		return charsWritten;
	}
	return 0;
}