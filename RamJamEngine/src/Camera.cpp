#include "Camera.h"

//////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	mPosition = Vector3::zero;
	mUp       = Vector3(0.0f, 1.0f, 0.0f);
	mLookAt   = Vector3::zero;

	mView      = Matrix44::identity;
	mPerspProj = Matrix44::identity;
	mOrthoProj = Matrix44::identity;
	mCurrentProjectionMatrix = &mPerspProj;

	bIsOrtho = false;

	mSettings.FOV         = 60;
	mSettings.OrthoZoom   = 0.01f;
	mSettings.NearZ       = 0.01f;
	mSettings.FarZ        = 1000.0f;
	mSettings.AspectRatio = 1.77777777777777777f;	// Default 16/9
}

//////////////////////////////////////////////////////////////////////////
BOOL Camera::IsOrtho() {return bIsOrtho;}

//////////////////////////////////////////////////////////////////////////
void Camera::SetCameraOrtho(BOOL orthographic = true)
{
	if (orthographic)
	{
		mCurrentProjectionMatrix = &mOrthoProj;
		bIsOrtho = true;
	}
	else
	{
		mCurrentProjectionMatrix = &mPerspProj;
		bIsOrtho = false;
	}
}

//////////////////////////////////////////////////////////////////////////
void Camera::UpdateViewMatrix()
{
	Vector3 eyeDir = mLookAt-mPosition;
	mView = Matrix44::LookAt(mPosition, eyeDir, mUp);
}

//////////////////////////////////////////////////////////////////////////
// Updates the current projection matrix
void Camera::UpdateProjMatrix(float sizeWidth, float sizeHeight)
{
	if (bIsOrtho)	UpdateOrthoMatrix(sizeWidth, sizeHeight);
	else			UpdatePerspMatrix();
}

//////////////////////////////////////////////////////////////////////////
// Only updates the perspective matrix
void Camera::UpdatePerspMatrix()
{ mPerspProj = Matrix44::PerspectiveFov(RJE::Math::Deg2Rad_f*mSettings.FOV, mSettings.AspectRatio, mSettings.NearZ, mSettings.FarZ); }

//////////////////////////////////////////////////////////////////////////
// Only updates the orthographic matrix
void Camera::UpdateOrthoMatrix(float sizeWidth, float sizeHeight)
{ mOrthoProj = Matrix44::Orthographic(sizeWidth*mSettings.OrthoZoom, sizeHeight*mSettings.OrthoZoom, mSettings.NearZ, mSettings.FarZ); }
