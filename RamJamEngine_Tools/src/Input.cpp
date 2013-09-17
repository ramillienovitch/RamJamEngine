#include "Input.h"
#include "../../RamJamEngine/include/Console.h"

Input* Input::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
Input::Input()
{
	mMousePosX = 0;
	mMousePosY = 0;

	ResetInputStates();
	for(int iKey=0; iKey<KEYBOARD_INPUTS; ++iKey)		mKeyboardState[iKey]   = false;
	for(int iMouse=0; iMouse<MOUSE_INPUTS; ++iMouse)	mMouseState   [iMouse] = false;
}

//////////////////////////////////////////////////////////////////////////
void Input::HandleInputEvent(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	bUpdated = true;

	switch (umsg)
	{
	case WM_CHAR:
		{
			if (Console::Instance()->IsActive())
			{
				if (wparam == VK_ESCAPE || wparam == VK_BACK || wparam == VK_RETURN)
					return;

				Console::Instance()->AddCharacter((const char) wparam);
			}
			return;
		}
	case WM_KEYDOWN:
		{
			if (wparam >= 0x30 && wparam <= 0x39)		// Keyboard 0 -> 9
			{
				int key = (int)(0x39-wparam);
				mKeyboardState    [Keycode::Keyboard9 - key] = true;
				mKeyboardStateDown[Keycode::Keyboard9 - key] = true;
				return;
			}
			else if (wparam >= 0x41 && wparam <= 0x5A)	// A -> Z
			{
				int key = (int)(0x5A-wparam);
				mKeyboardState    [Keycode::Z - key] = true;
				mKeyboardStateDown[Keycode::Z - key] = true;
				return;
			}
			else if (wparam >= 0x60 && wparam <= 0x69)	// Numpad 0 -> 9
			{
				int key = (int)(0x69-wparam);
				mKeyboardState    [Keycode::Numpad9 - key] = true;
				mKeyboardStateDown[Keycode::Numpad9 - key] = true;
				return;
			}
			else if (wparam >= 0x70 && wparam <= 0x7B)	// F1 -> F12
			{
				int key = (int)(0x7B-wparam);
				mKeyboardState    [Keycode::F1 - key]  = true;
				mKeyboardStateDown[Keycode::F12 - key] = true;
				return;
			}
			else
			{
				UINT scancode = (lparam & 0x00ff0000) >> 16;
				int extended  = (lparam & 0x01000000) != 0;
				switch (wparam) 
				{
				case VK_ADD :		mKeyboardState[Keycode::Add]          = true;	mKeyboardStateDown[Keycode::Add]          = true;		return;
				case VK_CAPITAL :	mKeyboardState[Keycode::CapsLock]     = true;	mKeyboardStateDown[Keycode::CapsLock]     = true;		return;
				case VK_CLEAR :		mKeyboardState[Keycode::Clear]        = true;	mKeyboardStateDown[Keycode::Clear]        = true;		return;
				case VK_DECIMAL :	mKeyboardState[Keycode::Decimal]      = true;	mKeyboardStateDown[Keycode::Decimal]      = true;		return;
				case VK_DELETE :	mKeyboardState[Keycode::Delete]       = true;	mKeyboardStateDown[Keycode::Delete]       = true;		return;
				case VK_DOWN :		mKeyboardState[Keycode::DownArrow]    = true;	mKeyboardStateDown[Keycode::DownArrow]    = true;		return;
				case VK_DIVIDE :	mKeyboardState[Keycode::Divide]       = true;	mKeyboardStateDown[Keycode::Divide]       = true;		return;
				case VK_END :		mKeyboardState[Keycode::End]          = true;	mKeyboardStateDown[Keycode::End]          = true;		return;
				case VK_EXECUTE :	mKeyboardState[Keycode::Execute]      = true;	mKeyboardStateDown[Keycode::Execute]      = true;		return;
				case VK_HELP :		mKeyboardState[Keycode::Help]         = true;	mKeyboardStateDown[Keycode::Help]         = true;		return;
				case VK_HOME :		mKeyboardState[Keycode::Home]         = true;	mKeyboardStateDown[Keycode::Home]         = true;		return;
				case VK_INSERT :	mKeyboardState[Keycode::Insert]       = true;	mKeyboardStateDown[Keycode::Insert]       = true;		return;
				case VK_LEFT :		mKeyboardState[Keycode::LeftArrow]    = true;	mKeyboardStateDown[Keycode::LeftArrow]    = true;		return;
				case VK_MULTIPLY :	mKeyboardState[Keycode::Multiply]     = true;	mKeyboardStateDown[Keycode::Multiply]     = true;		return;
				case VK_NUMLOCK :	mKeyboardState[Keycode::NumLock]      = true;	mKeyboardStateDown[Keycode::NumLock]      = true;		return;
				case VK_NEXT :		mKeyboardState[Keycode::PageDown]     = true;	mKeyboardStateDown[Keycode::PageDown]     = true;		return;
				case VK_PRIOR :		mKeyboardState[Keycode::PageUp]       = true;	mKeyboardStateDown[Keycode::PageUp]       = true;		return;
				case VK_PAUSE :		mKeyboardState[Keycode::Pause]        = true;	mKeyboardStateDown[Keycode::Pause]        = true;		return;
				case VK_PRINT :		mKeyboardState[Keycode::Print]        = true;	mKeyboardStateDown[Keycode::Print]        = true;		return;
				case VK_RIGHT :		mKeyboardState[Keycode::RightArrow]   = true;	mKeyboardStateDown[Keycode::RightArrow]   = true;		return;
				case VK_SCROLL :	mKeyboardState[Keycode::Scroll]       = true;	mKeyboardStateDown[Keycode::Scroll]       = true;		return;
				case VK_SELECT :	mKeyboardState[Keycode::Select]       = true;	mKeyboardStateDown[Keycode::Select]       = true;		return;
				case VK_SEPARATOR :	mKeyboardState[Keycode::Separator]    = true;	mKeyboardStateDown[Keycode::Separator]    = true;		return;
				case VK_SNAPSHOT :	mKeyboardState[Keycode::Snapshot]     = true;	mKeyboardStateDown[Keycode::Snapshot]     = true;		return;
				case VK_SPACE :		mKeyboardState[Keycode::Spacebar]     = true;	mKeyboardStateDown[Keycode::Spacebar]     = true;		return;
				case VK_SUBTRACT :	mKeyboardState[Keycode::Subtract]     = true;	mKeyboardStateDown[Keycode::Subtract]     = true;		return;
				case VK_TAB :		mKeyboardState[Keycode::Tab]          = true;	mKeyboardStateDown[Keycode::Tab]          = true;		return;
				case VK_UP :		mKeyboardState[Keycode::UpArrow]      = true;	mKeyboardStateDown[Keycode::UpArrow]      = true;		return;
				case VK_LWIN :		mKeyboardState[Keycode::Windows]      = true;	mKeyboardStateDown[Keycode::Windows]      = true;		return;
				case VK_CONTROL :
					if (extended)	{mKeyboardState[Keycode::RightCtrl]   = true;	mKeyboardStateDown[Keycode::RightCtrl]    = true;		return;}
					else			{mKeyboardState[Keycode::LeftCtrl]    = true;	mKeyboardStateDown[Keycode::LeftCtrl]     = true;		return;}
				case VK_MENU :
					if (extended)	{mKeyboardState[Keycode::RightAlt]    = true;	mKeyboardStateDown[Keycode::RightAlt]     = true;		return;}
					else			{mKeyboardState[Keycode::LeftAlt]     = true;	mKeyboardStateDown[Keycode::LeftAlt]      = true;		return;}
				case VK_SHIFT : 
					if (extended)	{mKeyboardState[Keycode::RightShift]  = true;	mKeyboardStateDown[Keycode::RightShift]   = true;		return;}
					else			{mKeyboardState[Keycode::LeftShift]   = true;	mKeyboardStateDown[Keycode::LeftShift]    = true;		return;}
				case VK_BACK :
					{
						mKeyboardState    [Keycode::Backspace] = true;
						mKeyboardStateDown[Keycode::Backspace] = true;
						if (Console::Instance()->IsActive())
							Console::Instance()->RemoveCharacter();
						return;
					}
				case VK_ESCAPE :
					{
						mKeyboardState    [Keycode::Esc] = true;
						mKeyboardStateDown[Keycode::Esc] = true;
						Console::Instance()->ToggleConsoleState();
						return;
					}
				case VK_RETURN :
					{
						mKeyboardState    [Keycode::Return] = true;
						mKeyboardStateDown[Keycode::Return] = true;
						if (Console::Instance()->IsActive())
							Console::Instance()->RegisterAndClearCommand();
						return;
					}
				}
			}
			return;
		}

	case WM_KEYUP:
		{
			if (wparam >= 0x30 && wparam <= 0x39)		// Keyboard 0 -> 9
			{
				int key = (int)(0x39-wparam);
				mKeyboardState  [Keycode::Keyboard9 - key] = false;
				mKeyboardStateUp[Keycode::Keyboard9 - key] = true;
				return;
			}
			else if (wparam >= 0x41 && wparam <= 0x5A)	// A -> Z
			{
				int key = (int)(0x5A-wparam);
				mKeyboardState  [Keycode::Z - key] = false;
				mKeyboardStateUp[Keycode::Z - key] = true;
				return;
			}
			else if (wparam >= 0x60 && wparam <= 0x69)	// Numpad 0 -> 9
			{
				int key = (int)(0x69-wparam);
				mKeyboardState  [Keycode::Numpad9 - key] = false;
				mKeyboardStateUp[Keycode::Numpad9 - key] = true;
				return;
			}
			else if (wparam >= 0x70 && wparam <= 0x7B)	// F1 -> F12
			{
				int key = (int)(0x7B-wparam);
				mKeyboardState  [Keycode::F1 - key]  = false;
				mKeyboardStateUp[Keycode::F12 - key] = true;
				return;
			}
			else
			{
				UINT scancode = (lparam & 0x00ff0000) >> 16;
				int extended  = (lparam & 0x01000000) != 0;
				switch (wparam) 
				{
				case VK_ADD :		mKeyboardState[Keycode::Add]          = false;	mKeyboardStateUp[Keycode::Add]          = true;		return;
				case VK_BACK :		mKeyboardState[Keycode::Backspace]    = false;	mKeyboardStateUp[Keycode::Backspace]    = true;		return;
				case VK_CAPITAL :	mKeyboardState[Keycode::CapsLock]     = false;	mKeyboardStateUp[Keycode::CapsLock]     = true;		return;
				case VK_CLEAR :		mKeyboardState[Keycode::Clear]        = false;	mKeyboardStateUp[Keycode::Clear]        = true;		return;
				case VK_DECIMAL :	mKeyboardState[Keycode::Decimal]      = false;	mKeyboardStateUp[Keycode::Decimal]      = true;		return;
				case VK_DELETE :	mKeyboardState[Keycode::Delete]       = false;	mKeyboardStateUp[Keycode::Delete]       = true;		return;
				case VK_DOWN :		mKeyboardState[Keycode::DownArrow]    = false;	mKeyboardStateUp[Keycode::DownArrow]    = true;		return;
				case VK_DIVIDE :	mKeyboardState[Keycode::Divide]       = false;	mKeyboardStateUp[Keycode::Divide]       = true;		return;
				case VK_END :		mKeyboardState[Keycode::End]          = false;	mKeyboardStateUp[Keycode::End]          = true;		return;
				case VK_ESCAPE :	mKeyboardState[Keycode::Esc]          = false;	mKeyboardStateUp[Keycode::Esc]          = true;		return;
				case VK_EXECUTE :	mKeyboardState[Keycode::Execute]      = false;	mKeyboardStateUp[Keycode::Execute]      = true;		return;
				case VK_HELP :		mKeyboardState[Keycode::Help]         = false;	mKeyboardStateUp[Keycode::Help]         = true;		return;
				case VK_HOME :		mKeyboardState[Keycode::Home]         = false;	mKeyboardStateUp[Keycode::Home]         = true;		return;
				case VK_INSERT :	mKeyboardState[Keycode::Insert]       = false;	mKeyboardStateUp[Keycode::Insert]       = true;		return;
				case VK_LEFT :		mKeyboardState[Keycode::LeftArrow]    = false;	mKeyboardStateUp[Keycode::LeftArrow]    = true;		return;
				case VK_MULTIPLY :	mKeyboardState[Keycode::Multiply]     = false;	mKeyboardStateUp[Keycode::Multiply]     = true;		return;
				case VK_NUMLOCK :	mKeyboardState[Keycode::NumLock]      = false;	mKeyboardStateUp[Keycode::NumLock]      = true;		return;
				case VK_NEXT :		mKeyboardState[Keycode::PageDown]     = false;	mKeyboardStateUp[Keycode::PageDown]     = true;		return;
				case VK_PRIOR :		mKeyboardState[Keycode::PageUp]       = false;	mKeyboardStateUp[Keycode::PageUp]       = true;		return;
				case VK_PAUSE :		mKeyboardState[Keycode::Pause]        = false;	mKeyboardStateUp[Keycode::Pause]        = true;		return;
				case VK_PRINT :		mKeyboardState[Keycode::Print]        = false;	mKeyboardStateUp[Keycode::Print]        = true;		return;
				case VK_RETURN :	mKeyboardState[Keycode::Return]       = false;	mKeyboardStateUp[Keycode::Return]       = true;		return;
				case VK_RIGHT :		mKeyboardState[Keycode::RightArrow]   = false;	mKeyboardStateUp[Keycode::RightArrow]   = true;		return;
				case VK_SCROLL :	mKeyboardState[Keycode::Scroll]       = false;	mKeyboardStateUp[Keycode::Scroll]       = true;		return;
				case VK_SELECT :	mKeyboardState[Keycode::Select]       = false;	mKeyboardStateUp[Keycode::Select]       = true;		return;
				case VK_SEPARATOR :	mKeyboardState[Keycode::Separator]    = false;	mKeyboardStateUp[Keycode::Separator]    = true;		return;
				case VK_SNAPSHOT :	mKeyboardState[Keycode::Snapshot]     = false;	mKeyboardStateUp[Keycode::Snapshot]     = true;		return;
				case VK_SPACE :		mKeyboardState[Keycode::Spacebar]     = false;	mKeyboardStateUp[Keycode::Spacebar]     = true;		return;
				case VK_SUBTRACT :	mKeyboardState[Keycode::Subtract]     = false;	mKeyboardStateUp[Keycode::Subtract]     = true;		return;
				case VK_TAB :		mKeyboardState[Keycode::Tab]          = false;	mKeyboardStateUp[Keycode::Tab]          = true;		return;
				case VK_UP :		mKeyboardState[Keycode::UpArrow]      = false;	mKeyboardStateUp[Keycode::UpArrow]      = true;		return;
				case VK_LWIN :		mKeyboardState[Keycode::Windows]      = false;	mKeyboardStateUp[Keycode::Windows]      = true;		return;
				case VK_CONTROL :
					if (extended)	{mKeyboardState[Keycode::RightCtrl]   = false;	mKeyboardStateUp[Keycode::RightCtrl]    = true;		return;}
					else			{mKeyboardState[Keycode::LeftCtrl]    = false;	mKeyboardStateUp[Keycode::LeftCtrl]     = true;		return;}
				case VK_MENU :
					if (extended)	{mKeyboardState[Keycode::RightAlt]    = false;	mKeyboardStateUp[Keycode::RightAlt]     = true;		return;}
					else			{mKeyboardState[Keycode::LeftAlt]     = false;	mKeyboardStateUp[Keycode::LeftAlt]      = true;		return;}
				case VK_SHIFT : 
					if (extended)	{mKeyboardState[Keycode::RightShift]  = false;	mKeyboardStateUp[Keycode::RightShift]   = true;		return;}
					else			{mKeyboardState[Keycode::LeftShift]   = false;	mKeyboardStateUp[Keycode::LeftShift]    = true;		return;}
				}
			}
			return;
		}

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)		mMouseStateDown[MouseCode::WheelUp]   = true;
		else										mMouseStateDown[MouseCode::WheelDown] = true;
		return;

	case WM_LBUTTONDOWN:
		mMouseState    [MouseCode::LButton] = true;
		mMouseStateDown[MouseCode::LButton] = true;
		return;
	case WM_MBUTTONDOWN:
		mMouseState    [MouseCode::MButton] = true;
		mMouseStateDown[MouseCode::MButton] = true;
		return;
	case WM_RBUTTONDOWN:
		mMouseState    [MouseCode::RButton] = true;
		mMouseStateDown[MouseCode::RButton] = true;
		return;
	case WM_LBUTTONUP:
		mMouseState  [MouseCode::LButton] = false;
		mMouseStateUp[MouseCode::LButton] = true;
		return;
	case WM_MBUTTONUP:
		mMouseState  [MouseCode::MButton] = false;
		mMouseStateUp[MouseCode::MButton] = true;
		return;
	case WM_RBUTTONUP:
		mMouseState  [MouseCode::RButton] = false;
		mMouseStateUp[MouseCode::RButton] = true;
		return;
	case WM_MOUSEMOVE:
		bMouseMoving = true;
		mMousePosX   = GET_X_LPARAM(lparam);
		mMousePosY   = GET_Y_LPARAM(lparam);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
bool Input::IsMouseMoving() {return bMouseMoving;}
int  Input::GetMousePosX()	{return mMousePosX;}
int  Input::GetMousePosY()	{return mMousePosY;}

//////////////////////////////////////////////////////////////////////////
bool Input::GetKeyboard(Keycode code)     {return mKeyboardState[code];}
bool Input::GetKeyboardUp(Keycode code)   {return mKeyboardStateUp[code];}
bool Input::GetKeyboardDown(Keycode code) {return mKeyboardStateDown[code];}

//////////////////////////////////////////////////////////////////////////
bool Input::GetKeyboardAny()
{
	for(int iKey=0; iKey<KEYBOARD_INPUTS; ++iKey)
		if (mKeyboardState[iKey]) return true;

	return false;
}
bool Input::GetKeyboardAnyUp()
{
	for(int iKey=0; iKey<KEYBOARD_INPUTS; ++iKey)
		if (mKeyboardStateUp[iKey]) return true;

	return false;
}
bool Input::GetKeyboardAnyDown()
{
	for(int iKey=0; iKey<KEYBOARD_INPUTS; ++iKey)
		if (mKeyboardStateDown[iKey]) return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
bool Input::GetMouseButton(MouseCode code)     {return mMouseState[code];}
bool Input::GetMouseButtonUp(MouseCode code)   {return mMouseStateUp[code];}
bool Input::GetMouseButtonDown(MouseCode code) {return mMouseStateDown[code];}

//////////////////////////////////////////////////////////////////////////
bool Input::GetMouseButtonAny()
{
	for(int iMouse=0; iMouse<MOUSE_INPUTS; ++iMouse)
		if (mMouseState[iMouse]) return true;

	return false;
}
bool Input::GetMouseButtonAnyUp()
{
	for(int iMouse=0; iMouse<MOUSE_INPUTS; ++iMouse)
		if (mMouseStateUp[iMouse]) return true;

	return false;
}
bool Input::GetMouseButtonAnyDown()
{
	for(int iMouse=0; iMouse<MOUSE_INPUTS; ++iMouse)
		if (mMouseStateDown[iMouse]) return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
void Input::ResetInputStates()
{
	if (bUpdated)
	{
		bMouseMoving = false;
		for(int iKey=0; iKey<KEYBOARD_INPUTS; ++iKey)
		{
			mKeyboardStateDown[iKey] = false;
			mKeyboardStateUp  [iKey] = false;
		}

		for(int iMouse=0; iMouse<MOUSE_INPUTS; ++iMouse)
		{
			mMouseStateDown[iMouse] = false;
			mMouseStateUp  [iMouse] = false;
		}
		bUpdated = false;
	}
}