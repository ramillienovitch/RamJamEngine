#pragma once

// This interface is the main entry for the graphic API.
// It is the linker in between DirectX/OpenGL and the engine.
// Instead of using the API directly, we have to use this interface.
// This enable us to work without knowing the API used behind

struct GraphicAPI
{
	//virtual ~GraphicAPI();

	virtual void Initialize(HWND hMainWnd, int windowWidth, int windowHeight) = 0;
	virtual void UpdateScene( float dt, float theta, float phi, float radius) = 0;
	virtual void DrawScene() = 0;
	virtual void Shutdown() = 0;
	virtual void ResizeWindow(int newSizeWidth, int newSizeHeight) = 0;
};