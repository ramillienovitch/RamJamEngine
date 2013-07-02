#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
System::System()
{
	mAppPaused = false;
	mMinimized = false;
	mMaximized = false;
	mResizing  = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
	mCameraTheta	= 1.5f*RJE::Math::Pi;
	mCameraPhi		= 0.25f*RJE::Math::Pi;
	mCameraRadius	= 5.0f;

#if (RJE_GRAPHIC_API == DIRECTX_11)
	mGraphicAPI = new DX11Wrapper();
#else
	mGraphicAPI = new OglWrapper();
#endif
}
System::~System(){}

//////////////////////////////////////////////////////////////////////////
BOOL System::Initialize(int nCmdShow)
{
	HACCEL hAccelTable;

	sInstance = this;

	// Initialize global strings
	LoadString(mHInst, IDS_APP_TITLE, mSzTitle, 100);
	LoadString(mHInst, IDC_RAMJAMENGINE, mSzWindowClass, 100);
	hAccelTable = LoadAccelerators(mHInst, MAKEINTRESOURCE(IDC_RAMJAMENGINE));

	RegisterMyClass(mHInst);
	RJE_ASSERT(InitializeWindows(nCmdShow));
	
	// We launch DirectX or OpenGL
	mGraphicAPI->Initialize(mHWnd, RJE_GLOBALS::gScreenWidth, RJE_GLOBALS::gScreenHeight);

	return true;
}

//////////////////////////////////////////////////////////////////////////
void System::Shutdown()
{
	ShutdownWindows();

	mGraphicAPI->Shutdown();

	RJE_SAFE_DELETE(mGraphicAPI);
}

//////////////////////////////////////////////////////////////////////////
void System::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	mTimer.Start();

	// Enter the infinite message loop
	while(msg.message != WM_QUIT)
	{
		// Check to see if any messages are waiting in the queue
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);
			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
		else	// Otherwise, do animation/game stuff.
		{
			mTimer.Update();

			if(mAppPaused)
				Sleep(50);
			else
			{
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime(), mCameraTheta, mCameraPhi, mCameraRadius);
				DrawScene();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void System::OnResize()
{
	mGraphicAPI->ResizeWindow(mScreenWidth, mScreenHeight);
}

//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	short zDelta=0;

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

	// WM_ACTIVATE is sent when the window is activated or deactivated.
	// We pause the game when the window is deactivated and unpause it
	// when it becomes active.  
	case WM_ACTIVATE:
		if( LOWORD(wparam) == WA_INACTIVE )
		{
			mAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.Start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mScreenWidth  = LOWORD(lparam);
		mScreenHeight = HIWORD(lparam);
		
		if( mGraphicAPI )
		{
			if( wparam == SIZE_MINIMIZED )
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if( wparam == SIZE_MAXIMIZED )
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if( wparam == SIZE_RESTORED )
			{

				// Restoring from minimized state?
				if( mMinimized )
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if( mMaximized )
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if( mResizing )
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		
		return 0;

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		mTimer.Stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		mTimer.Start();
		OnResize();
		return 0;

	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lparam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lparam)->ptMinTrackSize.y = 200; 
		return 0;

	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
		if (wparam & MK_SHIFT)
		{
			if (zDelta >= 0)	mCameraRadius -= 0.5f;
			else				mCameraRadius += 0.5f;
		}
		else
		{
			if (zDelta >= 0)	mCameraRadius -= 0.25f;
			else				mCameraRadius += 0.25f;
		}
		mCameraRadius = RJE::Math::Clamp(mCameraRadius, 3.0f, 15.0f);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;
	}
	
	// Any other messages send to the default message handler as our application won't make use of them.
	return DefWindowProc(hwnd, umsg, wparam, lparam);	
}

//////////////////////////////////////////////////////////////////////////
void System::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mHWnd);
}

//////////////////////////////////////////////////////////////////////////
void System::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

//////////////////////////////////////////////////////////////////////////
void System::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = DegreesToRadian(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = DegreesToRadian(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mCameraTheta -= dx;
		mCameraPhi   -= dy;

		// Restrict the angle mPhi.
		mCameraPhi = RJE::Math::Clamp(mCameraPhi, 0.1f, RJE::Math::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mCameraRadius += dx - dy;

		// Restrict the radius.
		mCameraRadius = RJE::Math::Clamp(mCameraRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::UpdateScene(float dt, float theta, float phi, float radius)
{
	mGraphicAPI->UpdateScene(dt, theta, phi, radius);
	return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::DrawScene()
{
	mGraphicAPI->DrawScene();
	return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::InitializeWindows(int nCmdShow)
{
	int posX, posY;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	if (RJE_GLOBALS::gFullScreen)
	{
		DEVMODE dmScreenSettings;
		mScreenWidth	= GetSystemMetrics(SM_CXSCREEN);
		mScreenHeight	= GetSystemMetrics(SM_CYSCREEN);
		posX = posY = 0;

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)mScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)mScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}
	else
	{
		mScreenHeight	= RJE_GLOBALS::gScreenHeight;
		mScreenWidth	= RJE_GLOBALS::gScreenWidth;
		posX = (GetSystemMetrics(SM_CXSCREEN) - RJE_GLOBALS::gScreenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - RJE_GLOBALS::gScreenHeight) / 2;

		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	}

	// Create the window with the screen settings and get the handle to it.
	mHWnd = CreateWindowEx(	WS_EX_APPWINDOW,			\
							sInstance->mSzWindowClass,	\
							sInstance->mSzTitle,		\
							dwStyle,					\
							posX,						\
							posY,						\
							mScreenWidth,				\
							mScreenHeight,				\
							NULL,						\
							NULL,						\
							sInstance->mHInst,			\
							NULL);
	if (!mHWnd)
	{
		return FALSE;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mHWnd, nCmdShow);
	SetForegroundWindow(mHWnd);
	SetFocus(mHWnd);
	ShowCursor(RJE_GLOBALS::gShowCursor);

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

	if (RJE_GLOBALS::gFullScreen)
		wcex.lpszMenuName	= NULL;
	else
		wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAMJAMENGINE);

	return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////////
void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	if (RJE_GLOBALS::gFullScreen)
		ChangeDisplaySettings(NULL, 0);	// Fix the display settings if leaving full screen mode.

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
void System::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.Time() - timeElapsed) >= 1.0f )
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << sInstance->mSzTitle << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mHWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
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