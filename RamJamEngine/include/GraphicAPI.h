#pragma once

// This interface is the main entry for the graphic API.
// It is the linker in between DirectX/OpenGL and the engine.
// Instead of using the API directly, we have to use this interface.
// This enable us to work without knowing the API used behind

#include "Camera.h"
#include "MeshData.h"
#include "GeometryGenerator.h"

#define MAX_LIGHTS_POWER 10
#define MAX_LIGHTS (1<<MAX_LIGHTS_POWER)

struct GraphicAPI
{
	Camera* mCamera;

	virtual void Initialize(int windowWidth, int windowHeight) = 0;
	virtual void UpdateScene( float dt ) = 0;
	virtual void DrawScene() = 0;
	virtual void Shutdown() = 0;
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight) = 0;

	//-----------

	virtual void SetWireframe(BOOL state) = 0;
	virtual void SetMSAA(UINT MSAA_Samples) = 0;
};