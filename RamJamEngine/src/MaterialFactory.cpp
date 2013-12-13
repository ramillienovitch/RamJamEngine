#include "MaterialFactory.h"

#include "Debug.h"

MaterialFactory* MaterialFactory::sInstance = nullptr;

//////////////////////////////////////////////////////////////////////////
BOOL MaterialFactory::IsShaderLoaded( std::string shaderName )
{
	std::unordered_map<std::string,std::vector<MaterialProperty>>::const_iterator factory = mFactories.find(shaderName);

	return (factory != mFactories.end());
}

//////////////////////////////////////////////////////////////////////////
void MaterialFactory::RegisterShader( std::string shaderName )
{
	++mFactoriesCount;
	
	// retrieve only the file name without the extension nor the filepath
	size_t last      = shaderName.rfind('\\');
	size_t len       = shaderName.find('.') - last - 1;
	std::string file = shaderName.substr(shaderName.rfind('\\')+1, len);

	if ( !IsShaderLoaded(file) )
	{
		// Adding shader in the factory
		mCurrentShader = file;
		std::vector<MaterialProperty> default;
		mFactories[file] = default;
	}
	else
	{
		// The shader was already loaded, this is not supposed to happen
		RJE_MESSAGE_BOX(NULL, L"A shader was already registered twice !", L"Material Factory", MB_ICONWARNING | MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////
void MaterialFactory::RegisterProperty( std::string name, std::string type )
{ RegisterProperty(name, std::string(), type); }
//---------------
void MaterialFactory::RegisterProperty( std::string name, std::string semantic, std::string type )
{
	MaterialProperty matProperties;
	matProperties.mName     = name;
	matProperties.mSemantic = semantic;
	matProperties.mType     = type;
	mFactories[mCurrentShader].push_back(matProperties);
}
