#include "Scene.h"
#include "System.h"

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

	unique_ptr<GameObject> modelGO (new GameObject);
	modelGO->mName = "Dragon";
	modelGO->mTransform.Rotation		= Quaternion(0,45,0).ToMatrix();
	modelGO->mTransform.Scale			= Vector3(0.2f, 0.2f, 0.2f);
	modelGO->mTransform.Position		= Vector3(0.0f, 1.0f, 0.0f);
	modelGO->mTransform.WorldMat		= modelGO->mTransform.WorldMatrix();
	modelGO->mTransform.WorldMatNoScale	= modelGO->mTransform.WorldMatrixNoScale();
	string modelPath = System::Instance()->mDataPath + CIniFile::GetValue("modelpath", "meshes", System::Instance()->mResourcesPath);
	modelGO->mDrawable.mMesh = rje_new DX11Mesh;
	modelGO->mDrawable.mMesh->LoadModelFromFile(modelPath);
	modelGO->mDrawable.mMesh->LoadMaterialFromFile("model.mat");
	modelGO->mDrawable.mGizmo = rje_new DX11Mesh;
	modelGO->mDrawable.mGizmo->LoadWireBox(1,1,1, Color::Lime);
	modelGO->mDrawable.SetShader(DX11Effects::BasicFX);
	modelGO->mDrawable.SetShaderGizmo(DX11Effects::ColorFX);
	mGameObjects.push_back(std::move(modelGO));
	//---------------
	mGameObjectEditorTransform	= &mGameObjects[0]->mTransform;
	mGameObjectEditorName		= mGameObjects[0]->mName;
	mGameObjectEditorRot		= TwQuaternion(mGameObjectEditorTransform->Rotation);
	mGameObjectEditorPos		= mGameObjectEditorTransform->Position;
	mGameObjectEditorScale		= mGameObjectEditorTransform->Scale;
}
