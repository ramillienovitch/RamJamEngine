#include "stdafx.h"

System* System::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
System::System()
{
	sInstance = this;

	mAppPaused = false;
	mMinimized = false;
	mMaximized = false;
	mResizing  = false;

	fps    = 0.0f;
	mspf   = 0.0f;
	minfps = 1e9f;
	maxfps = 0.0f;

	mTotalFrames = 0;

	mCpuUsage  = -1;
	mDwLastRun = 0;
	mRunCount  = 0;

	// ===================================
	int CPUInfo[4] = {-1};
	unsigned nExIds;
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (unsigned i=0x80000000; i<=nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)			memcpy(mCpuDescription,      CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)		memcpy(mCpuDescription + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)		memcpy(mCpuDescription + 32, CPUInfo, sizeof(CPUInfo));
	}
	// We remove all white spaces before the cpu description
	int i = 0;
	while(mCpuDescription[i] == ' ') { ++i; }
	memcpy(mCpuDescription, mCpuDescription+i, 64-i);

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx(&statex);
	mTotalSystemRAM = (statex.ullTotalPhys/1024)/1024;
	mProcessID = ::GetCurrentProcessId();
	// ===================================

	mLastMousePos.x  = 0;
	mLastMousePos.y  = 0;
	mCameraTheta	 = 1.5f*RJE::Math::Pi_f;
	mCameraPhi		 = 0.25f*RJE::Math::Pi_f;
	mCameraRadius	 = 10.0f;

	mCameraAnimated = false;
	mAnimationSpeed = 10.0f;


#if (RJE_GRAPHIC_API == DIRECTX_11)
	mGraphicAPI = rje_new DX11RenderingAPI(mScene);
#else
	mGraphicAPI = rje_new OglWrapper();
#endif
	mGraphicAPI->mCamera = rje_new Camera();
}
System::~System(){}

//////////////////////////////////////////////////////////////////////////
BOOL System::Initialize(int nCmdShow)
{
	HACCEL hAccelTable;

	mResourcesPath = "../data/Resources.ini";
	mDataPath = CIniFile::GetValue("datapath", "repositories", mResourcesPath);

	// Initialize all the globals defined in the .ini files
	LoadConfigFile();

	// Initialize global strings
	LoadString(mHInst, IDS_APP_TITLE, mSzTitle, 100);
	LoadString(mHInst, IDC_RAMJAMENGINE, mSzWindowClass, 100);
	hAccelTable = LoadAccelerators(mHInst, MAKEINTRESOURCE(IDC_RAMJAMENGINE));

	RegisterMyClass(mHInst);
	RJE_ASSERT(InitializeWindows(nCmdShow));

	// Load the Scene file
	string scenePath = mDataPath + CIniFile::GetValue("simple", "scenes", mResourcesPath);
	mScene.LoadFromFile(scenePath.c_str());

	// We launch DirectX or OpenGL
	mGraphicAPI->Initialize(RJE_GLOBALS::gScreenWidth, RJE_GLOBALS::gScreenHeight);

	return true;
}

//////////////////////////////////////////////////////////////////////////
void System::Shutdown()
{
	ShutdownWindows();

	RJE_SAFE_DELETE(mGraphicAPI->mCamera);
	mGraphicAPI->Shutdown();
	RJE_SAFE_DELETE(mGraphicAPI);

	Timer::   DeleteInstance();
	Input::   DeleteInstance();
	Console:: DeleteInstance();
	Profiler::DeleteInstance();
}

//////////////////////////////////////////////////////////////////////////
void System::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Timer::Instance()->Start();

	float processRefreshRate = 0.0f;	// the refresh rate in seconds for measurements only

	// Enter the infinite message loop
	while(msg.message != WM_QUIT)
	{
		// Check to see if any messages are waiting in the queue
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		// translate keystroke messages into the right format
			DispatchMessage (&msg);		// send the message to the WindowProc function
		}
		else	// Otherwise, do animation/game stuff.
		{
			processRefreshRate += Timer::Instance()->RealDeltaTime();
			if (processRefreshRate > 2)
			{
				mProcessCpuUsage = GetProcessCpuUsage();
				GetMemoryInfo(mProcessID);
				processRefreshRate = 0.0f;
			}

			FeedProfilerInfo();

			PROFILE_CPU("Frame");
			Timer::Instance()->Update();

			if (!Console::Instance()->IsActive())
				HandleInputs();

			if(mAppPaused)
				Sleep(50);
			else
			{
				CalculateFrameStats();
				UpdateScene(Timer::Instance()->DeltaTime());
				DrawScene();

				Console::Instance()->Update();
				Profiler::Instance()->Update();
				
				Input::Instance()->ResetInputStates();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void System::OnResize()
{
	mGraphicAPI->ResizeWindow(mScreenWidth, mScreenHeight);
	mGraphicAPI->mCamera->mSettings.AspectRatio = (float)mScreenWidth / (float)mScreenHeight;
	mGraphicAPI->mCamera->UpdateProjMatrix((float)mScreenWidth, (float)mScreenHeight);
}

//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	Input::Instance()->HandleInputEvent(umsg, wparam, lparam);

	switch(umsg)
	{	
	// WM_ACTIVATE is sent when the window is activated or deactivated.
	// We pause the game when the window is deactivated and unpause it
	// when it becomes active.
	case WM_ACTIVATE:
		if (RJE_GLOBALS::gRunInBackground)
			return 0;
		else
		{
			if( LOWORD(wparam) == WA_INACTIVE )
			{
				mAppPaused = true;
				Timer::Instance()->Stop();
			}
			else
			{
				mAppPaused = false;
				Timer::Instance()->Start();
			}
			return 0;
		}

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		Timer::Instance()->Stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		Timer::Instance()->Start();
		OnResize();
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
	}
	
	// Any other messages send to the default message handler as our application won't make use of them.
	return DefWindowProc(hwnd, umsg, wparam, lparam);	
}

//////////////////////////////////////////////////////////////////////////
BOOL System::UpdateScene(float dt)
{
	PROFILE_CPU("Update Scene");
	mGraphicAPI->UpdateScene(dt);
	return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::DrawScene()
{
	PROFILE_CPU("Draw Scene");
	mGraphicAPI->DrawScene();
	return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::InitializeWindows(int nCmdShow)
{
	int posX, posY;
	DWORD dwStyle = 0;

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

		dwStyle = WS_EX_TOPMOST | WS_POPUP;
	}
	else
	{
		mScreenHeight	= RJE_GLOBALS::gScreenHeight;
		mScreenWidth	= RJE_GLOBALS::gScreenWidth;
		posX = (GetSystemMetrics(SM_CXSCREEN) - RJE_GLOBALS::gScreenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - RJE_GLOBALS::gScreenHeight) / 2;

		dwStyle = WS_OVERLAPPEDWINDOW;
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

	return;
}

//////////////////////////////////////////////////////////////////////////
void System::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	++frameCnt;
	++mTotalFrames;

	// Compute averages over one second period.
	if( (Timer::Instance()->Time() - timeElapsed) >= 1.0f )
	{
		fps = (float)frameCnt;
		mspf = 1000.0f / fps;
		if (minfps > fps)	minfps = fps;
		if (maxfps < fps)	maxfps = fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << sInstance->mSzTitle << L"    "
//			<< L"Frame N° "         << mTotalFrames
			<< L" FPS: "            << fps
			<< L" ("                << minfps
			<< L"-"                 << maxfps
			<< L")    Frame Time: " << mspf << L" (ms)";
		SetWindowText(mHWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

//////////////////////////////////////////////////////////////////////////
void System::HandleInputs()
{
	int x = Input::Instance()->GetMousePosX();
	int y = Input::Instance()->GetMousePosY();
	
	if (Input::Instance()->GetKeyboardDown(Spacebar))
		mCameraAnimated = !mCameraAnimated;

	if (Input::Instance()->GetKeyboardDown(Return))
	{
		mGraphicAPI->mCamera->SetCameraOrtho(!mGraphicAPI->mCamera->IsOrtho());
		mGraphicAPI->mCamera->UpdateProjMatrix((float)mScreenWidth, (float)mScreenHeight);
	}

	if (mCameraAnimated)
		mCameraTheta = RJE::Math::Deg2Rad_f*mAnimationSpeed*Timer::Instance()->Time();

	// Wheel Scroll
	if (Input::Instance()->GetMouseButtonDown(WheelUp))
	{
		if (Input::Instance()->GetKeyboard(LeftShift) || Input::Instance()->GetKeyboard(RightShift))
			mCameraRadius -= 1.0f;
		else
			mCameraRadius -= 0.25f;
	}
	if (Input::Instance()->GetMouseButtonDown(WheelDown))
	{
		if (Input::Instance()->GetKeyboard(LeftShift) || Input::Instance()->GetKeyboard(RightShift))
			mCameraRadius += 1.0f;
		else
			mCameraRadius += 0.25f;
	}
	mCameraRadius = RJE::Math::Clamp(mCameraRadius, 2.0f, 500.0f);

	// Mouse Buttons
	if (Input::Instance()->GetMouseButtonAnyDown())		SetCapture(mHWnd);
	if (Input::Instance()->GetMouseButtonAnyUp())		ReleaseCapture();

	if (Input::Instance()->IsMouseMoving())
	{
		if (Input::Instance()->GetKeyboard(LeftCtrl))
		{
			if( Input::Instance()->GetMouseButton(RButton) )
			{
				if (mGraphicAPI->mCamera->IsOrtho())
				{
					// Make each pixel correspond to 0.001 unit in the scene.
					float dx = 0.001f*static_cast<float>(x - mLastMousePos.x);

					mGraphicAPI->mCamera->mSettings.OrthoZoom += dx;
					mGraphicAPI->mCamera->mSettings.OrthoZoom = RJE::Math::Clamp(mGraphicAPI->mCamera->mSettings.OrthoZoom, 0.0001f, 10000.0f);
					mGraphicAPI->mCamera->UpdateProjMatrix((float)mScreenWidth, (float)mScreenHeight);
				}
				else
				{
					// Make each pixel correspond to 0.05 unit in the scene.
					float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);

					mGraphicAPI->mCamera->mSettings.FOV += dx;
					mGraphicAPI->mCamera->mSettings.FOV = RJE::Math::Clamp(mGraphicAPI->mCamera->mSettings.FOV, 1.0f, 179.0f);
					mGraphicAPI->mCamera->UpdateProjMatrix((float)mScreenWidth, (float)mScreenHeight);
				}
			}
		}
		else
		{
			if( Input::Instance()->GetMouseButton(LButton) )
			{
				// Make each pixel correspond to a quarter of a degree.
				float dx = RJE::Math::Deg2Rad_f*0.25f*static_cast<float>(x - mLastMousePos.x);
				float dy = RJE::Math::Deg2Rad_f*0.25f*static_cast<float>(y - mLastMousePos.y);

				// Update angles based on input to orbit camera around box.
				if (!mCameraAnimated)
					mCameraTheta -= dx;

				mCameraPhi -= dy;
				mCameraPhi = RJE::Math::Clamp(mCameraPhi, 0.1f, RJE::Math::Pi_f-0.1f);
			}
			else if( Input::Instance()->GetMouseButton(RButton) )
			{
				// Make each pixel correspond to 0.05 unit in the scene.
				float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);

				// Update the camera radius based on input.
				mCameraRadius += dx;
				mCameraRadius = RJE::Math::Clamp(mCameraRadius, 0.5f, 100.0f);
			}
		}

		mLastMousePos.x = x;
		mLastMousePos.y = y;
	}
}

//////////////////////////////////////////////////////////////////////////
void System::LoadConfigFile()
{
	const char* filename = "../data/Config.ini";
	std::ifstream iFile(filename);
	if (!iFile)
	{
		CIniFile::Create(filename);

		CIniFile::SetValue("fullscreen",           "false", "rendering", filename);
		CIniFile::SetValue("screenwidth",          "1280",  "rendering", filename);
		CIniFile::SetValue("screenheight",         "720",   "rendering", filename);
		//---------------
		CIniFile::SetValue("runinbackground",     "true", "misc", filename);
		//---------------
		CIniFile::SetValue("debugverbosity", "0",    "debug", filename);
		CIniFile::SetValue("showcursor",     "true", "debug", filename);
	}
	RJE_GLOBALS::gFullScreen			= CIniFile::GetValueBool("fullscreen",          "rendering", filename);
	RJE_GLOBALS::gScreenWidth			= CIniFile::GetValueInt("screenwidth",          "rendering", filename);
	RJE_GLOBALS::gScreenHeight			= CIniFile::GetValueInt("screenheight",         "rendering", filename);
	//---------------
	RJE_GLOBALS::gRunInBackground		= CIniFile::GetValueBool("runinbackground",    "misc", filename);
	//---------------
	RJE_GLOBALS::gDebugVerbosity		= CIniFile::GetValueInt("debugverbosity",  "debug", filename);
	RJE_GLOBALS::gShowCursor			= CIniFile::GetValueBool("showcursor",     "debug", filename);
}

//////////////////////////////////////////////////////////////////////////
i16 System::GetProcessCpuUsage()
{
	//create a local copy to protect against race conditions in setting the member variable
	short nCpuCopy = mCpuUsage;
	if (::InterlockedIncrement(&mRunCount) == 1)
	{
		//If this is called too often, the measurement itself will greatly affect the results.
		if (!EnoughTimePassed())
		{
			::InterlockedDecrement(&mRunCount);
			return nCpuCopy;
		}

		FILETIME ftSysIdle, ftSysKernel, ftSysUser;
		FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;

		if (!GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser))
		{
			::InterlockedDecrement(&mRunCount);
			return nCpuCopy;
		}

		if (!IsFirstRun())
		{
			// CPU usage is calculated by getting the total amount of time the system has operated
			// since the last measurement (made up of kernel + user) and the total
			// amount of time the process has run (kernel + user).
			ULONGLONG ftSysKernelDiff = SubtractTimes(ftSysKernel, mFtPrevSysKernel);
			ULONGLONG ftSysUserDiff   = SubtractTimes(ftSysUser,   mFtPrevSysUser);

			ULONGLONG ftProcKernelDiff = SubtractTimes(ftProcKernel, mFtPrevProcKernel);
			ULONGLONG ftProcUserDiff   = SubtractTimes(ftProcUser,   mFtPrevProcUser);

			ULONGLONG nTotalSys  = ftSysKernelDiff  + ftSysUserDiff;
			ULONGLONG nTotalProc = ftProcKernelDiff + ftProcUserDiff;

			if (nTotalSys > 0)
				mCpuUsage = (short)((100.0 * nTotalProc) / nTotalSys);
		}
		
		mFtPrevSysKernel  = ftSysKernel;
		mFtPrevSysUser    = ftSysUser;
		mFtPrevProcKernel = ftProcKernel;
		mFtPrevProcUser   = ftProcUser;
		
		mDwLastRun = GetTickCount64();

		nCpuCopy = mCpuUsage;
	}
	
	::InterlockedDecrement(&mRunCount);
	return nCpuCopy;
}

//////////////////////////////////////////////////////////////////////////
u64 System::SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
{
	LARGE_INTEGER a, b;
	a.LowPart  = ftA.dwLowDateTime;
	a.HighPart = ftA.dwHighDateTime;

	b.LowPart  = ftB.dwLowDateTime;
	b.HighPart = ftB.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}

//////////////////////////////////////////////////////////////////////////
BOOL System::EnoughTimePassed()
{
	const int minElapsedMS = 250;//milliseconds

	ULONGLONG dwCurrentTickCount = GetTickCount64();
	return (dwCurrentTickCount - mDwLastRun) > minElapsedMS; 
}

//////////////////////////////////////////////////////////////////////////
void System::GetMemoryInfo( DWORD processID )
{
	HANDLE hProcess;
	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
	if (hProcess == NULL)
		return;

	GetProcessMemoryInfo( hProcess, &mProcessMemoryCounters, sizeof(mProcessMemoryCounters));

	CloseHandle( hProcess );
}

//////////////////////////////////////////////////////////////////////////
void System::FeedProfilerInfo()
{
	Profiler::Instance()->mProfilerInfos->ProcessCpuUsage       = mProcessCpuUsage;
	Profiler::Instance()->mProfilerInfos->ProcessPeakWorkingSet = (i16)(mProcessMemoryCounters.PeakWorkingSetSize/ 1048576);
	Profiler::Instance()->mProfilerInfos->ProcessWorkingSet     = (i16)(mProcessMemoryCounters.WorkingSetSize/ 1048576);
}

//////////////////////////////////////////////////////////////////////////
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	// Send event message to AntTweakBar
	if (TwEventWin(hWnd, message, wParam, lParam))
		return 0; // Event has been handled by AntTweakBar
	
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(System::Instance()->mHInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
		return System::Instance()->MessageHandler(hWnd, message, wParam, lParam);
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
