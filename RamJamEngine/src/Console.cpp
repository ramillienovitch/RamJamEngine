#include "Console.h"

#include "Memory.h"
#include "System.h"

Console* Console::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
Console::~Console() { delete mConsoleBuffer; }

//////////////////////////////////////////////////////////////////////////
Console::Console()
{
	mIsActive      = false;
	mConsoleBuffer = new char[COMMAND_MAX_LENGTH*LINE_MAX];
	
	mHistoricCount     = 0;
	mCurrentHistoric = -1;

	ClearConsole();
	ClearCommand();

	////////////////////////////////////
	// Initialize the command list
	// ------ Console ------
	CommandList["clear"] = CleanConsole;
	// ------- Core -------
	CommandList["quit"]     = Exit;
	CommandList["exit"]     = Exit;
	CommandList["shutdown"] = Exit;
	// ----- Rendering -----
	CommandList["wireframe"] = SetWireframe;
	// ------- Time -------
	CommandList["timescale"] = SetTimeScale;
	CommandList["time"]      = GetTime;
}

//////////////////////////////////////////////////////////////////////////
u16		Console::GetCommandLength()	{ return mCurrentCmdLength; }
u16		Console::GetLineCount()		{ return mLineCount; }
BOOL	Console::IsActive()			{ return mIsActive; }

//////////////////////////////////////////////////////////////////////////
void Console::GetCommand( OUT char (&p)[COMMAND_MAX_LENGTH] )
{
	memcpy(p, &mCurrentCmd, COMMAND_MAX_LENGTH);
	return;
}

//////////////////////////////////////////////////////////////////////////
void Console::GetCommandHistoric(BOOL upwards)
{
	if (upwards)
	{
		if (mCurrentHistoric >= HISTORIC_LENGTH-1 || mCurrentHistoric >= mHistoricCount -1)
			return;
		memcpy(mCurrentCmd.Cmd, mConsoleHistoric[mHistoricCount-(++mCurrentHistoric+1)].Cmd, COMMAND_MAX_LENGTH);
	}
	else
	{
		if (mCurrentHistoric < 1)
		{
			ClearCommand();
			if (mCurrentHistoric == 0)
				--mCurrentHistoric;
			return;
		}
		memcpy(mCurrentCmd.Cmd, mConsoleHistoric[mHistoricCount-(--mCurrentHistoric+1)].Cmd, COMMAND_MAX_LENGTH);
	}
}

//////////////////////////////////////////////////////////////////////////
void Console::GetParams(char*& params)
{
	int currentChar = 3;	// we ignore the 3 characters ">: "
	int paramLength = 0;
	while( true )
	{
		if (mCurrentCmd.Cmd[currentChar] == ' ' || mCurrentCmd.Cmd[currentChar] == nullchar)
			break;
		++currentChar;
	}
	if (currentChar == strlen(mCurrentCmd.Cmd))
		return;

	paramLength = (int)strlen(mCurrentCmd.Cmd) - currentChar;
	params = new char[paramLength];
	for (int i=0; i<paramLength-1; ++i)
		params[i] = mCurrentCmd.Cmd[++currentChar];
	params[paramLength-1]=nullchar;
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
	if (strlen(mCurrentCmd.Cmd)<=3)
		return;

	f_ptr handle = IsValidCommand();
	if (handle == nullptr)
		DisplayError();
	else
	{
		char* params = nullptr;
		GetParams(params);
		// We execute the command
		handle(params);
		RJE_SAFE_DELETE(params);
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
	mLineCount         = 1;

	ConcatText("RamJam Engine Runtime Console", CONSOLE_GREEN);
}

//////////////////////////////////////////////////////////////////////////
void Console::ConcatText(const char* text, u8 color)
{
	int newSize = (int)strlen(text);
	if (mConsoleBufferSize + newSize > COMMAND_MAX_LENGTH*LINE_MAX - 3)
		return;

	mConsoleBuffer[mConsoleBufferSize] = (char)color;
	memcpy(mConsoleBuffer+mConsoleBufferSize+1, text, newSize);
	mConsoleBuffer[mConsoleBufferSize+newSize+1] = (char)CONSOLE_WHITE;
	mConsoleBuffer[mConsoleBufferSize+newSize+2] = nullchar;
	mConsoleBufferSize += newSize+2;
}

//////////////////////////////////////////////////////////////////////////
void Console::ConcatCommand()
{
	if (strlen(mCurrentCmd.Cmd) == 3)
	{
		mConsoleBuffer[mConsoleBufferSize++] = newline;
		mConsoleBuffer[mConsoleBufferSize]   = nullchar;
		AddLine();
		return;
	}

	// We backup the command in the historic
	if (mHistoricCount == HISTORIC_LENGTH)
	{
		for (int i=0 ; i<HISTORIC_LENGTH-1 ; ++i)
		{
			memcpy(mConsoleHistoric[i].Cmd, mConsoleHistoric[i+1].Cmd, COMMAND_MAX_LENGTH);
		}
		--mHistoricCount;
	}
	memcpy(mConsoleHistoric[mHistoricCount++].Cmd, mCurrentCmd.Cmd, COMMAND_MAX_LENGTH);
	mCurrentHistoric = -1;

	if (mConsoleBufferSize > 0)
	{
		mConsoleBuffer[mConsoleBufferSize++] = newline;
		AddLine();
	}

	int cmdSize = (int)strlen(mCurrentCmd.Cmd);
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
void Console::AddLine()
{
	++mLineCount;

	if (mLineCount >= LINE_MAX)
		RemoveFirstLine();
}

//////////////////////////////////////////////////////////////////////////
void Console::RemoveCharacter()
{
	if (mCurrentCmdLength < 4)
		return;

	mCurrentCmd.Cmd[--mCurrentCmdLength] = nullchar;
}

//////////////////////////////////////////////////////////////////////////
void Console::RemoveFirstLine()
{
	if (mLineCount<2)
		return;

	// We search for the newline character
	for (int i=0 ; i<COMMAND_MAX_LENGTH*LINE_MAX ; ++i)
	{
		if (mConsoleBuffer[i] == newline)
		{
			memcpy(mConsoleBuffer, mConsoleBuffer+i+1, COMMAND_MAX_LENGTH*LINE_MAX-(i+1));
			mConsoleBufferSize -= (i+1);
			--mLineCount;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
f_ptr Console::IsValidCommand()
{
	int cmdSize = (int)strlen(mCurrentCmd.Cmd);
	RJE_ASSERT(cmdSize > 3);

	char* cmd = new char[cmdSize+1];

	int i=3;	// we ignore the 3 characters ">: "
	while( true )
	{
		if (mCurrentCmd.Cmd[i] == ' ' || mCurrentCmd.Cmd[i] == nullchar)
		{
			cmd[i-3] = nullchar;
			break;
		}
		cmd[i-3] = mCurrentCmd.Cmd[i];
		++i;
	}

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
	RJE_SAFE_DELETE(cmd);
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void Console::DisplayError()
{
	// We display an error formatted as such : [    2"command" : command not recognized. Syntax error1\0]
	mConsoleBuffer[mConsoleBufferSize++] = newline;
	AddLine();

	for (int i=0; i<4; ++i)
		mConsoleBuffer[mConsoleBufferSize++] = ' ';
	mConsoleBuffer[mConsoleBufferSize++] = 2;	// display in red the following text
	mConsoleBuffer[mConsoleBufferSize++] = '"';

	int cmdSize =(int) strlen(mCurrentCmd.Cmd);
	for (int i=3; i<cmdSize; ++i)
	{
		if (mCurrentCmd.Cmd[i] == ' ')
			break;
		mConsoleBuffer[mConsoleBufferSize++] = mCurrentCmd.Cmd[i];
	}

	ConcatText("\" : command not recognized. Syntax error", CONSOLE_RED);

	mConsoleBuffer[mConsoleBufferSize++] = 1;
	mConsoleBuffer[mConsoleBufferSize]   = nullchar;
}


//////////////////////////////////////////////////////////////////////////
/// COMMANDS
//////////////////////////////////////////////////////////////////////////

// ====== Console ======
void CleanConsole(char* command /* = nullptr */) { Console::Instance()->ClearConsole(); }


// ======= Core =======
void Exit(char* command /* = nullptr */)
{
	SendMessage(System::Instance()->mHWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
}


// ===== Rendering =====
void SetWireframe(char* command /* = nullptr */)
{
	if (command == nullptr)
	{
		Console::Instance()->ConcatText(" Command usage : wireframe [0|1]", CONSOLE_WHITE);
		return;
	}
		
	if (strlen(command) == 1)
	{
		BOOL bState = (command[0] == '0') || (command[0] == 'n') || (command[0] == 'N');
		System::Instance()->mGraphicAPI->SetWireframe( !bState );
	}
	else Console::Instance()->ConcatText(" -> Bad Parameter. Command usage : wireframe [0|1]", CONSOLE_WHITE);
}


// ======= Time =======
void SetTimeScale(char* command /* = nullptr */)
{
	if (command == nullptr)
	{
		Console::Instance()->ConcatText(" command usage : timescale \"value\"", CONSOLE_WHITE);
		return;
	}
	
	float scale = (float)atof(command);
	if (scale < 0 || scale > 100)
	{
		Console::Instance()->ConcatText(" -> Bad Parameter : timescale \"value\" [0-100]", CONSOLE_WHITE);
		return;
	}

	Timer::Instance()->SetTimeScale(scale);
}

//----------------------------------------
void GetTime(char* command /* = nullptr */)
{
	if (command != nullptr)
	{
		Console::Instance()->ConcatText(" command usage : time", CONSOLE_WHITE);
		return;
	}

	char buf[_CVTBUFSIZE];
	dtoa(buf, _CVTBUFSIZE, Timer::Instance()->Time(), 20);
	
	Console::Instance()->ConcatText(" -> current time : ", CONSOLE_WHITE);
	Console::Instance()->ConcatText(buf, CONSOLE_WHITE);
}
