#include "Scene.h"
#include "System.h"

//////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	mFogColor = Color(Color::Silver).GetVector4RGBANorm();
	mFogStart = 15.0f;
	mFogRange = 175.0f;

	mbUseFog          = false;
	mbUseTexture      = true;
	mbUseBlending     = true;
	mbWireframe       = false;
}

//////////////////////////////////////////////////////////////////////////
void Scene::LoadFromFile( const char* pFile)
{
	mSceneLoader.LoadFromFile(pFile, mGameObjects);
	//---------------
	mGameObjectEditorTransform	= &mGameObjects[0]->mTransform;
	mGameObjectEditorName		= mGameObjects[0]->mName;
	mGameObjectEditorRot		= TwQuaternion(mGameObjectEditorTransform->Rotation);
	mGameObjectEditorPos		= mGameObjectEditorTransform->Position;
	mGameObjectEditorScale		= mGameObjectEditorTransform->Scale;
	mGameObjectEditorColor		= mGameObjects[0]->mDrawable.mGizmoColor;
}
