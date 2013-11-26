#pragma once

#include "Types.h"
#include "Globals.h"
#include "Memory.h"

#define KEYBOARD_INPUTS		96
#define MOUSE_INPUTS		5

enum Keycode		// Taken from the Virtual-Key codes from Microsoft
{
	A, B, C, D, E, F, G, H, I, J, K, L, M,
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Add,
	Backspace,
	CapsLock,
	Clear,
	Decimal,
	Delete,
	DownArrow,
	Divide,
	End,
	Esc,
	Execute,
	F1, F2, F3, F4,
	F5, F6, F7, F8,
	F9, F10, F11, F12,
	Help,
	Home,
	Insert,
	Keyboard0,
	Keyboard1,
	Keyboard2,
	Keyboard3,
	Keyboard4,
	Keyboard5,
	Keyboard6,
	Keyboard7,
	Keyboard8,
	Keyboard9,
	LeftAlt,
	LeftArrow,
	LeftCtrl,
	LeftShift,
	Multiply,
	NumLock,
	Numpad0,
	Numpad1,
	Numpad2,
	Numpad3,
	Numpad4,
	Numpad5,
	Numpad6,
	Numpad7,
	Numpad8,
	Numpad9,
	PageDown,
	PageUp,
	Pause,
	Print,
	Return,
	RightArrow,
	RightAlt,
	RightCtrl,
	RightShift,
	Scroll,
	Select,
	Separator,
	Snapshot,
	Spacebar,
	Subtract,
	Tab,
	UpArrow,
	Windows
};

enum MouseCode
{
	LButton = 0,
	RButton,
	MButton,
	WheelUp,
	WheelDown
};

enum MultipleKeyboardKeys
{
	FUNCTION_KEYS		= 1,
	NUMPAD_KEYS			= 2,
	KEYBOARD_NUM_KEYS	= 4,
	ALPHA_KEYS			= 8
};

struct Input
{
	static Input* Instance()
	{
		if(!sInstance)
			sInstance = rje_new Input();

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

	void HandleInputEvent(UINT umsg, WPARAM wparam, LPARAM lparam);

	BOOL IsMouseMoving();
	int  GetMousePosX();
	int  GetMousePosY();

	BOOL GetKeyboard    (Keycode code);
	BOOL GetKeyboardUp  (Keycode code);
	BOOL GetKeyboardDown(Keycode code);
	//----
	BOOL GetKeyboard    (int keys);
	BOOL GetKeyboardUp  (int keys);
	BOOL GetKeyboardDown(int keys);
	//----
	BOOL GetKeyboardAny();
	BOOL GetKeyboardAnyUp();
	BOOL GetKeyboardAnyDown();
	//----
	BOOL GetMouseButton    (MouseCode code);
	BOOL GetMouseButtonUp  (MouseCode code);
	BOOL GetMouseButtonDown(MouseCode code);
	//----
	BOOL GetMouseButtonAny();
	BOOL GetMouseButtonAnyUp();
	BOOL GetMouseButtonAnyDown();
	//----
	void ResetInputStates();

private:
	Input();

	static Input* sInstance;

	BOOL bUpdated;

	int  mMousePosX;
	int  mMousePosY;
	BOOL bMouseMoving;
	
	BOOL mKeyboardState    [KEYBOARD_INPUTS];
	BOOL mKeyboardStateDown[KEYBOARD_INPUTS];
	BOOL mKeyboardStateUp  [KEYBOARD_INPUTS];
	BOOL mMouseState    [MOUSE_INPUTS];
	BOOL mMouseStateDown[MOUSE_INPUTS];
	BOOL mMouseStateUp  [MOUSE_INPUTS];
};