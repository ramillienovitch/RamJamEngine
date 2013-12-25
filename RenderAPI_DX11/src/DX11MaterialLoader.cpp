#include "DX11MaterialLoader.h"
#include "..\..\RamJamEngine\include\System.h"

//////////////////////////////////////////////////////////////////////////
void DX11MaterialLoader::LoadFromFile( Material& material, std::string materialFile )
{
	// first we set all the properties from the material file
	material.LoadPropertiesFromFile(materialFile);
	
	// then we load the textures if there is any
	for ( auto it = material.mProperties.begin(); it != material.mProperties.end(); ++it )
	{
		if ((*it)->mType == MaterialPropertyType::Type_Texture)
		{
			std::string texturePathRel = CIniFile::GetValue((*it)->mName, "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
			std::string texturePathAbs = System::Instance()->mDataPath + texturePathRel;
			int slash = (int)texturePathRel.rfind('\\')+1;
			int point = (int)texturePathRel.find('.');
			std::string textureName = texturePathRel.substr(slash, point-slash);

			if (textureName == "NONE")
				material.SetTexture((*it)->mName, DX11TextureManager::Instance()->mTextures["_default"]);
			else
			{
				if (!DX11TextureManager::Instance()->IsTextureLoaded(textureName))
					DX11TextureManager::Instance()->LoadTexture(texturePathAbs, textureName);

				// we load the texture properties
				Vector2 tiling = CIniFile::GetValueVector2("Tiling", "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
				Vector2 offset = CIniFile::GetValueVector2("Offset", "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
				float rotation = CIniFile::GetValueFloat("Rotation", "textures", System::Instance()->mDataPath + "materials\\" + materialFile);
			
				material.SetTexture((*it)->mName, DX11TextureManager::Instance()->mTextures[textureName], tiling, offset, rotation);
			}
		}
	}
	
}
