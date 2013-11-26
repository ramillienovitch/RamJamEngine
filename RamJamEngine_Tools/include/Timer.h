/// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "Types.h"
#include "Memory.h"

struct Timer
{
	float	Time() const;			// Return the Elapsed time the Game has been active in seconds since Reset
	void	Time(float time);		// Set the Elapsed playing time -- used for restarting in the middle of a game
	float	DeltaTime() const;		// Return the Delta time between the last two updates
	float	RealDeltaTime() const;	// Return the real delta time (without the timescale) between the last two updates

	void Reset(BOOL pause);
	void Start();
	void Stop();
	void Update();
	bool IsActive() { return mActive; };
	void SetTimeScale( float timeScale );

	static Timer* Instance()
	{
		if(!sInstance)
			sInstance = rje_new Timer();
		
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

private:
	Timer();
	static Timer* sInstance;

	float mSecondsPerCount;		// 1.0 / Frequency
	float mRealDeltaTime;		// delta time without the timescale
	float mDeltaTime;

	float mTimeScale;

	u64 mBaseTime;
	u64 mPausedTime;
	u64 mStopTime;
	u64 mPreviousTime;
	u64 mCurrentTime;

	bool mActive;
};
