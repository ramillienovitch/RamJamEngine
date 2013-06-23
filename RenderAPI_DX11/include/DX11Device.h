#pragma once

#include "DX11Helper.h"

struct DX11Device
{
	DX11Device();

	ID3D11Device*			md3dDevice;
	ID3D11DeviceContext*	md3dImmediateContext;

	void Release();
};