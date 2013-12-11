#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11TextureManager
{
	ID3D11Device*			mDevice;

	//-----------------------------

	void Initialize( ID3D11Device* device );

	//-----------------------------
	void LoadTexture(string keyName, ID3D11ShaderResourceView** shaderResourceView);
	void Create2DTextureFixedColor(i32 size, RJE_COLOR::Color color, ID3D11ShaderResourceView** textureSRV);
};