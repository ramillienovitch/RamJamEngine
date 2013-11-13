//////////////////////////////////////////////////////////////////////////
// Helper File with all data types available for meshes
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "MathHelper.h"

namespace MeshData
{
	//=========================================
	struct PosNormTanTex
	{
		PosNormTanTex(){}
		PosNormTanTex(const Vector3& p, const Vector3& n, const Vector3& t, const Vector2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv){}
		PosNormTanTex(
			float px, float py, float pz, 
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px,py,pz), Normal(nx,ny,nz),
			TangentU(tx, ty, tz), TexC(u,v){}

		Vector3 Position;
		Vector3 Normal;
		Vector3 TangentU;
		Vector2 TexC;
	};
	//=========================================
	

	//=========================================
	struct PosNormalTex
	{
		PosNormalTex(){}
		PosNormalTex(const Vector3& p, const Vector3& n, const Vector2& uv)
			: Pos(p), Normal(n), Tex(uv){}
		PosNormalTex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float u, float v)
			: Pos(px,py,pz), Normal(nx,ny,nz), Tex(u,v){}

		Vector3 Pos;
		Vector3 Normal;
		Vector2 Tex;
	};
	//=========================================


	//=========================================
	struct SpriteVertex
	{
		SpriteVertex(){}
		SpriteVertex(const Vector3& p, const Vector2& t, const Vector4& c)
			: Pos(p), Tex(t), Color(c){}
		SpriteVertex(
			float px, float py, float pz,
			float u, float v,
			float a, float r, float g, float b)
			: Pos(px,py,pz), Tex(u,v), Color(a,r,g,b){}

		Vector3 Pos;
		Vector2 Tex;
		Vector4 Color;
	};
	//=========================================


	//=========================================
	struct ColorVertex
	{
		ColorVertex(){}
		ColorVertex(const Vector3& p, const Vector4& c)
			: Pos(p), Color(c){}
		ColorVertex(
			float px, float py, float pz,
			float a, float r, float g, float b)
			: Pos(px,py,pz), Color(a,r,g,b){}

		Vector3 Pos;
		Vector4 Color;
	};
	//=========================================


	//////////////////////////////////////////////////////////////////////////
	template <typename VertexType>
	struct Data
	{
		std::vector<VertexType>		Vertices;
		std::vector<UINT>			Indices;
	};
	//////////////////////////////////////////////////////////////////////////
}