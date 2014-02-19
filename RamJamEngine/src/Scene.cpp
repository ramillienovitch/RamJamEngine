#include "Scene.h"
#include "System.h"

//////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	mFogColor          = Color(Color::Silver).GetVector4RGBANorm();
	mAmbientLightColor = Color(Color::Black).GetVector4RGBANorm();
	mFogStart = 15.0f;
	mFogRange = 175.0f;
	//-----------
	mbDeferredRendering = true;
	//-----------
	mbUseFog         = false;
	mbUseTexture     = true;
	mbUseBlending    = true;
	mbWireframe      = false;
	mbUseFaceNormals = false;
	//------
	mbOnlyPosition = false;
	mbOnlyAlbedo   = false;
	mbOnlyNormals  = false;
	mbOnlyDepth    = false;
	mbOnlySpecular = false;
	//------
	mbViewLightCount       = false;
	mbViewPerSampleShading = false;
	//------
	
	mCurrentEditorGOIdx   = 0;
	mCurrentEditorGOIdxUI = 0;
	mbEnableGizmo = false;
}

//////////////////////////////////////////////////////////////////////////
Scene::~Scene()
{
	mEditorGameobject->mDrawable.mGizmo->Destroy();
	RJE_SAFE_DELETE(mEditorGameobject->mDrawable.mGizmo);
	RJE_SAFE_DELETE(mEditorGameobject);
}


//////////////////////////////////////////////////////////////////////////
void Scene::LoadFromFile( const char* pFile)
{
	mSceneLoader.LoadFromFile(pFile, mGameObjects);
	//---------------
	mGameObjectEditorTransform	= &mGameObjects[mCurrentEditorGOIdx]->mTransform;
	mGameObjectEditorName		= mGameObjects[mCurrentEditorGOIdx]->mName;
	mGameObjectEditorRot		= TwQuaternion(mGameObjectEditorTransform->Rotation);
	mGameObjectEditorPos		= mGameObjectEditorTransform->Position;
	mGameObjectEditorScale		= mGameObjectEditorTransform->Scale;
	mGameObjectEditorColor		= mGameObjects[mCurrentEditorGOIdx]->mDrawable.mGizmoColor;
}

//////////////////////////////////////////////////////////////////////////
void Scene::Init()
{
	mEditorGameobject = rje_new GameObject();
#if (RJE_GRAPHIC_API == DIRECTX_11)
	mEditorGameobject->mDrawable.mGizmo = rje_new DX11Mesh;
#else
	mEditorGameobject->mDrawable.mGizmo = rje_new OglMesh;
#endif
	mEditorGameobject->mDrawable.mGizmo->LoadAxisArrows(Vector3::right, Vector3::up, Vector3::forward);
	mEditorGameobject->mDrawable.mGizmoColor = Color::White;
	mEditorGameobject->mTransform = mGameObjects[mCurrentEditorGOIdx]->mTransform;
}

//////////////////////////////////////////////////////////////////////////
void Scene::ChangeCurrentEditorGO(u32& idx)
{
	mCurrentEditorGOIdx = RJE::Math::Clamp(idx, 0u, (u32)(mGameObjects.size()-1));
	idx = mCurrentEditorGOIdx;

	mGameObjectEditorTransform	= &mGameObjects[mCurrentEditorGOIdx]->mTransform;
	mGameObjectEditorName		= mGameObjects[mCurrentEditorGOIdx]->mName;
	mGameObjectEditorRot		= TwQuaternion(mGameObjectEditorTransform->Rotation);
	mGameObjectEditorPos		= mGameObjectEditorTransform->Position;
	mGameObjectEditorScale		= mGameObjectEditorTransform->Scale;
	mGameObjectEditorColor		= mGameObjects[mCurrentEditorGOIdx]->mDrawable.mGizmoColor;
}

//////////////////////////////////////////////////////////////////////////
void Scene::Update()
{
	if (mCurrentEditorGOIdx != mCurrentEditorGOIdxUI)
		ChangeCurrentEditorGO(mCurrentEditorGOIdxUI);

	mGameObjectEditorTransform->Position			= mGameObjectEditorPos;
	mGameObjectEditorTransform->Rotation			= mGameObjectEditorRot;
	mGameObjectEditorTransform->Scale				= mGameObjectEditorScale;
	mGameObjectEditorTransform->WorldMat			= mGameObjectEditorTransform->WorldMatrix();
	mGameObjectEditorTransform->WorldMatNoScale		= mGameObjectEditorTransform->WorldMatrixNoScale();
	mGameObjects[mCurrentEditorGOIdx]->mDrawable.mGizmoColor = mGameObjectEditorColor;
}
