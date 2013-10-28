#pragma once

#include "stdafx.h"

struct System
{
public:

	HWND		mHWnd;
	HINSTANCE	mHInst;					// current instance
	TCHAR		mSzTitle[100];			// The title bar text
	TCHAR		mSzWindowClass[100];	// the main window class name
	BOOL		mAppPaused;
	BOOL		mMinimized;
	BOOL		mMaximized;
	BOOL		mResizing;
	int			mScreenWidth;
	int			mScreenHeight;

	// Application Statistics
	float	fps;		// Frames Per Seconds
	float	mspf;		// MilliSeconds Per Frame
	float	minfps;
	float	maxfps;

	// GPU Infos
	WCHAR	mGpuDescription[128];
	SIZE_T	mGpuDedicatedVRAM;
	SIZE_T	mGpuSharedVRAM;

	// Rendering Statistics
	float mRenderingTime;


	POINT		mLastMousePos;
	float		mCameraTheta;
	float		mCameraPhi;
	float		mCameraRadius;
	float		mCameraFOV;
	float		mCameraOrthoZoom;
	float		mCameraNearZ;
	float		mCameraFarZ;

	BOOL		mCameraAnimated;
	float		mAnimationSpeed;

	GraphicAPI* mGraphicAPI;
	
	BOOL				Initialize(int);
	void				Shutdown();
	void				Run();
	void				OnResize();
	float				AspectRatio()const;
	LRESULT CALLBACK	MessageHandler(HWND, UINT, WPARAM, LPARAM);
	
	static System* Instance()
	{
		if(!sInstance)
			sInstance = new System();

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

protected:
	BOOL InitMainWindow();
	BOOL InitGraphicAPI();
	void CalculateFrameStats();

private:
	System();
	~System();

	static System* sInstance;

	BOOL UpdateScene(float dt);
	BOOL DrawScene();
	BOOL InitializeWindows(int);
	ATOM RegisterMyClass(HINSTANCE);
	void ShutdownWindows();

	void HandleInputs();
	void LoadCameraSettings();
	void LoadConfigFile();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
