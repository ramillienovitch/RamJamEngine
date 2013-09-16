#include "Console.h"

Console* Console::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
Console::~Console() { delete mConsoleBuffer; }

//////////////////////////////////////////////////////////////////////////
Console::Console()
{
	mIsActive          = false;
	mConsoleBuffer     = new char[COMMAND_MAX_LENGTH*LINE_MAX];
	mConsoleBuffer[0]  = nullchar;
	mConsoleBufferSize = 0;
	mLineCount         = 0;

	ClearCommand();
}

//////////////////////////////////////////////////////////////////////////
u8		Console::GetCommandLength() { return mCurrentCmdLength; }
BOOL	Console::IsActive()			{ return mIsActive; }

//////////////////////////////////////////////////////////////////////////
void Console::GetCommand( OUT char (&p)[COMMAND_MAX_LENGTH] )
{
	memcpy(p, &mCurrentCmd, COMMAND_MAX_LENGTH);
	return;
}

//////////////////////////////////////////////////////////////////////////
void Console::RegisterAndClearCommand()
{
	// TODO : register the command and do it !
	ConcatCommand();
	ClearCommand();
}

//////////////////////////////////////////////////////////////////////////
void Console::ClearCommand()
{
	mCurrentCmdLength = 3;

	mCurrentCmd.Cmd[0] = '>';
	mCurrentCmd.Cmd[1] = ':';
	mCurrentCmd.Cmd[2] = ' ';
	mCurrentCmd.Cmd[3] = nullchar;
}

//////////////////////////////////////////////////////////////////////////
void Console::ConcatText(const char* text, u8 size)
{
	// TODO !
}

//////////////////////////////////////////////////////////////////////////
void Console::ConcatCommand()
{
	mConsoleBuffer[mConsoleBufferSize++] = newline;
	for (int i=0; i<COMMAND_MAX_LENGTH; ++i)
	{
		mConsoleBuffer[mConsoleBufferSize++] = mCurrentCmd.Cmd[i];
	}
}

//////////////////////////////////////////////////////////////////////////
void Console::ToggleConsoleState()
{
	if (mIsActive)
	{
		mIsActive=false;
		ClearCommand();
	}
	else mIsActive = true;
}

//////////////////////////////////////////////////////////////////////////
void Console::AddCharacter( const char c )
{
	if (mCurrentCmdLength >= COMMAND_MAX_LENGTH-1)
		return;

	mCurrentCmd.Cmd[mCurrentCmdLength++] = c;
	mCurrentCmd.Cmd[mCurrentCmdLength]   = nullchar;
}

//////////////////////////////////////////////////////////////////////////
void Console::RemoveCharacter()
{
	if (mCurrentCmdLength < 4)
		return;

	mCurrentCmd.Cmd[--mCurrentCmdLength] = nullchar;
}
