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
	mbUseNormalMaps  = true;
	//------
	mbOnlyPosition = false;
	mbOnlyAlbedo   = false;
	mbOnlyNormals  = false;
	mbOnlyDepth    = false;
	mbOnlySpecular = false;
	//------
	mbDrawLightSphere = false;
	mbDrawSun         = false;
	mbAnimateLights   = false;
	mbAnimateSun      = false;
	mSunHeight        = 0.5f;
	//------
	mbViewLightCount       = false;
	mbViewPerSampleShading = false;
	//------
	mbDisplayShadows       = true;
	mbAlignLightToFrustum  = true;
	mbViewLightSpace       = false;
	mbViewPartitions       = false;
	mbEdgeSoftening        = false;
	mEdgeSofteningAmount    = 0.0f;
	mMaxEdgeSofteningFilter = 16.0f;
	mShadowStrength         = 1.0f;
	//------
	mbUsePositiveExponent = true;
	mbUseNegativeExponent = true;
	mPositiveExponent     = 800.0f;
	mNegativeExponent     = 100.0f;
	//------
	mCurrentEditorGOIdx   = 0;
	mCurrentEditorGOIdxUI = 0;
	mbEnableGizmo = false;
	//------
	mPointLightRadius = 20.0f;
	mPointLightHeight = 5.0f;
}

//////////////////////////////////////////////////////////////////////////
Scene::~Scene()
{
	mEditorGameobject->mDrawable.mGizmo->Destroy();
	RJE_SAFE_DELETE(mEditorGameobject->mDrawable.mGizmo);
	RJE_SAFE_DELETE(mEditorGameobject);
}


//////////////////////////////////////////////////////////////////////////
void Scene::LoadFromFile(const char* pFile)
{
	mSceneLoader.LoadFromFile(pFile, mGameObjects, mSkyboxName);
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

	//-------

	ComputeSceneExtents();
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
void Scene::ComputeSceneExtents()
{
	Vector3 mSceneMin = Vector3(RJE::Math::Infinity_f, RJE::Math::Infinity_f, RJE::Math::Infinity_f);
	Vector3 mSceneMax = -mSceneMin;

	for(const unique_ptr<GameObject>& gameobject : mGameObjects)
	{
		if (gameobject->mDrawable.mMesh)
		{
			for (u32 iSubset=0 ; iSubset<gameobject->mDrawable.mMesh->mSubsetCount; ++iSubset)
			{
				Vector3 minAABB = gameobject->mDrawable.mMesh->mSubsets[iSubset].mCenter - gameobject->mDrawable.mMesh->mSubsets[iSubset].mExtents;
				Vector3 maxAABB = gameobject->mDrawable.mMesh->mSubsets[iSubset].mCenter + gameobject->mDrawable.mMesh->mSubsets[iSubset].mExtents;
				minAABB = Vector3::Scale(minAABB, gameobject->mTransform.Scale);
				maxAABB = Vector3::Scale(maxAABB, gameobject->mTransform.Scale);
				minAABB += gameobject->mTransform.Position;
				maxAABB += gameobject->mTransform.Position;
				mSceneMin = Vector3::Min(mSceneMin, minAABB);
				mSceneMax = Vector3::Max(mSceneMax, maxAABB);
			}
		}
	}

	mSceneCenter  = 0.5f*(mSceneMin+mSceneMax);
	mSceneExtents = 0.5f*(mSceneMax-mSceneMin);
	mSceneRadius  = mSceneExtents.Magnitude();
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
