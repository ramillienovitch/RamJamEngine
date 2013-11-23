#pragma once

#include "DX11Helper.h"

struct ProfilingInfo_Rendering
{
	u64 IAVertices;
	u64 IAPrimitives;
	u64 VSInvoc;
	u64 PSInvoc;
	u64 GSInvoc;
	u64 HSInvoc;
	u64 DSInvoc;
	u64 CSInvoc;
	u64 rasterizerPrimitives;
	u64 renderedPrimitives;
};
struct ProfilingInfo
{
	ProfilingInfo_Rendering Info_Rendering;
};

//////////////////////////////////////////////////////////////////////////
struct DX11Profiler
{
	static DX11Profiler sInstance;

	static const u64 QueryLatency = 5;

	//=====================
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
	//=====================
	struct ProfileDataDeep
	{
		ID3D11Query* mPipelineStatsQuery[QueryLatency];
		BOOL mQueryStarted;
		BOOL mQueryFinished;

		ProfilingInfo mInfos;

		ProfileDataDeep() : mQueryStarted(false), mQueryFinished(false)
		{
			for (int i=0 ; i<QueryLatency ; ++i)
				mPipelineStatsQuery[i] = nullptr;
		}

		~ProfileDataDeep()
		{
			for (int i=0 ; i<QueryLatency ; ++i)
				RJE_SAFE_RELEASE(mPipelineStatsQuery[i]);
		}
	};

	//=====================

	typedef std::map<wstring, ProfileData>		ProfileMap;
	typedef std::map<wstring, ProfileDataDeep>	DeepProfileMap;

	ProfileMap		mProfiles;
	u64				mCurrFrame;

	DeepProfileMap	mDeepProfiles;
	u64				mCurrDeepFrame;

	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mContext;

	float	mTimeWaitingForQueries;

	//------------------

	char*	mProfileInfoString;
	char*	mProfileDeepInfoString;
	u16		mProfileInfoStringSize;
	u16		mProfileDeepInfoStringSize;
	float	mProfilerRefreshRate;

	//---------------------------

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* immContext);

	void StartProfile(    const wstring& name);
	void StartDeepProfile(const wstring& name);
	void EndProfile(      const wstring& name);
	void EndDeepProfile(  const wstring& name);

	void EndFrame();
	void Exit();

	void GetProfilerInfo();
	void ConcatText        (const char* text, u8 color = SCREEN_WHITE, BOOL deepProfile  = false);
	void ConcatTextAndAlign(const char* text, int tabs = 4, u8 color = SCREEN_WHITE, BOOL deepProfile  = false);
	void ResetProfilerInfo();
};

//////////////////////////////////////////////////////////////////////////

#if RJE_PROFILE_GPU
#	define PROFILE_GPU_INIT(device, context)	DX11Profiler::sInstance.Initialize(device, context);
#	define PROFILE_GPU_START(name)				DX11Profiler::sInstance.StartProfile(name);
#	define PROFILE_GPU_START_DEEP(name)			DX11Profiler::sInstance.StartDeepProfile(name);
#	define PROFILE_GPU_END(name)				DX11Profiler::sInstance.EndProfile(name);
#	define PROFILE_GPU_END_DEEP(name)			DX11Profiler::sInstance.EndDeepProfile(name);
#	define PROFILE_GPU_END_FRAME()				DX11Profiler::sInstance.EndFrame();
#	define PROFILE_GPU_EXIT()					DX11Profiler::sInstance.Exit();
#else
#	define PROFILE_GPU_INIT(device, context)	(void)0
#	define PROFILE_GPU_START(name)				(void)0
#	define PROFILE_GPU_START_DEEP(name)			(void)0
#	define PROFILE_GPU_END(name)				(void)0
#	define PROFILE_GPU_END_DEEP(name)			(void)0
#	define PROFILE_GPU_END_FRAME()				(void)0
#	define PROFILE_GPU_EXIT()					(void)0
#endif
