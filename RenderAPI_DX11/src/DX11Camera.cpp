#include "DX11Camera.h"

//////////////////////////////////////////////////////////////////////////
DX11Camera::DX11Camera()
{
	mPosition = Vector3::zero;
	mUp       = Vector3(0.0f, 1.0f, 0.0f);
	mLookAt   = Vector3::zero;

	mView      = Matrix44::identity;
	mPerspProj = Matrix44::identity;
	mOrthoProj = Matrix44::identity;
	mCurrentProjectionMatrix = &mPerspProj;

	bIsOrtho = false;

	mFOV         = 80;
	mOrthoZoom   = 0.01f;
	mNearZ       = 0.01f;
	mFarZ        = 1000.0f;
	mAspectRatio = 1.77777777777777777f;	// Default 16/9
}

DX11Camera::~DX11Camera() {}

//////////////////////////////////////////////////////////////////////////
BOOL DX11Camera::IsOrtho() {return bIsOrtho;}

//////////////////////////////////////////////////////////////////////////
void DX11Camera::SetCameraOrtho(BOOL orthographic = true)
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
void DX11Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR pos    = DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(mLookAt.x,   mLookAt.y,   mLookAt.z,   0.0f);
	DirectX::XMVECTOR up     = DirectX::XMVectorSet(mUp.x,       mUp.y,       mUp.z,       0.0f);

	mView = DirectX::XMMatrixLookAtLH(pos, target, up);
}

//////////////////////////////////////////////////////////////////////////
// Updates the current projection matrix
void DX11Camera::UpdateProjMatrix(float sizeWidth, float sizeHeight)
{
	if (bIsOrtho)	UpdateOrthoMatrix(sizeWidth, sizeHeight);
	else			UpdatePerspMatrix();
}

//////////////////////////////////////////////////////////////////////////
// Only updates the perspective matrix
void DX11Camera::UpdatePerspMatrix()
{
	mPerspProj = DirectX::XMMatrixPerspectiveFovLH(DegreesToRadian_F(mFOV), mAspectRatio, mNearZ, mFarZ);
}

//////////////////////////////////////////////////////////////////////////
// Only updates the orthographic matrix
void DX11Camera::UpdateOrthoMatrix(float sizeWidth, float sizeHeight)
{
	mOrthoProj = DirectX::XMMatrixOrthographicLH(sizeWidth*mOrthoZoom, sizeHeight*mOrthoZoom, mNearZ, mFarZ);
}

//////////////////////////////////////////////////////////////////////////
void DX11Camera::SetCameraSettings(float FOV, float orthoZoom, float sizeWidth, float sizeHeight, float nearZ, float farZ)
{
	mFOV         = FOV;
	mOrthoZoom   = orthoZoom;
	mAspectRatio = sizeWidth/sizeHeight;
	mNearZ       = nearZ;
	mFarZ        = farZ;
}