#pragma once

#include "MathHelper.h"
#include "Texture.h"

enum MaterialPropertyType
{
	Type_Int = 0x0,
	Type_Bool,
	Type_Float,
	Type_Vector,
	Type_Matrix,
	Type_Texture,
	Type_Cubemap,
	
	//  This value is not used. It forces the compiler to use at least 32 Bit integers to represent this enum.
	Type_Force32Bit  = 0x9fffffff
};

// enum TextureType
// {
// 	TextureType_NONE         = 0x0,
// 	TextureType_DIFFUSE      = 0x1,
// 	TextureType_SPECULAR     = 0x2,
// 	TextureType_AMBIENT      = 0x3,
// 	TextureType_EMISSIVE     = 0x4,
// 	TextureType_HEIGHT       = 0x5,
// 	TextureType_NORMALS      = 0x6,
// 	TextureType_SHININESS    = 0x7,
// 	TextureType_OPACITY      = 0x8,
// 	TextureType_DISPLACEMENT = 0x9,
// 	TextureType_LIGHTMAP     = 0xA,
// 	TextureType_REFLECTION   = 0xB,
// 	TextureType_UNKNOWN      = 0xC,
// 
// 	//  This value is not used. It forces the compiler to use at least 32 Bit integers to represent this enum.
// 	TextureType_Force32Bit   = 0x9fffffff
// };

//////////////////////////////////////////////////////////////////////////
struct MaterialProperty
{
	std::string				mName;
	MaterialPropertyType	mType;

	// For non-texture properties, these members are always 0 
// 	u32	mTextureUsageSemantic;
// 	u32	mTextureIndex;

	void*			mData;
	Texture			mShaderResource;

	MaterialProperty()
	{
		mData = nullptr;
		//mTextureIndex = mTextureUsageSemantic = 0;
	}

	~MaterialProperty()
	{
		if (mData != nullptr)
			free(mData);
	}
};

//////////////////////////////////////////////////////////////////////////
struct Material
{
	Material();
	~Material();

	u32 mPropertiesCount;
	std::vector<MaterialProperty*> mProperties;

	//----------------------------------

	void LoadPropertiesFromFile  (std::string filename);
	void SetPropertiesFromFactory(std::string shaderName);
	//-------
	template <typename T>
	void SetProperty       (std::string propertyName, T propertyData);
	template <typename T>
	T    GetProperty       (std::string propertyName);
	void AddProperty       (std::string propertyName, MaterialPropertyType propertyType, u64 propertyDataLength);
	//-------
	void AddPropertyInt    (std::string propertyName);
	void AddPropertyBool   (std::string propertyName);
	void AddPropertyFloat  (std::string propertyName);
	void AddPropertyVector (std::string propertyName);
	void AddPropertyMatrix (std::string propertyName);
	//-------
	void AddPropertyTexture(std::string propertyName);
	void SetTexture        (std::string propertyName, ShaderResource* shaderResource);
	void SetTexture        (MaterialProperty& property, ShaderResource* shaderResource);
};

template <typename T>
void Material::SetProperty( std::string propertyName, T propertyData )
{
	for ( auto it = mProperties.begin(); it != mProperties.end(); ++it )
	{
		if ((*it)->mName == propertyName)
			memcpy((*it)->mData, reinterpret_cast<void*>(&propertyData), sizeof(T));
	}
}

// template <typename T>
// T Material::GetProperty( std::string propertyName )
// {
// 
// }
