#pragma once

//////////////////////////////////////////////////////////////////////////
// RapidXML
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
//------
#include <memory>
//------
#include "Types.h"
#include "MathHelper.h"
#include "Transform.h"
#include "GameObject.h"

//////////////////////////////////////////////////////////////////////////
struct SceneLoader
{
	// Load from an XML file scene and search gameobjects
	// Example : 
	//	<gameobject>
	//		<name>dragon</name>
	//		<transform>
	//			<position x="0" y="1" z="0"/>
	//			<rotation x="0" y="45" z="0"/>
	//			<scale    x="0.2" y="0.2" z="0.2"/>
	//		</transform>
	//		<mesh>
	//			<file>dragon.mesh</file>
	//			<material>model.mat</material>
	//		</mesh>
	//		<gizmo>
	//			<primitive>box</primitive>
	//			<parameters width="2" height="2" depth="2"/>
	//			<color a="255" r="0" g="255" b="0"/>
	//		</gizmo>
	//</gameobject>
	void LoadFromFile(const char* pFile, std::vector<unique_ptr<GameObject>>& gameobjects);

	//////////////////////////////////////////////////////////////////////////

private:
	void  ReadScene         (rapidxml::xml_node<>* scene, std::vector<unique_ptr<GameObject>>& gameobjects);
	void  ExtractGameObjects(rapidxml::xml_node<>* node, std::vector<unique_ptr<GameObject>>& gameobjects);
	void  ExtractTransform  (rapidxml::xml_node<>* node, Transform& transform);
	Color ExtractColor      (rapidxml::xml_node<>* node);
	//-------
	void ExtractBox        (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractSphere     (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractGeoSphere  (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractCylinder   (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractGrid       (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractWireBox    (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractWireSphere (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractWireCone   (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractWireFrustum(rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractAxisArrows (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractLine       (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	void ExtractRay        (rapidxml::xml_node<>* node, unique_ptr<GameObject>& gameobject);
	//-------
	// Extract the parameters used by the primitives (width, height, etc.)
	void ExtractParameters(rapidxml::xml_node<>* node, float& f);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, float& f2);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, float& f2, float& f3);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, float& f2, float& f3, float& f4);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, u32& u1);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, u32& u1, u32& u2);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, float& f2, u32& u1, u32& u2);
	void ExtractParameters(rapidxml::xml_node<>* node, float& f1, float& f2, float& f3, u32& u1, u32& u2);
	void ExtractParameters(rapidxml::xml_node<>* node, Vector3& v);
	void ExtractParameters(rapidxml::xml_node<>* node, Vector3& v1, Vector3& v2);
	void ExtractParameters(rapidxml::xml_node<>* node, Vector3& v1, Vector3& v2, Vector3& v3);
	void ExtractParameters(rapidxml::xml_node<>* node, Vector3& v1, Vector3& v2, Vector3& v3, float& f1, float& f2, float& f3, float& f4);
};
