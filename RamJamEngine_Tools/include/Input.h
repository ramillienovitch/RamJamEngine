#pragma once

#include "Types.h"
#include "Globals.h"

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

struct Input
{
	static Input* Instance()
	{
		if(!sInstance)
			sInstance = new Input();

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

	bool IsMouseMoving();
	int  GetMousePosX();
	int  GetMousePosY();

	bool GetKeyboard(Keycode code);
	bool GetKeyboardUp(Keycode code);
	bool GetKeyboardDown(Keycode code);
	bool GetKeyboardAny();
	bool GetKeyboardAnyUp();
	bool GetKeyboardAnyDown();
	bool GetMouseButton(MouseCode code);
	bool GetMouseButtonUp(MouseCode code);
	bool GetMouseButtonDown(MouseCode code);
	bool GetMouseButtonAny();
	bool GetMouseButtonAnyUp();
	bool GetMouseButtonAnyDown();

	void ResetInputStates();

private:
	Input();

	static Input* sInstance;

	bool bUpdated;

	int  mMousePosX;
	int  mMousePosY;
	bool bMouseMoving;
	
	bool mKeyboardState    [KEYBOARD_INPUTS];
	bool mKeyboardStateDown[KEYBOARD_INPUTS];
	bool mKeyboardStateUp  [KEYBOARD_INPUTS];
	bool mMouseState    [MOUSE_INPUTS];
	bool mMouseStateDown[MOUSE_INPUTS];
	bool mMouseStateUp  [MOUSE_INPUTS];
};