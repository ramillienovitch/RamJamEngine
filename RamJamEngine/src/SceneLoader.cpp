#include "SceneLoader.h"
#include "System.h"

using namespace rapidxml;

//////////////////////////////////////////////////////////////////////////
void SceneLoader::LoadFromFile( const char* pFilename, std::vector<unique_ptr<GameObject>>& gameobjects )
{
	file<>			xmlFile(pFilename);
	xml_document<>	xmlDoc;
	
	xmlDoc.parse<0>(xmlFile.data());    // 0 means default parse flags

	if (strcmp(xmlDoc.first_node()->name(), "scene") == 0)
		ReadScene(xmlDoc.first_node(), gameobjects);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ReadScene( xml_node<>* scene, std::vector<unique_ptr<GameObject>>& gameobjects )
{
	for (xml_node<> *node = scene->first_node(); node; node = node->next_sibling())
	{
		if (strcmp(node->name(), "gameobject") == 0)
			ExtractGameObjects(node, gameobjects);
	}
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractGameObjects(xml_node<>* gameobjectNode, std::vector<unique_ptr<GameObject>>& gameobjects)
{
	unique_ptr<GameObject> gameobject (new GameObject);
	for (xml_node<> *node = gameobjectNode->first_node(); node; node = node->next_sibling())
	{
		//===== NAME =====
		if (strcmp(node->name(), "name") == 0)
			gameobject->mName = string(node->value());
		
		//===== TRANSFORM =====
		if (strcmp(node->name(), "transform") == 0)
			ExtractTransform(node, gameobject->mTransform);
		
		//===== MESH =====
		if (strcmp(node->name(), "mesh") == 0)
		{
#if (RJE_GRAPHIC_API == DIRECTX_11)
			gameobject->mDrawable.mMesh = rje_new DX11Mesh;
#else
			gameobject->mDrawable.mMesh = rje_new OglMesh;
#endif
			//===== FILE ===
			if (strcmp(node->first_node()->name(), "file") == 0)
			{
				string meshPath     = System::Instance()->mDataPath + "models\\" + string(node->first_node()->value());
				string materialFile = string(node->first_node()->next_sibling()->value());
				gameobject->mDrawable.mMesh->LoadModelFromFile(meshPath);
				gameobject->mDrawable.mMesh->LoadMaterialLibraryFromFile(materialFile);
			}
			//===== PRIMITIVE ===
			if (strcmp(node->first_node()->name(), "primitive") == 0)
			{
				if (strcmp(node->first_node()->value(), "box")       == 0)		ExtractBox(      node->first_node()->next_sibling(), gameobject);
				if (strcmp(node->first_node()->value(), "sphere")    == 0)		ExtractSphere(   node->first_node()->next_sibling(), gameobject);
				if (strcmp(node->first_node()->value(), "geosphere") == 0)		ExtractGeoSphere(node->first_node()->next_sibling(), gameobject);
				if (strcmp(node->first_node()->value(), "cylinder")  == 0)		ExtractCylinder( node->first_node()->next_sibling(), gameobject);
				if (strcmp(node->first_node()->value(), "grid")      == 0)		ExtractGrid(     node->first_node()->next_sibling(), gameobject);
				string materialFile = string(node->first_node()->next_sibling()->next_sibling()->value());
				gameobject->mDrawable.mMesh->CheckMaterialFile(materialFile);
				gameobject->mDrawable.mMesh->LoadMaterialFromFile(materialFile);
			}
		}

		//===== GIZMO =====
		if (strcmp(node->name(), "gizmo") == 0)
		{
#if (RJE_GRAPHIC_API == DIRECTX_11)
			gameobject->mDrawable.mGizmo = rje_new DX11Mesh;
#else
			gameobject->mDrawable.mGizmo = rje_new OglMesh;
#endif
			//===== PRIMITIVE ===
			if (strcmp(node->first_node()->value(), "box")     == 0)		ExtractWireBox(    node->first_node()->next_sibling(), gameobject);
			if (strcmp(node->first_node()->value(), "sphere")  == 0)		ExtractWireSphere( node->first_node()->next_sibling(), gameobject);
			if (strcmp(node->first_node()->value(), "cone")    == 0)		ExtractWireCone(   node->first_node()->next_sibling(), gameobject);
			if (strcmp(node->first_node()->value(), "frustum") == 0)		ExtractWireFrustum(node->first_node()->next_sibling(), gameobject);
			if (strcmp(node->first_node()->value(), "arrows")  == 0)		ExtractAxisArrows( node->first_node()->next_sibling(), gameobject);
			if (strcmp(node->first_node()->value(), "line")    == 0)		ExtractLine(       node->first_node()->next_sibling(), gameobject);
			if (strcmp(node->first_node()->value(), "ray")     == 0)		ExtractRay(        node->first_node()->next_sibling(), gameobject);
			//------
			gameobject->mDrawable.mGizmoColor = ExtractColor(node->first_node()->next_sibling()->next_sibling());
		}
	}
	gameobjects.push_back(std::move(gameobject));
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractTransform(xml_node<>* node, Transform& transform)
{
	xml_node<>* positionNode = node->first_node();
	xml_node<>* rotationNode = positionNode->next_sibling();
	xml_node<>* scaleNode    = rotationNode->next_sibling();

	Vector3 rotation;
	ExtractParameters(positionNode, transform.Position);
	ExtractParameters(scaleNode,    transform.Scale);
	ExtractParameters(rotationNode, rotation);
	transform.Rotation        = Quaternion(rotation);
	transform.WorldMat        = transform.WorldMatrix();
	transform.WorldMatNoScale = transform.WorldMatrixNoScale();
}

//////////////////////////////////////////////////////////////////////////
Color SceneLoader::ExtractColor( xml_node<>* node )
{
	u8 a,r,g,b;
	xml_attribute<> *attr = node->first_attribute();
	a = (u8) atoi(attr->value());		attr = attr->next_attribute();
	r = (u8) atoi(attr->value());		attr = attr->next_attribute();
	g = (u8) atoi(attr->value());		attr = attr->next_attribute();
	b = (u8) atoi(attr->value());
	return Color(a,r,g,b);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractBox(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float width  = 0.0f;
	float height = 0.0f;
	float depth  = 0.0f;
	ExtractParameters(node, width, height, depth);
	gameobject->mDrawable.mMesh->LoadBox(width, height, depth);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractSphere(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float radius   = 0.0f;
	u32 sliceCount = 0;
	u32 stackCount = 0;
	ExtractParameters(node, radius, sliceCount, stackCount);
	gameobject->mDrawable.mMesh->LoadSphere(radius, sliceCount, stackCount);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractGeoSphere(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float radius = 0.0f;
	u32 subdiv   = 0;
	ExtractParameters(node, radius, subdiv);
	gameobject->mDrawable.mMesh->LoadGeoSphere(radius, subdiv);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractCylinder(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float bottomRadius = 0.0f;
	float topRadius    = 0.0f;
	float height       = 0.0f;
	u32 sliceCount     = 0;
	u32 stackCount     = 0;
	ExtractParameters(node, bottomRadius, topRadius, height, sliceCount, stackCount);
	gameobject->mDrawable.mMesh->LoadCylinder(bottomRadius, topRadius, height, sliceCount, stackCount);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractGrid(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float width = 0.0f;
	float depth = 0.0f;
	u32 rows    = 0;
	u32 columns = 0;
	ExtractParameters(node, width, depth, rows, columns);
	gameobject->mDrawable.mMesh->LoadGrid(width, depth, rows, columns);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractWireBox(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float width  = 0.0f;
	float height = 0.0f;
	float depth  = 0.0f;
	ExtractParameters(node, width, height, depth);
	gameobject->mDrawable.mGizmo->LoadWireBox(width, height, depth);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractWireSphere(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float radius = 0.0f;
	ExtractParameters(node, radius);
	gameobject->mDrawable.mGizmo->LoadWireSphere(radius);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractWireCone(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	float length = 0.0f;
	float angle  = 0.0f;
	ExtractParameters(node, length, angle);
	gameobject->mDrawable.mGizmo->LoadWireCone(length, angle);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractWireFrustum(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	Vector3 right;
	Vector3 up;
	Vector3 forward;
	float fovX           = 0.0f;
	float ratio          = 0.0f;
	float nearPlaneDepth = 0.0f;
	float farPlaneDepth  = 0.0f;
	ExtractParameters(node, right, up, forward, fovX, ratio, nearPlaneDepth, farPlaneDepth);
	gameobject->mDrawable.mGizmo->LoadWireFrustum(right, up, forward, fovX, ratio, nearPlaneDepth, farPlaneDepth);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractAxisArrows(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	Vector3 right;
	Vector3 up;
	Vector3 forward;
	ExtractParameters(node, right, up, forward);
	gameobject->mDrawable.mGizmo->LoadAxisArrows(right, up, forward);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractLine(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	Vector3 start;
	Vector3 end;
	ExtractParameters(node, start, end);
	gameobject->mDrawable.mGizmo->LoadLine(start, end);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractRay(xml_node<>* node, unique_ptr<GameObject>& gameobject)
{
	Vector3 start;
	Vector3 orientation;
	ExtractParameters(node, start, orientation);
	gameobject->mDrawable.mGizmo->LoadRay(start, orientation);
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f)
{
	xml_attribute<> *attr = node->first_attribute();
	f = (f32) atof(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, float& f2)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());
	attr = attr->next_attribute();
	f2 = (f32) atof(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, float& f2, float& f3)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f2 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f3 = (f32) atof(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, float& f2, float& f3, float& f4)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f2 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f3 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f4 = (f32) atof(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, u32& u1)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());
	attr = attr->next_attribute();
	u1 = (u32) atoi(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, u32& u1, u32& u2)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());		attr = attr->next_attribute();
	u1 = (u32) atoi(attr->value());		attr = attr->next_attribute();
	u2 = (u32) atoi(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, float& f2, u32& u1, u32& u2)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f2 = (f32) atof(attr->value());		attr = attr->next_attribute();
	u1 = (u32) atoi(attr->value());		attr = attr->next_attribute();
	u2 = (u32) atoi(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, float& f1, float& f2, float& f3, u32& u1, u32& u2)
{
	xml_attribute<> *attr = node->first_attribute();
	f1 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f2 = (f32) atof(attr->value());		attr = attr->next_attribute();
	f3 = (f32) atof(attr->value());		attr = attr->next_attribute();
	u1 = (u32) atoi(attr->value());		attr = attr->next_attribute();
	u2 = (u32) atoi(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, Vector3& v)
{ ExtractParameters(node, v.x, v.y, v.z); }

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, Vector3& v1, Vector3& v2)
{
	xml_attribute<> *attr = node->first_attribute();
	v1.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v1.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v1.z = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.z = (f32) atof(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, Vector3& v1, Vector3& v2, Vector3& v3)
{
	xml_attribute<> *attr = node->first_attribute();
	v1.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v1.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v1.z = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.z = (f32) atof(attr->value());	attr = attr->next_attribute();
	v3.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v3.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v3.z = (f32) atof(attr->value());
}

//////////////////////////////////////////////////////////////////////////
void SceneLoader::ExtractParameters(xml_node<>* node, Vector3& v1, Vector3& v2, Vector3& v3, float& f1, float& f2, float& f3, float& f4)
{
	xml_attribute<> *attr = node->first_attribute();
	v1.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v1.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v1.z = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v2.z = (f32) atof(attr->value());	attr = attr->next_attribute();
	v3.x = (f32) atof(attr->value());	attr = attr->next_attribute();
	v3.y = (f32) atof(attr->value());	attr = attr->next_attribute();
	v3.z = (f32) atof(attr->value());	attr = attr->next_attribute();
	f1   = (f32) atof(attr->value());	attr = attr->next_attribute();
	f2   = (f32) atof(attr->value());	attr = attr->next_attribute();
	f3   = (f32) atof(attr->value());	attr = attr->next_attribute();
	f4   = (f32) atof(attr->value());
}
