#pragma once

#include "DX11Helper.h"

struct DX11Profiler
{
	static DX11Profiler sInstance;

	static const u64 QueryLatency = 5;

	struct ProfileData
	{
		ID3D11Query* mDisjointQuery[QueryLatency];
		ID3D11Query* mTimestampStartQuery[QueryLatency];
		ID3D11Query* mTimestampEndQuery[QueryLatency];
		BOOL mQueryStarted;
		BOOL mQueryFinished;

		float mElaspedTime;

		ProfileData() : mElaspedTime(0.0f), mQueryStarted(false), mQueryFinished(false)
		{
			for (int i=0 ; i<QueryLatency ; ++i)
			{
				mDisjointQuery      [i] = nullptr;
				mTimestampStartQuery[i] = nullptr;
				mTimestampEndQuery  [i] = nullptr;
			}
		}

		~ProfileData()
		{
			for (int i=0 ; i<QueryLatency ; ++i)
			{
				RJE_SAFE_RELEASE(mDisjointQuery      [i]);
				RJE_SAFE_RELEASE(mTimestampStartQuery[i]);
				RJE_SAFE_RELEASE(mTimestampEndQuery  [i]);
			}
		}
	};

	typedef std::map<wstring, ProfileData> ProfileMap;

	ProfileMap	mProfiles;
	u64			mCurrFrame;

	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mContext;

	float	mTimeWaitingForQueries;

	//---------------------------

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* immContext);

	void StartProfile(const wstring& name);
	void EndProfile(  const wstring& name);

	void EndFrame();
};

//////////////////////////////////////////////////////////////////////////

#if RJE_PROFILE_GPU
#	define PROFILE_GPU_INIT(device, context)	DX11Profiler::sInstance.Initialize(device, context);
#	define PROFILE_GPU_START(name)				DX11Profiler::sInstance.StartProfile(name);
#	define PROFILE_GPU_END(name)				DX11Profiler::sInstance.EndProfile(name);
#	define PROFILE_GPU_END_FRAME()				DX11Profiler::sInstance.EndFrame();
#else
#	define PROFILE_GPU_INIT(device, context)	(void)0
#	define PROFILE_GPU_START(name)				(void)0
#	define PROFILE_GPU_END(name)				(void)0
#	define PROFILE_GPU_END_FRAME()				(void)0
#endif
