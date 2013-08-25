#pragma once

#include "DX11Helper.h"
#include "../../RamJamEngine/include/Camera.h"

struct DX11Camera : Camera
{
	DX11Camera();
	~DX11Camera();

	virtual BOOL IsOrtho();

	virtual void SetCameraOrtho(BOOL);

	virtual void UpdateViewMatrix();
	virtual void UpdateProjMatrix(float, float);
	virtual void UpdateOrthoMatrix(float, float);
	virtual void UpdatePerspMatrix();

	virtual void SetCameraSettings(float, float, float, float, float, float);
};