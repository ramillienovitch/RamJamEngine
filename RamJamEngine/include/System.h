#pragma once

#include "stdafx.h"

struct System
{
public:
	System();
	~System();

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

	POINT		mLastMousePos;
	float		mCameraTheta;
	float		mCameraPhi;
	float		mCameraRadius;

	GraphicAPI* mGraphicAPI;

	Timer mTimer;

	BOOL				Initialize(int);
	void				Shutdown();
	void				Run();
	void				OnResize();
	float				AspectRatio()const;
	LRESULT CALLBACK	MessageHandler(HWND, UINT, WPARAM, LPARAM);

	void OnMouseDown(WPARAM, int, int);
	void OnMouseUp(WPARAM, int, int);
	void OnMouseMove(WPARAM, int, int);

protected:
	BOOL InitMainWindow();
	BOOL InitGraphicAPI();
	void CalculateFrameStats();

private:
	BOOL UpdateScene(float dt, float theta, float phi, float radius);
	BOOL DrawScene();
	BOOL InitializeWindows(int);
	ATOM RegisterMyClass(HINSTANCE);
	void ShutdownWindows();

	void LoadConfigFile();

};

static System* sInstance;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
