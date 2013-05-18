#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
System::System() {}
System::System(const System& other)	{}
System::~System(){}

//////////////////////////////////////////////////////////////////////////
BOOL System::Initialize(int nCmdShow)
{
	HACCEL hAccelTable;

	sInstance = this;

	// Initialize global strings
	LoadString(mHInst, IDS_APP_TITLE, mSzTitle, MAX_LOADSTRING);
	LoadString(mHInst, IDC_RAMJAMENGINE, mSzWindowClass, MAX_LOADSTRING);
	hAccelTable = LoadAccelerators(mHInst, MAKEINTRESOURCE(IDC_RAMJAMENGINE));

	RegisterMyClass(mHInst);
	if (!InitializeWindows(nCmdShow))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
void System::Shutdown()
{
	ShutdownWindows();

	return;
}

//////////////////////////////////////////////////////////////////////////
void System::Launch()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	// Enter the infinite message loop
	while(TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);
			// send the message to the WindowProc function
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				break;
		}
		else
		{
			if (!Update())	// If the Update fails, we quit the program
				break;
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
		{
			return 0;
		}

		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
		{
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
	default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL System::Update()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::InitializeWindows(int nCmdShow)
{
	int posX, posY;
	int screenWidth, screenHeight;
	
#if FULL_SCREEN == 1
	DEVMODE dmScreenSettings;
	screenWidth		= GetSystemMetrics(SM_CXSCREEN);
	screenHeight	= GetSystemMetrics(SM_CYSCREEN);
	posX = posY = 0;

	// If full screen set the screen to maximum size of the users desktop and 32bit.
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
	dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
	dmScreenSettings.dmBitsPerPel = 32;			
	dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// Change the display settings to full screen.
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
#else
	screenHeight	= SCREEN_HEIGHT;
	screenWidth		= SCREEN_WIDTH;
	posX = (GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH)  / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2;

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
#endif // FULL_SCREEN

	// Create the window with the screen settings and get the handle to it.
	mHWnd = CreateWindowEx(	WS_EX_APPWINDOW,			\
								sInstance->mSzWindowClass,	\
								sInstance->mSzTitle,		\
								dwStyle,					\
								posX,						\
								posY,						\
								screenWidth,				\
								screenHeight,				\
								NULL,						\
								NULL,						\
								sInstance->mHInst,			\
								NULL);
	if (!mHWnd)
		return FALSE;

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mHWnd, nCmdShow);
	UpdateWindow(mHWnd);
	SetForegroundWindow(mHWnd);
	SetFocus(mHWnd);
	ShowCursor(SHOW_CURSOR);

	UpdateWindow(mHWnd);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
ATOM System::RegisterMyClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAMJAMENGINE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= mSzWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.cbSize			= sizeof(WNDCLASSEX);

#if FULL_SCREEN == 1
	wcex.lpszMenuName	= NULL;
#else
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAMJAMENGINE);
#endif // !FULL_SCREEN

	return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////////
void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

#if FULL_SCREEN == 1
	// Fix the display settings if leaving full screen mode.
	ChangeDisplaySettings(NULL, 0);
#endif // FULL_SCREEN

	// Remove the window.
	DestroyWindow(mHWnd);
	mHWnd = NULL;

	// Remove the application instance.
	UnregisterClass(mSzWindowClass, mHInst);
	mHInst = NULL;

	// Release the pointer to this class.
	sInstance = nullptr;

	return;
}

//////////////////////////////////////////////////////////////////////////
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(sInstance->mHInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return sInstance->MessageHandler(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
