#include "Material.h"
#include "Memory.h"

//-------------
Material::Material() { ZeroMemory(this, sizeof(this)); }
//-------------
Material::~Material()
{
	for (u32 i = 0; i < mPropertiesCount; ++i)
	{
		delete mProperties[i];
	}
	mPropertiesCount = 0;
	mProperties.clear();
}

//------------------------------------------------------------
void Material::AddProperty( std::string propertyName, MaterialPropertyType propertyType, size_t propertyDataLength, void* propertyData )
{
	MaterialProperty* property = rje_new MaterialProperty();
	property->mName              = propertyName;
	property->mType              = propertyType;
	property->mData              = malloc (propertyDataLength);
	void* pData = propertyData;
	memcpy(property->mData, pData, propertyDataLength);
	++mPropertiesCount;
	mProperties.push_back(property);
}
//------------------------------------------------------------
void Material::AddPropertyVector( std::string propertyName, Vector4 propertyData )
{
	AddProperty(propertyName, MaterialPropertyType::Type_Vector, sizeof(Vector4), reinterpret_cast<void*>(&propertyData));
}
//------------------------------------------------------------
void Material::AddPropertyTexture( std::string propertyName, ShaderResource* shaderResource )
{
	MaterialProperty* property         = rje_new MaterialProperty();
	property->mName                    = propertyName;
	property->mType                    = MaterialPropertyType::Type_Texture;
	property->mShaderResource.mTexture = shaderResource;
	++mPropertiesCount;
	mProperties.push_back(property);
}
