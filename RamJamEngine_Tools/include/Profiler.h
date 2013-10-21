#pragma once

#include "Types.h"

struct Profile
{
	const char* name;
	int parentId;
	int totalCalls;
	i64 totalTime;
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
			delete sInstance;
			sInstance = nullptr;
		}
	}

	void PrintChildren(std::ofstream &fout, int parent, int depth);
	void ProfileStart(const char* name);
	void ProfileEnd(i64 elapsedTime);
	void PrintToFile();

private:
	Profiler();
	static Profiler* sInstance;

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
		i64 elapsedTime = endTime.QuadPart - startTime.QuadPart;
		Profiler::Instance()->ProfileEnd(elapsedTime);
	}

	const char* name;
	LARGE_INTEGER startTime;
};
#define PROFILE(name) AutoProfile p(name)
