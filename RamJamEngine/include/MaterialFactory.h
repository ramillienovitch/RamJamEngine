#pragma once

#include "../../RamJamEngine_Tools/include/Types.h"

//////////////////////////////////////////////////////////////////////////
struct MaterialFactory
{
	struct MaterialProperty
	{
		std::string mName;
		std::string mSemantic;
		std::string mType;
	};

	// Unordered map with <ShaderName, Properties>
	std::unordered_map<std::string, std::vector<MaterialProperty>>	mFactories;
	std::string		mCurrentShader;
	u32				mFactoriesCount;

	//-------------------------------------------------

	BOOL IsShaderLoaded(std::string shaderName);

	void RegisterShader(std::string shaderName);
	void RegisterProperty(std::string mName, std::string mType);
	void RegisterProperty(std::string mName, std::string mSemantic, std::string mType);

	//-------------------------------------------------

	MaterialFactory() : mFactoriesCount(0) {}
	//------
	static MaterialFactory* Instance()
	{
		if(!sInstance)
			sInstance = new MaterialFactory();

		return sInstance;
	}
	//------
	static void DeleteInstance()
	{
		if(sInstance)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

private:
	static MaterialFactory* sInstance;
};
