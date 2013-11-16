//***************************************************************************************
// GeometryGenerator.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "GeometryGenerator.h"

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateBox(float width, float height, float depth, Data<PosNormTanTex>& meshData)
{
	//
	// Create the vertices.
	//

	MeshData::PosNormTanTex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;
	
	// Fill in the front face vertex data.
	v[0] = MeshData::PosNormTanTex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = MeshData::PosNormTanTex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = MeshData::PosNormTanTex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = MeshData::PosNormTanTex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = MeshData::PosNormTanTex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = MeshData::PosNormTanTex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = MeshData::PosNormTanTex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = MeshData::PosNormTanTex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = MeshData::PosNormTanTex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = MeshData::PosNormTanTex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = MeshData::PosNormTanTex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = MeshData::PosNormTanTex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = MeshData::PosNormTanTex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = MeshData::PosNormTanTex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = MeshData::PosNormTanTex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = MeshData::PosNormTanTex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = MeshData::PosNormTanTex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = MeshData::PosNormTanTex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = MeshData::PosNormTanTex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = MeshData::PosNormTanTex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = MeshData::PosNormTanTex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = MeshData::PosNormTanTex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = MeshData::PosNormTanTex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = MeshData::PosNormTanTex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.Vertices.assign(&v[0], &v[24]);
 
	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	MeshData::PosNormTanTex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	MeshData::PosNormTanTex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back( topVertex );

	float phiStep   = RJE_PI_F/stackCount;
	float thetaStep = 2.0f*RJE_PI_F/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			MeshData::PosNormTanTex v;

			// spherical to cartesian
			v.Position.x = radius*sinf(phi)*cosf(theta);
			v.Position.y = radius*cosf(phi);
			v.Position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius*sinf(phi)*sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius*sinf(phi)*cosf(theta);

			v.TangentU.Normalize();
			v.Normal.Normalize();

			v.TexC.x = theta / RJE_TWO_PI_F;
			v.TexC.y = phi / RJE_PI_F;

			meshData.Vertices.push_back( v );
		}
	}

	meshData.Vertices.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for(UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i+1);
		meshData.Indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount+1;
	for(UINT i = 0; i < stackCount-2; ++i)
	{
		for(UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.Vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex+i);
		meshData.Indices.push_back(baseIndex+i+1);
	}
}
 
//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::Subdivide(Data<PosNormTanTex>& meshData)
{
	// Save a copy of the input geometry.
	Data<PosNormTanTex> inputCopy = meshData;


	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	size_t numTris = inputCopy.Indices.size()/3;
	for(UINT i = 0; i < (UINT)numTris; ++i)
	{
		MeshData::PosNormTanTex v0 = inputCopy.Vertices[ inputCopy.Indices[i*3+0] ];
		MeshData::PosNormTanTex v1 = inputCopy.Vertices[ inputCopy.Indices[i*3+1] ];
		MeshData::PosNormTanTex v2 = inputCopy.Vertices[ inputCopy.Indices[i*3+2] ];

		//
		// Generate the midpoints.
		//

		MeshData::PosNormTanTex m0, m1, m2;

		// For subdivision, we just care about the position component.
		// We derive the other vertex components in CreateGeosphere.

		m0.Position = Vector3(	0.5f*(v0.Position.x + v1.Position.x),
								0.5f*(v0.Position.y + v1.Position.y),
								0.5f*(v0.Position.z + v1.Position.z));

		m1.Position = Vector3(	0.5f*(v1.Position.x + v2.Position.x),
								0.5f*(v1.Position.y + v2.Position.y),
								0.5f*(v1.Position.z + v2.Position.z));

		m2.Position = Vector3(	0.5f*(v0.Position.x + v2.Position.x),
								0.5f*(v0.Position.y + v2.Position.y),
								0.5f*(v0.Position.z + v2.Position.z));

		//
		// Add new geometry.
		//

		meshData.Vertices.push_back(v0); // 0
		meshData.Vertices.push_back(v1); // 1
		meshData.Vertices.push_back(v2); // 2
		meshData.Vertices.push_back(m0); // 3
		meshData.Vertices.push_back(m1); // 4
		meshData.Vertices.push_back(m2); // 5
 
		meshData.Indices.push_back(i*6+0);
		meshData.Indices.push_back(i*6+3);
		meshData.Indices.push_back(i*6+5);

		meshData.Indices.push_back(i*6+3);
		meshData.Indices.push_back(i*6+4);
		meshData.Indices.push_back(i*6+5);

		meshData.Indices.push_back(i*6+5);
		meshData.Indices.push_back(i*6+4);
		meshData.Indices.push_back(i*6+2);

		meshData.Indices.push_back(i*6+3);
		meshData.Indices.push_back(i*6+1);
		meshData.Indices.push_back(i*6+4);
	}
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateGeosphere(float radius, UINT numSubdivisions, Data<PosNormTanTex>& meshData)
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = RJE::Math::Min(numSubdivisions, 5u);

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	Vector3 pos[12] = 
	{
		Vector3(-X,   0.0f, Z),			Vector3(X,    0.0f, Z),
		Vector3(-X,   0.0f, -Z),		Vector3(X,    0.0f, -Z),
		Vector3(0.0f, Z,    X),			Vector3(0.0f, Z,    -X),
		Vector3(0.0f, -Z,   X),			Vector3(0.0f, -Z,   -X),
		Vector3(Z,    X,    0.0f),		Vector3(-Z,   X,    0.0f),
		Vector3(Z,    -X,   0.0f),		Vector3(-Z,   -X,   0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	meshData.Vertices.resize(12);
	meshData.Indices.resize(60);

	for(UINT i = 0; i < 12; ++i)
		meshData.Vertices[i].Position = pos[i];

	for(UINT i = 0; i < 60; ++i)
		meshData.Indices[i] = k[i];

	for(UINT i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	// Project vertices onto sphere and scale.
	for(UINT i = 0; i < meshData.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		Vector3 n = meshData.Vertices[i].Position;
		meshData.Vertices[i].Normal = n.Normalize();

		// Project onto sphere.
		meshData.Vertices[i].Position = radius*n;

		// Derive texture coordinates from spherical coordinates.
		float theta = RJE::Math::AngleFromXY(
			meshData.Vertices[i].Position.x, 
			meshData.Vertices[i].Position.z);

		float phi = acosf(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].TexC.x = theta/RJE_TWO_PI_F;
		meshData.Vertices[i].TexC.y = phi/RJE_PI_F;

		// Partial derivative of P with respect to theta
		meshData.Vertices[i].TangentU.x = -radius*sinf(phi)*sinf(theta);
		meshData.Vertices[i].TangentU.y = 0.0f;
		meshData.Vertices[i].TangentU.z = +radius*sinf(phi)*cosf(theta);

		meshData.Vertices[i].TangentU.Normalize();
	}
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount+1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for(UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		// vertices of ring
		float dTheta = 2.0f*RJE_PI_F/sliceCount;
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			MeshData::PosNormTanTex vertex;

			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			vertex.Position.Set(r*c, y, r*s);

			vertex.TexC.x = (float)j/sliceCount;
			vertex.TexC.y = 1.0f - (float)i/stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.TangentU.Set(-s, 0.0f, c);

			float dr = bottomRadius-topRadius;
			Vector3 bitangent(dr*c, -height, dr*s);
			Vector3 cross = Vector3::Cross(vertex.TangentU, bitangent);
			vertex.Normal = cross.Normalize();
			meshData.Vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = sliceCount+1;

	// Compute indices for each stack.
	for(UINT i = 0; i < stackCount; ++i)
	{
		for(UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(i*ringVertexCount + j);
			meshData.Indices.push_back((i+1)*ringVertexCount + j);
			meshData.Indices.push_back((i+1)*ringVertexCount + j+1);

			meshData.Indices.push_back(i*ringVertexCount + j);
			meshData.Indices.push_back((i+1)*ringVertexCount + j+1);
			meshData.Indices.push_back(i*ringVertexCount + j+1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, 
											UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData)
{
	UINT baseIndex = (UINT)meshData.Vertices.size();

	float y = 0.5f*height;
	float dTheta = 2.0f*RJE_PI_F/sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for(UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		meshData.Vertices.push_back( MeshData::PosNormTanTex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
	}

	// Cap center vertex.
	meshData.Vertices.push_back( MeshData::PosNormTanTex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );

	// Index of center vertex.
	UINT centerIndex = (UINT)meshData.Vertices.size()-1;

	for(UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i+1);
		meshData.Indices.push_back(baseIndex + i);
	}
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, 
											   UINT sliceCount, UINT stackCount, Data<PosNormTanTex>& meshData)
{
	// 
	// Build bottom cap.
	//

	UINT baseIndex = (UINT)meshData.Vertices.size();
	float y = -0.5f*height;

	// vertices of ring
	float dTheta = 2.0f*RJE_PI_F/sliceCount;
	for(UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius*cosf(i*dTheta);
		float z = bottomRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		meshData.Vertices.push_back( MeshData::PosNormTanTex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
	}

	// Cap center vertex.
	meshData.Vertices.push_back( MeshData::PosNormTanTex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );

	// Cache the index of center vertex.
	UINT centerIndex = (UINT)meshData.Vertices.size()-1;

	for(UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i+1);
	}
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, Data<PosNormTanTex>& meshData)
{
	UINT vertexCount = m*n;
	UINT faceCount   = (m-1)*(n-1)*2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n-1);
	float dz = depth / (m-1);

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	meshData.Vertices.resize(vertexCount);
	for(UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for(UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			meshData.Vertices[i*n+j].Position.Set(x, 0.0f, z);
			meshData.Vertices[i*n+j].Normal  .Set(0.0f, 1.0f, 0.0f);
			meshData.Vertices[i*n+j].TangentU.Set(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			meshData.Vertices[i*n+j].TexC.x = j*du;
			meshData.Vertices[i*n+j].TexC.y = i*dv;
		}
	}
 
	//
	// Create the indices.
	//

	meshData.Indices.resize(faceCount*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for(UINT i = 0; i < m-1; ++i)
	{
		for(UINT j = 0; j < n-1; ++j)
		{
			meshData.Indices[k]   = i*n+j;
			meshData.Indices[k+1] = i*n+j+1;
			meshData.Indices[k+2] = (i+1)*n+j;

			meshData.Indices[k+3] = (i+1)*n+j;
			meshData.Indices[k+4] = i*n+j+1;
			meshData.Indices[k+5] = (i+1)*n+j+1;

			k += 6; // next quad
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateFullscreenQuad(Data<PosNormTanTex>& meshData)
{
	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData.Vertices[0] = MeshData::PosNormTanTex(
		-1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	meshData.Vertices[1] = MeshData::PosNormTanTex(
		-1.0f, +1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	meshData.Vertices[2] = MeshData::PosNormTanTex(
		+1.0f, +1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	meshData.Vertices[3] = MeshData::PosNormTanTex(
		+1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateWireBox(float width, float height, float depth, Data<ColorVertex>& meshData, Color color)
{
	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	MeshData::ColorVertex v[8];
	v[0] = MeshData::ColorVertex(+w2, +h2, -d2, color);
	v[1] = MeshData::ColorVertex(-w2, +h2, -d2, color);
	v[2] = MeshData::ColorVertex(-w2, -h2, -d2, color);
	v[3] = MeshData::ColorVertex(+w2, -h2, -d2, color);
	v[4] = MeshData::ColorVertex(+w2, +h2, +d2, color);
	v[5] = MeshData::ColorVertex(-w2, +h2, +d2, color);
	v[6] = MeshData::ColorVertex(-w2, -h2, +d2, color);
	v[7] = MeshData::ColorVertex(+w2, -h2, +d2, color);
	meshData.Vertices.assign(&v[0], &v[8]);

	UINT i[24];
	i[0]  = 0; i[1]  = 1;
	i[2]  = 1; i[3]  = 2;
	i[4]  = 2; i[5]  = 3;
	i[6]  = 3; i[7]  = 0;
	i[8]  = 0; i[9]  = 4;
	i[10] = 3; i[11] = 7;
	i[12] = 2; i[13] = 6;
	i[14] = 1; i[15] = 5;
	i[16] = 5; i[17] = 4;
	i[18] = 4; i[19] = 7;
	i[20] = 7; i[21] = 6;
	i[22] = 6; i[23] = 5;
	meshData.Indices.assign(&i[0], &i[24]);
}
//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateWireSphere(float radius, Data<ColorVertex>& meshData, Color color)
{
	
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateAxisArrows(Data<ColorVertex>& meshData)
{ CreateAxisArrows(Vector3::right, Vector3::up, Vector3::forward, meshData); }
//-------------
void GeometryGenerator::CreateAxisArrows(Quaternion rotation, Data<ColorVertex>& meshData)
{
	Vector3 right   = rotation.GetRightVector();
	Vector3 up      = rotation.GetUpVector();
	Vector3 forward = rotation.GetForwardVector();
	CreateAxisArrows(right, up, forward, meshData);
}
//-------------
void GeometryGenerator::CreateAxisArrows(Vector3 right, Vector3 up, Vector3 forward, Data<ColorVertex>& meshData)
{
	MeshData::ColorVertex v[6];
	v[0] = MeshData::ColorVertex(0, 0, 0,                         Color::Red);
	v[1] = MeshData::ColorVertex(right.x, right.y, right.z,       Color::Red);
	v[2] = MeshData::ColorVertex(0, 0, 0,                         Color::Lime);
	v[3] = MeshData::ColorVertex(up.x, up.y, up.z,                Color::Lime);
	v[4] = MeshData::ColorVertex(0, 0, 0,                         Color::Blue);
	v[5] = MeshData::ColorVertex(forward.x, forward.y, forward.z, Color::Blue);
	meshData.Vertices.assign(&v[0], &v[6]);

	UINT i[6];
	i[0] = 0; i[1] = 1;
	i[2] = 2; i[3] = 3;
	i[4] = 4; i[5] = 5;
	meshData.Indices.assign(&i[0], &i[6]);
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateLine(Vector3 orientation, Data<ColorVertex>& meshData, Color color)
{
	MeshData::ColorVertex v[2];
	v[0] = MeshData::ColorVertex(0, 0, 0, color);
	v[1] = MeshData::ColorVertex(orientation.x, orientation.y, orientation.z, color);
	meshData.Vertices.assign(&v[0], &v[2]);

	UINT i[2];
	i[0] = 0; i[1] = 1;
	meshData.Indices.assign(&i[0], &i[2]);
}

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateLine(Data<ColorVertex>& meshData, Color color)
{ CreateLine(Vector3::forward, meshData, color); }

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateRay(Data<ColorVertex>& meshData, Color color)
{ CreateLine(10000*Vector3::forward, meshData, color); }
//---------
void GeometryGenerator::CreateRay(Vector3 orientation, Data<ColorVertex>& meshData, Color color)
{ CreateLine(10000*orientation, meshData, color); }

//////////////////////////////////////////////////////////////////////////
void GeometryGenerator::CreateWireFrustum(float fov, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color)
{
	
}
//---------
void GeometryGenerator::CreateWireFrustum(float width, float height, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color)
{ CreateWireFrustum(Vector3::right, Vector3::up, Vector3::forward, width, height, nearPlaneDepth, farPlaneDepth, meshData, color); }
//---------
void GeometryGenerator::CreateWireFrustum(Quaternion orientation, float width, float height, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color)
{
	Vector3 right   = orientation.GetRightVector();
	Vector3 up      = orientation.GetUpVector();
	Vector3 forward = orientation.GetForwardVector();
	CreateWireFrustum(right, up, forward, width, height, nearPlaneDepth, farPlaneDepth, meshData, color);
}
//---------
void GeometryGenerator::CreateWireFrustum(Vector3 right, Vector3 up, Vector3 forward, float width, float height, float nearPlaneDepth, float farPlaneDepth, Data<ColorVertex>& meshData, Color color)
{
	right.Normalize();
	up.Normalize();
	forward.Normalize();

	float wFar2 = 0.5f*width;
	float hFar2 = 0.5f*height;
	float wNear2 = 0.5f*((nearPlaneDepth*width)/farPlaneDepth);
	float hNear2 = 0.5f*((nearPlaneDepth*height)/farPlaneDepth);


	UINT i[24];
	i[0]  = 0; i[1]  = 1;
	i[2]  = 1; i[3]  = 2;
	i[4]  = 2; i[5]  = 3;
	i[6]  = 3; i[7]  = 0;
	i[8]  = 0; i[9]  = 4;
	i[10] = 3; i[11] = 7;
	i[12] = 2; i[13] = 6;
	i[14] = 1; i[15] = 5;
	i[16] = 5; i[17] = 4;
	i[18] = 4; i[19] = 7;
	i[20] = 7; i[21] = 6;
	i[22] = 6; i[23] = 5;
	meshData.Indices.assign(&i[0], &i[24]);

	MeshData::ColorVertex v[8];
	Vector3 fc = forward * farPlaneDepth;
	Vector3 nc = forward * nearPlaneDepth;
	v[0] = MeshData::ColorVertex(nc + (up * hNear2) + (right * wNear2), color);
	v[1] = MeshData::ColorVertex(nc + (up * hNear2) - (right * wNear2), color);
	v[2] = MeshData::ColorVertex(nc - (up * hNear2) - (right * wNear2), color);
	v[3] = MeshData::ColorVertex(nc - (up * hNear2) + (right * wNear2), color);
	v[4] = MeshData::ColorVertex(fc + (up * hFar2) + (right * wFar2), color);
	v[5] = MeshData::ColorVertex(fc + (up * hFar2) - (right * wFar2), color);
	v[6] = MeshData::ColorVertex(fc - (up * hFar2) - (right * wFar2), color);
	v[7] = MeshData::ColorVertex(fc - (up * hFar2) + (right * wFar2), color);
	meshData.Vertices.assign(&v[0], &v[8]);
}
