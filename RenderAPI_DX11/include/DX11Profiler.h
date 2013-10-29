#pragma once

#include "DX11Helper.h"

struct DX11Profiler
{
	static DX11Profiler sProfiler;

	static const u64 QueryLatency = 5;

	struct ProfileData
	{
		ID3D11Query* mDisjointQuery[QueryLatency];
		ID3D11Query* mTimestampStartQuery[QueryLatency];
		ID3D11Query* mTimestampEndQuery[QueryLatency];
		BOOL mQueryStarted;
		BOOL mQueryFinished;

		float mElaspedTime;

		ProfileData() : mElaspedTime(0.0f), mQueryStarted(false), mQueryFinished(false) {}
	};

	typedef std::map<const char*, ProfileData> ProfileMap;

	ProfileMap	mProfiles;
	u64			mCurrFrame;

	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mContext;

	float	mTimeWaitingForQueries;

	//---------------------------

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* immContext);

	void StartProfile(const char* name);
	void EndProfile(  const char* name);

	void EndFrame();
};

//////////////////////////////////////////////////////////////////////////
struct AutoDXProfile
{
	AutoDXProfile(const char* name)
	{
		name = name;
		DX11Profiler::sProfiler.StartProfile(name);
	}

	~AutoDXProfile()
	{
		DX11Profiler::sProfiler.EndProfile(name);
	}

	const char* name;
};

#if RJE_PROFILE_GPU
#	define PROFILE_GPU(name)	AutoDXProfile profile(name)
#elif
#	define PROFILE_GPU(name)	(void)0
#endif
