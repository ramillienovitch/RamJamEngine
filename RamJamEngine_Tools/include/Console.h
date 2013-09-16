#pragma once

#include "Types.h"
#include "Globals.h"

//////////////////////////////////////////////////////////////////////////
struct Command { char Cmd[COMMAND_MAX_LENGTH]; };

/// The command prompt is formatted as such : >: [command]

//////////////////////////////////////////////////////////////////////////
struct Console
{
	~Console();

	static Console* Instance()
	{
		if(!sInstance)
			sInstance = new Console();

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
	u8		GetCommandLength();

	void	RegisterAndClearCommand();
	void	ClearCommand();
	void	ConcatText(const char* text, u8 size);
	void	ConcatCommand();

	BOOL	IsActive();
	void	ToggleConsoleState();

	void	AddCharacter(const char c);
	void	RemoveCharacter();

	char*	mConsoleBuffer;

private:
	Console();

	static Console* sInstance;

	BOOL		mIsActive;

	u8			mCurrentCmdLength;
	Command		mCurrentCmd;
	Command		mConsoleHistoric[HISTORIC_LENGTH];

	u8		mLineCount;				// how many lines we have to display in the console
	u8		mConsoleBufferSize;		// how many characters in the total buffer
};
