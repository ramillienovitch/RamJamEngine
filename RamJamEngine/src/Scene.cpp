#include "Scene.h"

//////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	mFogStart = 15.0f;
	mFogRange = 175.0f;

	mbDrawReflections = true;
	mbUseFog          = true;
	mbUseTexture      = true;
	mbUseBlending     = true;
	mbWireframe       = false;
}

//////////////////////////////////////////////////////////////////////////
void Scene::LoadFromFile( const char* pFile )
{
	mSceneLoader.LoadFromFile(pFile);
}
