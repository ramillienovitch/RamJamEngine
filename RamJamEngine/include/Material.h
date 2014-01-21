#pragma once

#include "MathHelper.h"
#include "Texture.h"
#include "Transform.h"

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
		// TODO: handle the case with several objects per material
		if (mData != nullptr)
		{
			free(mData);
			mData = nullptr;
		}
	}
};

//////////////////////////////////////////////////////////////////////////
struct Material
{
	Material();
	~Material();

	u32 mPropertiesCount;
	std::vector<MaterialProperty*> mProperties;
	BOOL mIsOpaque;

	//----------------------------------

	void LoadPropertiesFromFile  (std::string filename);
	void SetPropertiesFromFactory(std::string shaderName);
	//-------
	void AddProperty       (std::string propertyName, MaterialPropertyType propertyType, u64 propertyDataLength, void* propertyData);
	void AddPropertyInt    (std::string propertyName, int     propertyData);
	void AddPropertyBool   (std::string propertyName, BOOL    propertyData);
	void AddPropertyFloat  (std::string propertyName, float   propertyData);
	void AddPropertyVector (std::string propertyName, Vector4 propertyData);
	void AddPropertyMatrix (std::string propertyName);
	void AddPropertyTexture(std::string propertyName, ShaderResource* shaderResource = nullptr, Vector2& tiling = Vector2(1,1), Vector2& offset = Vector2(0,0), float rotation = 0.0f);
};

//////////////////////////////////////////////////////////////////////////
FORCEINLINE void Material::AddProperty( std::string propertyName, MaterialPropertyType propertyType, u64 propertyDataLength, void* propertyData )
{
	MaterialProperty* property = new MaterialProperty();
	property->mName            = propertyName;
	property->mType            = propertyType;
	property->mData            = malloc (propertyDataLength);
	if (propertyData)
		memcpy(property->mData, propertyData, propertyDataLength);
	++mPropertiesCount;
	mProperties.push_back(property);
}
//------------------------------------------------------------
FORCEINLINE void Material::AddPropertyInt( std::string propertyName, int propertyData )
{ AddProperty(propertyName, MaterialPropertyType::Type_Int, sizeof(int), reinterpret_cast<void*>(&propertyData)); }
//------------------
FORCEINLINE void Material::AddPropertyBool( std::string propertyName, BOOL propertyData )
{ AddProperty(propertyName, MaterialPropertyType::Type_Bool, sizeof(bool), reinterpret_cast<void*>(&propertyData)); }
//------------------
FORCEINLINE void Material::AddPropertyFloat( std::string propertyName, float propertyData )
{ AddProperty(propertyName, MaterialPropertyType::Type_Float, sizeof(float), reinterpret_cast<void*>(&propertyData)); }
//------------------
FORCEINLINE void Material::AddPropertyVector( std::string propertyName, Vector4 propertyData )
{ AddProperty(propertyName, MaterialPropertyType::Type_Vector, sizeof(Vector4), reinterpret_cast<void*>(&propertyData)); }
//------------------
FORCEINLINE void Material::AddPropertyMatrix( std::string propertyName )
{ AddProperty(propertyName, MaterialPropertyType::Type_Matrix, sizeof(Matrix44), nullptr); }
//------------------------------------------------------------
FORCEINLINE void Material::AddPropertyTexture( std::string propertyName, ShaderResource* shaderResource /*= nullptr*/, Vector2& tiling /*= Vector2(1,1)*/, Vector2& offset /*= Vector2(0,0)*/, float rotation /*= 0.0f*/ )
{
	MaterialProperty* property                        = new MaterialProperty();
	property->mName                                   = propertyName;
	property->mType                                   = MaterialPropertyType::Type_Texture;
	property->mShaderResource.mTexture                = shaderResource;
	property->mShaderResource.mOffset                 = offset;
	property->mShaderResource.mTiling                 = tiling;
	property->mShaderResource.mRotationAngleInDegrees = rotation;
	property->mShaderResource.mTextureMatrix          = Transform::MatrixFromTextureProperties(tiling, offset, rotation);
	++mPropertiesCount;
	mProperties.push_back(property);
}
