#include "Profiler.h"
#include "Debug.h"

Profiler* Profiler::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
Profiler::Profiler()
{
	numProfiles = 1;
	currentParent = 0;
	totalFrames = 0;

	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);
	countsPerMs = double(frequencyCount.QuadPart)/1000;

	Profile fs;
	fs.totalCalls = 0;
	fs.name = "Frame";
	fs.parentId = -1;
	fs.totalTime = 0;
	profiles.push_back(fs);
}

void Profiler::PrintChildren(std::ofstream &fout, int parent, int depth)
{
	depth++;
	for(int i = 1; i < numProfiles; ++i)
	{
		if(profiles[i].parentId == parent)
		{
			// Name and ID
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "|-" << profiles[i].name << " " << i << "\n    ";

			// Parent's ID
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "Parent ID: " << profiles[i].parentId << "\n    ";

			// Total Time in Counts
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "Raw Time (Counts): " << profiles[i].totalTime << "\n    ";

			// Total Calls
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "Total Calls: " << profiles[i].totalCalls << "\n    ";

			// Average Calls Per Parent
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "Avg. Calls/Parent: " << (float)profiles[i].totalCalls/profiles[profiles[i].parentId].totalCalls << "\n    ";

			// Average Time Per Call
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "Avg. Time/Call (Ms): " << (profiles[i].totalTime/profiles[i].totalCalls)/countsPerMs << "\n    ";

			// Percent of Parent's Total Time
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "% of Parent Time: %" << (float)profiles[i].totalTime/profiles[profiles[i].parentId].totalTime*100.0f << "\n    ";

			fout << "\n\n";

			PrintChildren(fout, i, depth);
		}
	}
}

void Profiler::ProfileStart(const char* name)
{
	if(strcmp(name, "Frame") == 0)
	{
		totalFrames++;
		profiles[0].parentId = currentParent;
		currentParent = 0;
	}
	else
	{
		bool profileExists = false;
		for(int i = 0; i < numProfiles; ++i)
		{
			if(profiles[i].name == name)
			{
				profiles[i].parentId = currentParent;
				currentParent = i;
				profileExists = true;
			}
		}
		if(!profileExists)
		{
			Profile p;
			p.totalCalls = 0;
			p.totalTime = 0;
			p.name = name;
			p.parentId = currentParent;
			profiles.push_back(p);
			currentParent = numProfiles++;
		}
	}
}

void Profiler::ProfileEnd(i64 elapsedTime)
{   
	profiles[currentParent].totalCalls++;
	profiles[currentParent].totalTime += elapsedTime;

	currentParent = profiles[currentParent].parentId;
}

void Profiler::PrintToFile()
{
#ifdef RJE_DEBUG
	std::remove("profiler_debug.txt");
	std::ofstream fout;
	fout.open ("profiler_debug.txt");
#else
	std::remove("profiler_release.txt");
	std::ofstream fout;
	fout.open ("profiler_release.txt");
#endif
	fout << "Total Frames: ";
	fout << totalFrames << "\n\n";

	// Average Frame Info
	double avgTPC = (profiles[0].totalTime/totalFrames)/countsPerMs;
	fout << profiles[0].name
		<< "\n   Parent ID: " << profiles[0].parentId
		<< "\n   Raw Time (Counts): " << profiles[0].totalTime
		<< "\n   Total Calls: " << profiles[0].totalCalls
		<< "\n   Avg. Time/Frame (Ms): " << avgTPC
		<< "\n   Avg. FPS: " << 1.0f/(avgTPC/1000.0f);

	fout << "\n\n";

	PrintChildren(fout, 0, 0);

	fout.close();
}
