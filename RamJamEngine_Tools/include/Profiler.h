#pragma once

#include "Types.h"
#include "Debug.h"
#include "Globals.h"
#include "Memory.h"

//////////////////////////////////////////////////////////////////////////
typedef enum PROFILER_STATES
{
	E_NONE      = 0,
	E_SIMPLE    = 1,
	E_ADVANCED  = 2,
	E_CPU       = 3,
	E_GPU       = 4,
	E_MEMORY    = 5,
	E_PHYSICS   = 6,
	E_ANIMATION = 7,
	E_AI        = 8,
	E_SCRIPT    = 9,
	E_SOUND     = 10
} PROFILER_STATES;

//////////////////////////////////////////////////////////////////////////
struct Profile
{
	const char* name;
	int Id;
	int parentId;
	int totalCalls;
	u64 totalTime;

	BOOL operator<(const Profile& rhs) {return (this->totalTime < rhs.totalTime);}
};

struct ProfilerInfos
{
	i16		ProcessCpuUsage;
	i16		ProcessPeakWorkingSet;
	i16		ProcessWorkingSet;
};

#define PROFILE_INFO_MAX_LENGTH 4096

//////////////////////////////////////////////////////////////////////////
struct Profiler
{
	static Profiler* Instance()
	{
		if(!sInstance)
			sInstance = rje_new Profiler();

		return sInstance;
	}

	static void DeleteInstance()
	{
		if(sInstance)
		{
#if RJE_PROFILE_CPU
			sInstance->PrintToFile();
#endif
			delete sInstance;
			sInstance = nullptr;
		}
	}

	char*			mProfileInfoString;
	u16				mProfileInfoStringSize;
	ProfilerInfos*	mProfilerInfos;
	float			mProfilerRefreshRate;

	//-----------
	void PrintChildren(std::ofstream &fout, int parent, int depth);
	void ProfileStart(const char* name);
	void ProfileEnd(u64 elapsedTime);
	void PrintToFile();
	//-----------
	BOOL IsActive();
	void ExitProfiler();
	void ActivateProfiler();
	//-----------
	void Update();
	//-----------
	int  GetState();
	void ChangeState();
	//-----------
	void GetProfilerInfo();
	void DisplaySimpleState();
	void DisplayAdvancedState();
	void DisplayCpuState();
	void DisplayMemoryState();
	void DisplayPhysicsState();
	void DisplayAnimationState();
	void DisplayAiState();
	void DisplayScriptState();
	void DisplaySoundState();
	//-----------
	void ConcatText(const char* text, u8 color = SCREEN_WHITE);
	void ConcatTextAndAlign(const char* text, int tabs = 4, u8 color = SCREEN_WHITE);
	void ResetProfilerInfo();

private:
	Profiler();
	~Profiler();
	static Profiler* sInstance;

	BOOL mIsActive;
	PROFILER_STATES mCurrentState;

	std::vector<Profile> profiles;
	int numProfiles;
	int currentParent;
	int totalFrames;
	double countsPerMs;
};

//////////////////////////////////////////////////////////////////////////
struct AutoProfile
{
	AutoProfile(const char* name)
	{
		name = name;
		Profiler::Instance()->ProfileStart(name);
		QueryPerformanceCounter(&startTime);
	}

	~AutoProfile()
	{
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);
		u64 elapsedTime = endTime.QuadPart - startTime.QuadPart;
		Profiler::Instance()->ProfileEnd(elapsedTime);
	}

	const char* name;
	LARGE_INTEGER startTime;
};

#if RJE_PROFILE_CPU
#	define PROFILE_CPU(name)	AutoProfile profile(name)
#else
#	define PROFILE_CPU(name)	(void)0
#endif
