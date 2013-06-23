#pragma once

#include "DX11Helper.h"

struct DX11DepthBuffer
{
public :
	DX11DepthBuffer();

	ID3D11Texture2D*			mDepthStencilBuffer;
	ID3D11DepthStencilView*		mDepthStencilView;

	void Release();
};
