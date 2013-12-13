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

			if (!DX11TextureManager::Instance()->IsTextureLoaded(textureName))
				DX11TextureManager::Instance()->LoadTexture(texturePathAbs, textureName);
				
			material.SetTexture((*it)->mName, DX11TextureManager::Instance()->mTextures[textureName]);
		}
	}
	
}
