#include "Timer.h"

Timer* Timer::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
Timer::Timer():mActive(false)
{

	u64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	mSecondsPerCount = 1.0f / frequency;

	Reset();
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float Timer::Time() const
{
	if (mActive)
	{
		// The distance mCurrTime - mBaseTime includes paused time,
		// which we do not want to count.  To correct this, we can subtract 
		// the paused time from mCurrTime:  
		//
		//  (mCurrTime - mPausedTime) - mBaseTime 
		//
		//                     |<--paused time-->|
		// ----*---------------*-----------------*------------*------> time
		//  mBaseTime       mStopTime        startTime     mCurrTime
		return static_cast<float>(((mCurrentTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
	}
	else
	{
		
		// If we are stopped, do not count the time that has passed since we stopped.
		// Moreover, if we previously already had a pause, the distance 
		// mStopTime - mBaseTime includes paused time, which we do not want to count.
		// To correct this, we can subtract the paused time from mStopTime:  
		//
		//                     |<--paused time-->|
		// ----*---------------*-----------------*------------*------------*------> time
		//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime
		return static_cast<float>(((mStopTime - mPausedTime)-mBaseTime)*mSecondsPerCount);
	}
}


void Timer::Time(float time)
{
	// Reset the internal state to reflect a PlayingTime of 'time'
	// Offset the baseTime by this 'time'.
	mActive		= false;
	mStopTime	= mCurrentTime;
	mPausedTime	= 0;

	mBaseTime = mStopTime - static_cast<u64>(time / mSecondsPerCount);
}


float Timer::DeltaTime() const
{
	return mDeltaTime;
}

void Timer::Reset()
{
	u64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	mBaseTime		= currentTime;
	mPreviousTime	= currentTime;
	mStopTime		= currentTime;
	mCurrentTime	= currentTime;
	mPausedTime		= 0;
	mActive			= false;
}

void Timer::Start()
{
	u64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime

	if (!mActive)
	{
		// Accumulate the time elapsed between stop and start pairs.
		mPausedTime += (startTime - mStopTime);

		mPreviousTime	= startTime;
		mStopTime		= 0;
		mActive			= true;
	}
}

void Timer::Stop()
{
	if (mActive)
	{
		// Set the stop time to the time of the last update.
		mStopTime = mCurrentTime;
		mActive = false;
	}
}

void Timer::Update()
{
	if (!mActive)
	{
		mDeltaTime = 0.0f;
		return;
	}

	u64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	mCurrentTime = currentTime;

	// Time difference between this frame and the previous.
	mDeltaTime = (mCurrentTime - mPreviousTime)*mSecondsPerCount;
	
	// Prepare for next frame.
	mPreviousTime = mCurrentTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
		mDeltaTime = 0.0f;
}
