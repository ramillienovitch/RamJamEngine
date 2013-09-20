#pragma once

#include "Types.h"

struct Profile
{
	const char* name;
	int parentId;
	int totalCalls;
	i64 totalTime;
};

class Profiler
{
private:
	std::vector<Profile> profiles;
	int numProfiles;
	int currentParent;
	int totalFrames;
	double countsPerMs;

public:
	Profiler();
	void PrintChildren(std::ofstream &fout, int parent, int depth);
	void ProfileStart(const char* name);
	void ProfileEnd(i64 elapsedTime);
	void PrintToFile();
};

static Profiler sProfiler;

struct AutoProfile
{
	AutoProfile(const char* name)
	{
		name = name;
		sProfiler.ProfileStart(name);
		QueryPerformanceCounter(&startTime);
	}

	~AutoProfile()
	{
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);
		i64 elapsedTime = endTime.QuadPart - startTime.QuadPart;
		sProfiler.ProfileEnd(elapsedTime);
	}

	const char* name;
	LARGE_INTEGER startTime;
};
#define PROFILE(name) AutoProfile p(name)
