#pragma once

#include "Types.h"
#include "Debug.h"
#include "Globals.h"
#include "Memory.h"

typedef enum CONSOLE_STATE
{
	E_CONSOLE_UP   = 1,
	E_CONSOLE_DOWN = 2,
	E_CONSOLE_IDLE = 3
} CONSOLE_STATE;

//////////////////////////////////////////////////////////////////////////
typedef void (*f_ptr)(char*);

//////////////////////////////////////////////////////////////////////////
struct Command
{ 
	char Cmd[COMMAND_MAX_LENGTH];
};

/// The command prompt is formatted as such : >: [command]
/// the char 1 (0x01) [written in white] means the following text is in white
/// the char 2 (0x02) [written in red]   means there's an error in the command line (not recognized / syntax error)
/// the char 3 (0x03) [written in green] means it's a valid command

//////////////////////////////////////////////////////////////////////////
struct Console
{
	~Console();

	static Console* Instance()
	{
		if(!sInstance)
			sInstance = rje_new Console();

		return sInstance;
	}

	static void DeleteInstance()
	{
		if(sInstance)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	void	GetCommand(OUT char (&p)[COMMAND_MAX_LENGTH]);
	u16		GetCommandLength();
	void	GetCommandHistoric(BOOL upwards);
	void	GetParams(char*& params);
	u16		GetLineCount();
	//----------
	void	RegisterAndClearCommand();
	void	RegisterCommand();
	//----------
	void	ClearCommand();
	void	ClearConsole();
	//----------
	void	ConcatText(const char* text, u8 color);
	void	ConcatCommand();
	//----------
	BOOL	IsActive();
	void	ActivateConsole();
	void	ExitConsole();
	//----------
	void	AddCharacter(const char c);
	void	AddLine();
	void	RemoveCharacter();
	void	RemoveFirstLine();
	//----------
	void	Update();

	char*	mConsoleBuffer;

	// The position (y-axis) of the console (used for transition effect)
	float			mConsoleElevation;
	CONSOLE_STATE	mConsoleState;

private:
	Console();

	static Console* sInstance;

	// The commands that can be executed by the console 
	std::map<char*, f_ptr> CommandList;

	BOOL		mIsActive;

	u16			mCurrentCmdLength;
	Command		mCurrentCmd;
	Command		mConsoleHistoric[HISTORIC_LENGTH];

	u8		mHistoricCount;
	i8		mCurrentHistoric;
	u16		mLineCount;				// how many lines we have to display in the console
	u16		mConsoleBufferSize;		// how many characters in the total buffer

	f_ptr	IsValidCommand();
	void	DisplayError();
};


//////////////////////////////////////////////////////////////////////////
/// COMMANDS
//////////////////////////////////////////////////////////////////////////

// ------ Console ------
void CleanConsole(char* command = nullptr);
// ------- Core -------
void Exit        (char* command = nullptr);
// ----- Rendering -----
void SetWireframe        (char* command = nullptr);
void SetMSAA             (char* command = nullptr);
void InstantiateModel    (char* command = nullptr);
void InstantiatePrimitive(char* command = nullptr);
void LoadScene           (char* command = nullptr);
// ------- Time -------
void Time        (char* command = nullptr);
