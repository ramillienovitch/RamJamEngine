#pragma once

#include "Types.h"
#include "SceneLoader.h"
#include "Transform.h"

//////////////////////////////////////////////////////////////////////////
struct Scene
{
	std::set<Transform>		mGameObjects;
	SceneLoader				mSceneLoader;

	//-----------------------------------------------
	// Scene parameters
	float	mFogStart;
	float	mFogRange;
	BOOL	mbDrawReflections;
	BOOL	mbUseTexture;
	BOOL	mbUseBlending;
	BOOL	mbUseFog;
	BOOL	mbWireframe;
	//-----------------------------------------------
	
	Scene();
	void LoadFromFile(const char* pFile);
	void ExtractGameObjects();
};
