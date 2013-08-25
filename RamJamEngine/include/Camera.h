#pragma once

#include "MathHelper.h"

struct Camera
{
	Vector3 mPosition;
	Vector3 mUp;
	Vector3 mLookAt;

	Matrix44  mView;
	Matrix44  mPerspProj;
	Matrix44  mOrthoProj;
	Matrix44* mCurrentProjectionMatrix;

	BOOL bIsOrtho;

	float mFOV;
	float mOrthoZoom;
	float mNearZ;
	float mFarZ;
	float mAspectRatio;

	virtual BOOL IsOrtho() = 0;

	virtual void SetCameraOrtho(BOOL) = 0;

	virtual void UpdateViewMatrix()              = 0;
	virtual void UpdateProjMatrix(float, float)  = 0;
	virtual void UpdateOrthoMatrix(float, float) = 0;
	virtual void UpdatePerspMatrix()             = 0;

	virtual void SetCameraSettings(float, float, float, float, float, float) = 0;
};
