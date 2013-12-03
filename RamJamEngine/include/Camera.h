#pragma once

#include "MathHelper.h"

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
	Vector3 mPosition;
	Vector3 mUp;
	Vector3 mLookAt;

	Matrix44  mView;
	Matrix44  mPerspProj;
	Matrix44  mOrthoProj;
	Matrix44* mCurrentProjectionMatrix;

	BOOL bIsOrtho;

	CameraSettings mSettings;

	Camera();
	~Camera() {};

	BOOL IsOrtho();

	void SetCameraOrtho(BOOL);

	void UpdateViewMatrix();
	void UpdateProjMatrix(float, float);
	void UpdateOrthoMatrix(float, float);
	void UpdatePerspMatrix();
};
