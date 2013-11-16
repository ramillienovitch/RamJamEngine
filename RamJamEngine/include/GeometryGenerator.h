//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines a static class for procedurally generating the geometry of 
// common mathematical objects.
//
// All triangles are generated "outward" facing.  If you want "inward" 
// facing triangles (for example, if you want to place the camera inside
// a sphere to simulate a sky), you will need to:
//   1. Change the Direct3D cull mode or manually reverse the winding order.
//   2. Invert the normal.
//   3. Update the texture coordinates and tangent vectors.
//***************************************************************************************

//***************************************************************************************
// Completed by RamJam (C) 2013 All Rights Are Not Reserved mmmmm'Kay ! :D
//***************************************************************************************

#pragma once

#include "MathHelper.h"
#include "MeshData.h"

using namespace MeshData;

class GeometryGenerator
{
public:
	//////////////////////////////////////////////////////////////////////////
	// These primitives are to be used with Triangle List Topology
	// (all these primitives are centered at the origin)
	//
	/// Creates a box centered at the origin with the given dimensions.
	void CreateBox(float width, float height, float depth, Data<PosNormTanTex>& meshData);

	/// Creates a sphere with the given radius.
	/// The slices and stacks parameters control the degree of tessellation.
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData);

	/// Creates a geosphere with the given radius.
	/// The depth controls the level of tessellation.
	void CreateGeosphere(float radius, UINT numSubdivisions, Data<PosNormTanTex>& meshData);

	/// Creates a cylinder parallel to the y-axis
	/// The bottom and top radius can vary to form various cone shapes rather than true cylinders.
	/// The slices and stacks parameters control the degree of tessellation.
	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData);

	/// Creates an mxn grid in the xz-plane with m rows and n columns with the specified width and depth.
	void CreateGrid(float width, float depth, UINT m, UINT n, Data<PosNormTanTex>& meshData);

	/// Creates a quad covering the screen in NDC coordinates.
	/// This is useful for postprocessing effects.
	void CreateFullscreenQuad(Data<PosNormTanTex>& meshData);
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// These primitives are to be used with Line List Topology
	// (all these primitives are centered at the origin)
	//
	void CreateWireBox(float width, float height, float depth, Data<ColorVertex>& meshData, Color color = Color::White);
	void CreateWireSphere(float radius, Data<ColorVertex>& meshData, Color color = Color::White);
	//---------
	void CreateAxisArrows(Data<ColorVertex>& meshData);
	void CreateAxisArrows(Vector3 right, Vector3 up, Vector3 forward, Data<ColorVertex>& meshData);
	void CreateAxisArrows(Quaternion rotation, Data<ColorVertex>& meshData);
	
	/// These primitives are by-default aligned with the z-axis
	void CreateLine(Data<ColorVertex>& meshData, Color color = Color::White);
	void CreateLine(Vector3 orientation, Data<ColorVertex>& meshData, Color color = Color::White);
	//---------
	void CreateRay(Data<ColorVertex>& meshData, Color color = Color::White);
	void CreateRay(Vector3 orientation, Data<ColorVertex>& meshData, Color color = Color::White);
	//---------
	void CreateWireFrustum(float fov, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color = Color::White);
	void CreateWireFrustum(float width, float height, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color = Color::White);
	void CreateWireFrustum(Quaternion orientation, float width, float height, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color = Color::White);
	void CreateWireFrustum(Vector3 right, Vector3 up, Vector3 forward, float width, float height, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color = Color::White);
	//////////////////////////////////////////////////////////////////////////

private:
	void Subdivide(Data<PosNormTanTex>& meshData);
	void BuildCylinderTopCap(   float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData);
};