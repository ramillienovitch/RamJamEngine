#include "Console.h"

#include "Memory.h"
#include "System.h"

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

	// Initialize the command list
	CommandList["Exit"]      = Exit;
	CommandList["Wireframe"] = SetWireframe;
}

//////////////////////////////////////////////////////////////////////////
u16		Console::GetCommandLength()	{ return mCurrentCmdLength; }
BOOL	Console::IsActive()			{ return mIsActive; }

//////////////////////////////////////////////////////////////////////////
void Console::GetCommand( OUT char (&p)[COMMAND_MAX_LENGTH] )
{
	memcpy(p, &mCurrentCmd, COMMAND_MAX_LENGTH);
	return;
}

//////////////////////////////////////////////////////////////////////////
void Console::GetParams(const char* params)
{
	// TODO !
}

//////////////////////////////////////////////////////////////////////////
void Console::RegisterAndClearCommand()
{
	// Display the command in the console
	ConcatCommand();
	
	RegisterCommand();
	ClearCommand();
}

//////////////////////////////////////////////////////////////////////////
void Console::RegisterCommand()
{
	// if the command is empty (">: ")
	if (mCurrentCmd.Size()==3)
	{
		mConsoleBuffer[mConsoleBufferSize++] = newline;
		mConsoleBuffer[mConsoleBufferSize]   = nullchar;
		return;
	}

	f_ptr handle = IsValidCommand();
	if (handle == nullptr)
		DisplayError();
	else
	{
		char* params = nullptr;
		GetParams(params);
		// We execute the command
		handle(params);
	}
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
void Console::ClearConsole()
{
	mConsoleBuffer[0]  = nullchar;
	mConsoleBufferSize = 0;
}

//////////////////////////////////////////////////////////////////////////
void Console::ConcatText(const char* text, u16 size)
{
	// TODO !
}

//////////////////////////////////////////////////////////////////////////
void Console::ConcatCommand()
{
	if (mConsoleBufferSize > 0)
		mConsoleBuffer[mConsoleBufferSize++] = newline;

	int cmdSize = mCurrentCmd.Size();
	for (int i=3; i<cmdSize; ++i)
		mConsoleBuffer[mConsoleBufferSize++] = mCurrentCmd.Cmd[i];
	
	mConsoleBuffer[mConsoleBufferSize] = nullchar;
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

//////////////////////////////////////////////////////////////////////////
f_ptr Console::IsValidCommand()
{
	int cmdSize = mCurrentCmd.Size();
	RJE_ASSERT(cmdSize > 3);

	char* cmd = new char[cmdSize+1];

	for (int i=3; i<cmdSize; ++i)
		cmd[i-3] = mCurrentCmd.Cmd[i];

	cmd[cmdSize-3] = nullchar;

	// We check if the command exists in the command list
	std::map<char*, f_ptr>::iterator it;
	for (it=CommandList.begin(); it!=CommandList.end(); ++it)
	{
		if(strcmp(it->first, cmd) == 0)
		{
			f_ptr handle = it->second;
			delete cmd;
			return handle;
		}
	}
	delete cmd;
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void Console::DisplayError()
{
	// We display an error formatted as such : [    2"command" : command not recognized. Syntax error1\0]
	mConsoleBuffer[mConsoleBufferSize++] = newline;

	for (int i=0; i<4; ++i)
		mConsoleBuffer[mConsoleBufferSize++] = ' ';
	mConsoleBuffer[mConsoleBufferSize++] = 2;	// display in red the following text
	mConsoleBuffer[mConsoleBufferSize++] = '"';

	int cmdSize = mCurrentCmd.Size();
	for (int i=3; i<cmdSize; ++i)
		mConsoleBuffer[mConsoleBufferSize++] = mCurrentCmd.Cmd[i];
	mConsoleBuffer[mConsoleBufferSize++] = '"';

	char* temp = " : command not recognized. Syntax error";
	memcpy(mConsoleBuffer + mConsoleBufferSize, temp, strlen(temp));
	mConsoleBufferSize += (u16)strlen(temp);

	mConsoleBuffer[mConsoleBufferSize++] = 1;
	mConsoleBuffer[mConsoleBufferSize] = nullchar;
}

//////////////////////////////////////////////////////////////////////////
/// COMMANDS
//////////////////////////////////////////////////////////////////////////

void Exit(char* command /* = nullptr */)
{
	printf("Exit\n");
	//SendMessage(System::Instance()->mHWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
}

void SetWireframe(char* command /* = nullptr */)
{
	// TODO !
	printf("SetWireframe\n");
}
