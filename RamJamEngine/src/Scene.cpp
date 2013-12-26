#include "Scene.h"

//////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	mFogColor = Color(Color::Silver).GetVector4RGBANorm();
	mFogStart = 15.0f;
	mFogRange = 175.0f;

	mbDrawReflections = true;
	mbUseFog          = false;
	mbUseTexture      = true;
	mbUseBlending     = true;
	mbWireframe       = false;
}

//////////////////////////////////////////////////////////////////////////
void Scene::LoadFromFile( const char* pFile )
{
	mSceneLoader.LoadFromFile(pFile);
}
