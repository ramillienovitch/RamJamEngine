#include "Profiler.h"
#include "Memory.h"
#include "Debug.h"
#include "Input.h"
#include "Timer.h"

Profiler* Profiler::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
Profiler::~Profiler()
{ 
	RJE_SAFE_DELETE(mProfileInfoString);
	RJE_SAFE_DELETE(mProfilerInfos);
}

//////////////////////////////////////////////////////////////////////////
Profiler::Profiler()
{
	numProfiles   = 1;
	currentParent = 0;
	totalFrames   = 0;

	mIsActive     = false;
	mCurrentState = E_NONE;

	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);
	countsPerMs = double(frequencyCount.QuadPart)/1000;

	mProfileInfoString	 = new char[PROFILE_INFO_MAX_LENGTH];
	mProfilerInfos		 = new ProfilerInfos;
	mProfilerRefreshRate = -1.0f;
	ResetProfilerInfo();

	Profile fs;
	fs.totalCalls = 0;
	fs.name       = "Frame";
	fs.parentId   = -1;
	fs.totalTime  = 0;
	profiles.push_back(fs);
}

//////////////////////////////////////////////////////////////////////////
BOOL Profiler::IsActive() { return mIsActive; }

//////////////////////////////////////////////////////////////////////////
void Profiler::ActivateProfiler()
{
	mIsActive     = true;
	mCurrentState = E_SIMPLE;
}

//////////////////////////////////////////////////////////////////////////
void Profiler::ExitProfiler()
{
	mIsActive     = false;
	mCurrentState = E_NONE;
}

//////////////////////////////////////////////////////////////////////////
void Profiler::Update()
{
	PROFILE_CPU("Update Profiler");
	if (mIsActive)
	{
		if (Input::Instance()->GetKeyboardDown(F2))		ChangeState();
		if (Input::Instance()->GetKeyboardDown(Keycode::Esc) || Input::Instance()->GetKeyboardDown(Keycode::F1))
		{
			ExitProfiler();
			mProfilerRefreshRate = -1.0f;
		}
	}
	else
	{
		if (Input::Instance()->GetKeyboardDown(F2))		ActivateProfiler();
	}
}

//////////////////////////////////////////////////////////////////////////
int Profiler::GetState() {return (int) mCurrentState;}
void Profiler::ChangeState()
{
	mProfilerRefreshRate = -1.0f;
	switch (mCurrentState)
	{
	case E_NONE:		mCurrentState = E_SIMPLE;		return;
	case E_SIMPLE:		mCurrentState = E_ADVANCED;		return;
	case E_ADVANCED:	mCurrentState = E_CPU;			return;
	case E_CPU:			mCurrentState = E_GPU;			return;
	case E_GPU:			mCurrentState = E_MEMORY;		return;
	case E_MEMORY:		mCurrentState = E_PHYSICS;		return;
	case E_PHYSICS:		mCurrentState = E_ANIMATION;	return;
	case E_ANIMATION:	mCurrentState = E_AI;			return;
	case E_AI:			mCurrentState = E_SCRIPT;		return;
	case E_SCRIPT:		mCurrentState = E_SOUND;		return;
	case E_SOUND:		mCurrentState = E_SIMPLE;		return;
	default:			ExitProfiler();	return;
	}
}

//////////////////////////////////////////////////////////////////////////
void Profiler::PrintChildren(std::ofstream &fout, int parent, int depth)
{
	for(int i = 1; i < numProfiles; ++i)
	{
		if(profiles[i].parentId == parent)
		{
			// Name and ID
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "|-" << profiles[i].name << " (ID: " << i << ")\n    ";

			// Parent's ID
			for(int tabs = 0; tabs < depth; ++tabs)	{fout << "    ";}
			fout << "Parent: " << profiles[profiles[i].parentId].name << " (ID : " << profiles[i].parentId << ")\n    ";

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

//////////////////////////////////////////////////////////////////////////
void Profiler::ProfileStart(const char* name)
{
	if(strcmp(name, "Frame") == 0)
	{
		totalFrames++;
		profiles[0].parentId = currentParent;
		currentParent        = 0;
	}
	else
	{
		bool profileExists = false;
		for(int i = 0; i < numProfiles; ++i)
		{
			if(profiles[i].name == name)
			{
				profiles[i].parentId = currentParent;
				currentParent        = i;
				profileExists        = true;
			}
		}
		if(!profileExists)
		{
			Profile p;
			p.totalCalls = 0;
			p.totalTime  = 0;
			p.name       = name;
			p.parentId   = currentParent;
			profiles.push_back(p);
			currentParent = numProfiles++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Profiler::ProfileEnd(u64 elapsedTime)
{   
	profiles[currentParent].totalCalls++;
	profiles[currentParent].totalTime += elapsedTime;

	currentParent = profiles[currentParent].parentId;
}

//////////////////////////////////////////////////////////////////////////
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
	double avgTPC = (profiles[0].totalTime/totalFrames)/countsPerMs;
	fout	<< "RamJam Engine Profiler Report\n"
			<< "Total Frames: "				<< totalFrames
			<< "\nAvg. Time/Frame (Ms): "	<< avgTPC
			<< "\nAvg. FPS: "				<< 1.0f/(avgTPC/1000.0f) << "\n\n";

	PrintChildren(fout, 0, 0);

	fout.close();
}

//////////////////////////////////////////////////////////////////////////
void Profiler::GetProfilerInfo()
{
	if (mProfilerRefreshRate > 0)
	{
		mProfilerRefreshRate -= Timer::Instance()->RealDeltaTime();
		return;
	}

	mProfilerRefreshRate = 1.0f;
	ResetProfilerInfo();
	switch (mCurrentState)
	{
	case E_SIMPLE:		DisplaySimpleState();		return;
	case E_CPU:			DisplayCpuState();			return;
	case E_ADVANCED:	DisplayAdvancedState();		return;
	case E_MEMORY:		DisplayMemoryState();		return;
	case E_PHYSICS:		DisplayPhysicsState();		return;
	case E_ANIMATION:	DisplayAnimationState();	return;
	case E_AI:			DisplayAiState();			return;
	case E_SCRIPT:		DisplayScriptState();		return;
	case E_SOUND:		DisplaySoundState();		return;
	case E_NONE:
	case E_GPU:			// The GPU Profiling is displayed by the Rendering API itself (DX11Profiler or OglProfiler)
	default:			return;
	}
}

//////////////////////////////////////////////////////////////////////////
void Profiler::DisplaySimpleState()
{
	ConcatText("  - Profiler Simple Mode - \n", SCREEN_ROSE);
	//-------------
	ConcatText("CPU Usage : ");
	char buf[32];
	itoa(mProfilerInfos->ProcessCpuUsage, buf, 32, 10);
	ConcatText(buf); 
	ConcatText("%\n");
	//-------------
	ConcatText("RAM Usage : ");
	ZeroMemory(buf, 32);
	itoa(mProfilerInfos->ProcessPeakWorkingSet, buf, 32, 10);
	ConcatText(buf); 
	ConcatText("MB\n");
	//-------------
	ConcatText("\nPhysics : ");		ConcatText("Not Yet Available", SCREEN_GRAY);
	ConcatText("\nAnimation : ");	ConcatText("Not Yet Available", SCREEN_GRAY);
	ConcatText("\nAI : ");			ConcatText("Not Yet Available", SCREEN_GRAY);
	ConcatText("\nScripts : ");		ConcatText("Not Yet Available", SCREEN_GRAY);
	ConcatText("\nSound : ");		ConcatText("Not Yet Available", SCREEN_GRAY);
}

//////////////////////////////////////////////////////////////////////////
void Profiler::DisplayAdvancedState()
{
	ConcatText("Advanced Mode Not Yet Supported", SCREEN_RED);
}

//////////////////////////////////////////////////////////////////////////
void Profiler::DisplayCpuState()
{
	ConcatText("  - Profiler CPU Mode - \n", SCREEN_ROSE);
	//-------------
	ConcatText("CPU Usage : ");
	char buf[32];
	itoa(mProfilerInfos->ProcessCpuUsage, buf, 32, 10);
	ConcatText(buf); 
	ConcatText("%\n");
	//-------------
	std::sort(profiles.rbegin(), profiles.rend());	// reverse sort the profiles (using totalTimes)
	for (std::vector<Profile>::iterator it=profiles.begin()+1 ; it<profiles.end(); ++it)
	{
		ConcatText(it->name);
		dtoa(buf, 32, (float)((it->totalTime/totalFrames)/countsPerMs), 10);
		ConcatText(" : ");
		ConcatText(buf);
		ConcatText("ms (");
		dtoa(buf, 32, (float)(((float)it->totalTime/profiles[0].totalTime) * 100.0f), 10);
		ConcatText(buf);
		ConcatText("%)\n");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Profiler::DisplayMemoryState()
{
	ConcatText("Memory Mode Not Yet Supported", SCREEN_RED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Profiler::DisplayPhysicsState()
{
	ConcatText("Physics Mode Not Yet Supported", SCREEN_RED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Profiler::DisplayAnimationState()
{
	ConcatText("Animation Mode Not Yet Supported", SCREEN_RED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Profiler::DisplayAiState()
{
	ConcatText("AI Mode Not Yet Supported", SCREEN_RED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Profiler::DisplayScriptState()
{
	ConcatText("Script Mode Not Yet Supported", SCREEN_RED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Profiler::DisplaySoundState()
{
	ConcatText("Sound Mode Not Yet Supported", SCREEN_RED);
}

//////////////////////////////////////////////////////////////////////////
void Profiler::ConcatText(const char* text, u8 color)
{
	int newSize = (int)strlen(text);

	if (color == SCREEN_WHITE)
	{
		memcpy(mProfileInfoString+mProfileInfoStringSize, text, newSize);
		mProfileInfoString[mProfileInfoStringSize+newSize] = nullchar;
		mProfileInfoStringSize += newSize;
	}
	else
	{
		mProfileInfoString[mProfileInfoStringSize] = (char)color;
		memcpy(mProfileInfoString+mProfileInfoStringSize+1, text, newSize);
		mProfileInfoString[mProfileInfoStringSize+newSize+1] = (char)SCREEN_WHITE;
		mProfileInfoString[mProfileInfoStringSize+newSize+2] = nullchar;
		mProfileInfoStringSize += newSize+2;
	}

}

//////////////////////////////////////////////////////////////////////////
void Profiler::ResetProfilerInfo()
{
	mProfileInfoString[0]  = nullchar;
	mProfileInfoStringSize = 0;
}
