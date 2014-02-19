#pragma once

#include "MathHelper.h"
#include "Transform.h"

enum CameraMode
{
	Camera_TrackBall,
	Camera_FPS
};

struct CameraSettings
{
	float FOV;
	float OrthoZoom;
	float AspectRatio;
	float NearZ;
	float FarZ;
};

struct Camera
{
	Vector3 mUp;
	Vector3 mLookAt;
	float mPitch;
	float mYaw;
	Transform mTrf;

	Matrix44  mView;
	Matrix44  mPerspProj;
	Matrix44  mOrthoProj;
	Matrix44* mCurrentProjectionMatrix;

	BOOL bIsOrtho;

	CameraMode		mMode;
	CameraSettings	mSettings;

	// TrackBall Camera Settings
	Vector2	mLastMousePos;
	float	mCameraTheta;
	float	mCameraPhi;
	float	mCameraRadius;
	BOOL	mCameraAnimated;
	float	mAnimationSpeed;

	// FPS Camera Setting
	float mSpeed;
	float mSpeedMultiplier;

	Camera();
	~Camera() {};

	BOOL IsOrtho();

	void SetCameraOrtho(BOOL);

	void Init(CameraSettings settings, BOOL ortho);
	void Update();

	void UpdateViewMatrix();
	void UpdateProjMatrix(float, float);
	void UpdateOrthoMatrix(float, float);
	void UpdatePerspMatrix();
};
