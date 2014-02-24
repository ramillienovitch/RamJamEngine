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
void Material::DefaultMaterial()
{
	AddPropertyFloat("SpecularAmount", 0.0f);
	AddPropertyFloat("SpecularPower",  0.0f);

	// Texture
	Vector2 tiling = Vector2(1.0f, 1.0);
	Vector2 offset = Vector2(0.0f, 0.0);
	float rotation = 0.0f;
	AddPropertyTexture("Texture_Diffuse", DX11TextureManager::Instance()->mTextures["_default"], tiling, offset, rotation);
}

//////////////////////////////////////////////////////////////////////////
void Material::LoadPropertiesFromFile( std::string filename )
{
	std::string shaderName = CIniFile::GetValue("Name", "shader");
	mIsOpaque = !CIniFile::GetValueBool("Transparency", "properties");
	
	RJE_ASSERT(MaterialFactory::Instance()->IsShaderLoaded(shaderName));
	SetPropertiesFromFactory(shaderName);
}

//////////////////////////////////////////////////////////////////////////
void Material::SetPropertiesFromFactory( std::string shaderName )
{
	std::vector<MaterialFactory::MaterialProperty>& properties = MaterialFactory::Instance()->mFactories[shaderName];

	for (std::vector<MaterialFactory::MaterialProperty>::iterator it = properties.begin() ; it != properties.end(); ++it)
	{
		if (!it->mSemantic.empty())
		{
			std::string& type = it->mType;
			if      ( type == "int")			AddPropertyInt(   it->mSemantic, CIniFile::GetValueInt(    it->mSemantic, "properties"));
			else if ( type == "bool")			AddPropertyBool(  it->mSemantic, CIniFile::GetValueBool(   it->mSemantic, "properties"));
			else if ( type == "float")			AddPropertyFloat( it->mSemantic, CIniFile::GetValueFloat(  it->mSemantic, "properties"));
			else if ( type == "float4")			AddPropertyVector(it->mSemantic, CIniFile::GetValueVector4(it->mSemantic, "properties"));
			else if ( type == "float4x4")		AddPropertyMatrix(it->mSemantic);
			else if ( type == "Texture2D")
			{
				std::string texturePathRel = CIniFile::GetValue(it->mSemantic, "textures");
				std::string texturePathAbs = System::Instance()->mDataPath + texturePathRel;
				int slash = (int)texturePathRel.rfind('\\')+1;
				int point = (int)texturePathRel.find('.');
				std::string textureName = texturePathRel.substr(slash, point-slash);

				if (textureName != "NONE")
				{
					// we prevent loading if we're already using this texture
					if (!DX11TextureManager::Instance()->IsTextureLoaded(textureName))
						DX11TextureManager::Instance()->LoadTexture(texturePathAbs, textureName);

					// we load the texture properties
					Vector2 tiling = CIniFile::GetValueVector2("Tiling", "textures");
					Vector2 offset = CIniFile::GetValueVector2("Offset", "textures");
					float rotation = CIniFile::GetValueFloat("Rotation", "textures");

					AddPropertyTexture(it->mSemantic, DX11TextureManager::Instance()->mTextures[textureName], tiling, offset, rotation);
				}
			}
		}
	}
}
