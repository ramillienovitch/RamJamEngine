#include "Material.h"
#include "Memory.h"
#include "System.h"

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

//////////////////////////////////////////////////////////////////////////
void Material::LoadPropertiesFromFile( std::string filename )
{
	std::string materialPath = System::Instance()->mDataPath + "materials\\" + filename;
	std::string shaderName   = CIniFile::GetValue("Name", "shader", materialPath);
	mIsOpaque = CIniFile::GetValueBool("IsOpaque", "transparency", materialPath);
	
	RJE_ASSERT(MaterialFactory::Instance()->IsShaderLoaded(shaderName));
	SetPropertiesFromFactory(shaderName, materialPath);
}

//////////////////////////////////////////////////////////////////////////
void Material::SetPropertiesFromFactory( std::string shaderName, std::string materialPath )
{
	std::vector<MaterialFactory::MaterialProperty>& properties = MaterialFactory::Instance()->mFactories[shaderName];

	for (std::vector<MaterialFactory::MaterialProperty>::iterator it = properties.begin() ; it != properties.end(); ++it)
	{
		if (!it->mSemantic.empty())
		{
			std::string& type = it->mType;
			if ( type == "int")
			{
				AddPropertyInt(it->mSemantic);
				SetProperty(it->mSemantic, CIniFile::GetValueInt(it->mSemantic, "properties", materialPath));
			}
			else if ( type == "bool")
			{
				AddPropertyBool(it->mSemantic);
				SetProperty(it->mSemantic, CIniFile::GetValueBool(it->mSemantic, "properties", materialPath));
			}
			else if ( type == "float")
			{
				AddPropertyFloat(it->mSemantic);
				SetProperty(it->mSemantic, CIniFile::GetValueFloat(it->mSemantic, "properties", materialPath));
			}
			else if ( type == "float4")
			{
				AddPropertyVector(it->mSemantic);
				SetProperty(it->mSemantic, CIniFile::GetValueVector4(it->mSemantic, "properties", materialPath));
			}
			else if ( type == "float4x4")		AddPropertyMatrix(it->mSemantic);
			else if ( type == "Texture2D")		AddPropertyTexture(it->mSemantic);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Material::AddProperty( std::string propertyName, MaterialPropertyType propertyType, size_t propertyDataLength )
{
	MaterialProperty* property = rje_new MaterialProperty();
	property->mName            = propertyName;
	property->mType            = propertyType;
	property->mData            = malloc (propertyDataLength);
	++mPropertiesCount;
	mProperties.push_back(property);
}
//------------------------------------------------------------
void Material::AddPropertyInt( std::string propertyName )
{ AddProperty(propertyName, MaterialPropertyType::Type_Int, sizeof(int)); }
//------------------
void Material::AddPropertyBool( std::string propertyName )
{ AddProperty(propertyName, MaterialPropertyType::Type_Bool, sizeof(bool)); }
//------------------
void Material::AddPropertyFloat( std::string propertyName )
{ AddProperty(propertyName, MaterialPropertyType::Type_Float, sizeof(float)); }
//------------------
void Material::AddPropertyVector( std::string propertyName )
{ AddProperty(propertyName, MaterialPropertyType::Type_Vector, sizeof(Vector4)); }
//------------------
void Material::AddPropertyMatrix( std::string propertyName )
{ AddProperty(propertyName, MaterialPropertyType::Type_Matrix, sizeof(Matrix44)); }
//------------------------------------------------------------
void Material::AddPropertyTexture( std::string propertyName )
{
	MaterialProperty* property         = rje_new MaterialProperty();
	property->mName                    = propertyName;
	property->mType                    = MaterialPropertyType::Type_Texture;
	property->mShaderResource.mTexture                = nullptr;
	property->mShaderResource.mOffset                 = 0.0f;
	property->mShaderResource.mTiling                 = 0.0f;
	property->mShaderResource.mRotationAngleInDegrees = 0.0f;
	++mPropertiesCount;
	mProperties.push_back(property);
}

//////////////////////////////////////////////////////////////////////////
void Material::SetTexture( std::string propertyName, ShaderResource* shaderResource, Vector2& tiling, Vector2& offset, float rotation)
{
	for ( auto it = mProperties.begin(); it != mProperties.end(); ++it )
	{
		if ((*it)->mName == propertyName)
		{
			(*it)->mShaderResource.mTexture                = shaderResource;
			(*it)->mShaderResource.mTiling                 = tiling;
			(*it)->mShaderResource.mOffset                 = offset;
			(*it)->mShaderResource.mRotationAngleInDegrees = rotation;
			break;
		}
	}
}
