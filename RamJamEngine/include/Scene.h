#pragma once

#include "Types.h"
#include "SceneLoader.h"
#include "Transform.h"

//////////////////////////////////////////////////////////////////////////
struct Scene
{
	std::set<Transform>		mGameObjects;
	SceneLoader				mSceneLoader;

	void LoadFromFile(const char* pFile);
	void ExtractGameObjects();
};
