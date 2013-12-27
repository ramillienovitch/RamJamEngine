#pragma once

#include "..\..\RamJamEngine\include\Mesh.h"

//////////////////////////////////////////////////////////////////////////
struct DX11Mesh : Mesh
{
	void Render();
	void Initialize();
	void LoadFromMemory();
	void LoadFromFile();
};
