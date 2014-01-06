#pragma once

#include "Types.h"
#include "SceneLoader.h"
#include "Transform.h"
#include "Color.h"
#include "GameObject.h"

using namespace RJE_COLOR;

//////////////////////////////////////////////////////////////////////////
struct Scene
{
	SceneLoader				mSceneLoader;
	std::vector<unique_ptr<GameObject>> mGameObjects;
	
	//-----------------------------------------------
	// Editor Values
	Transform*		mGameObjectEditorTransform;
	std::string		mGameObjectEditorName;
	Vector3			mGameObjectEditorPos;
	Vector3			mGameObjectEditorScale;
	TwQuaternion	mGameObjectEditorRot;
	//-----------------------------------------------

	//-----------------------------------------------
	// Scene parameters
	Vector4	mFogColor;
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
