#include "SceneLoader.h"

//////////////////////////////////////////////////////////////////////////
void SceneLoader::LoadFromFile( const char* pFilename )
{
	rapidxml::file<>			xmlFile(pFilename);
	rapidxml::xml_document<>	xmlDoc;
	
	xmlDoc.parse<0>(xmlFile.data());    // 0 means default parse flags

	rapidxml::xml_node<> *rootNode = xmlDoc.first_node();
	for (rapidxml::xml_node<> *node = rootNode->first_node(); node; node = node->next_sibling())
	{
		std::cout << "Node foobar has node " << node->name() << " and ";
		std::cout << count_children(node)<< "nodes\n";
	}

	
	std::ofstream myfile;
	myfile.open ("sceneout.xml");

	myfile << xmlDoc;

	myfile.close();
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractGameObjects()
{

}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractTransform(rapidxml::xml_node<> transformNode)
{

}