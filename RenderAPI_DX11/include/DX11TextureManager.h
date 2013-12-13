#pragma once

#include "DX11Helper.h"

//////////////////////////////////////////////////////////////////////////
struct DX11TextureManager
{
	ID3D11Device*	mDevice;
	u32				mTextureCount;
	std::unordered_map<std::string, ID3D11ShaderResourceView*>	mTextures;

	//-----------------------------

	DX11TextureManager() : mTextureCount(0) {}

	//-----------------------------

	void Initialize( ID3D11Device* device );
	void ReleaseTextures();
	//------------
	BOOL IsTextureLoaded(std::string textureName);
	void LoadTexture(string texturePath, string textureName);
	void LoadTexture(string keyName, ID3D11ShaderResourceView** shaderResourceView);
	void Create2DTextureFixedColor(i32 size, RJE_COLOR::Color color, std::string textureName);

	//------
	static DX11TextureManager* Instance()
	{
		if(!sInstance)
			sInstance = new DX11TextureManager();

		return sInstance;
	}
	//------
	static void DeleteInstance()
	{
		if(sInstance)
		{
			sInstance->ReleaseTextures();
			delete sInstance;
			sInstance = nullptr;
		}
	}

private:
	static DX11TextureManager* sInstance;
};
