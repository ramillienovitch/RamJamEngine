#pragma once

#include "Types.h"
#include "Debug.h"

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
	int parentId;
	int totalCalls;
	u64 totalTime;
};

//////////////////////////////////////////////////////////////////////////
struct Profiler
{
	static Profiler* Instance()
	{
		if(!sInstance)
			sInstance = new Profiler();

		return sInstance;
	}

	static void DeleteInstance()
	{
		if(sInstance)
		{
			sInstance->PrintToFile();
			delete sInstance;
			sInstance = nullptr;
		}
	}

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
	int GetState();
	void ChangeState();

	ProfilingInfo mInfos;

private:
	Profiler();
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

#if RJE_PROFILE == RJE_PROFILE_NONE
#	define PROFILE(name)	(void)0
// TODO: Profiler Sample mode
// #elif RJE_PROFILE == RJE_PROFILE_SAMPLE
// #	define PROFILE(name)	AutoProfile profile(name)
#elif RJE_PROFILE == RJE_PROFILE_INTRUSIVE
#	define PROFILE(name)	AutoProfile profile(name)
#endif
