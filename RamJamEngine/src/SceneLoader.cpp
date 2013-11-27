#include "SceneLoader.h"

//////////////////////////////////////////////////////////////////////////
void SceneLoader::LoadFromFile( const char* pFilename )
{
	rapidxml::file<>			xmlFile(pFilename);
	rapidxml::xml_document<>	xmlDoc;
	
	xmlDoc.parse<0>(xmlFile.data());    // 0 means default parse flags

	std::cout << "Name of my first node is: " << xmlDoc.first_node()->name() << "\n";
	rapidxml::xml_node<> *node = xmlDoc.first_node();
	for (rapidxml::xml_node<> *attr = node->first_node();
		attr; attr = attr->next_sibling())
	{
		std::cout << "Node foobar has node " << attr->name() << " ";
		std::cout << "with value " << attr->value() << "\n";
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
