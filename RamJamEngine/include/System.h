#pragma once

#include "stdafx.h"

class System
{
public:
	System();
	System(const System&);
	~System();

	HWND		mHWnd;
	HINSTANCE	mHInst;								// current instance
	TCHAR		mSzTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR		mSzWindowClass[MAX_LOADSTRING];	// the main window class name

	BOOL				Initialize(int);
	void				Shutdown();
	void				Launch();
	LRESULT CALLBACK	MessageHandler(HWND, UINT, WPARAM, LPARAM);


private:
	BOOL Update();
	BOOL InitializeWindows(int);
	ATOM RegisterMyClass(HINSTANCE);
	void ShutdownWindows();
};

static System* sInstance;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
