//////////////////////////////////////////////////////////////////////////
// Helper File with all data types available for meshes
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "MathHelper.h"
#include "Color.h"

using namespace RJE_COLOR;

namespace MeshData
{
	//=========================================
	enum RJE_InputLayout
	{
		RJE_IL_PosNormalTex  = 0,
		RJE_IL_PosNormTanTex = 1,
		RJE_IL_PosColor      = 2
	};
	//=========================================


	//=========================================
	enum RJE_PrimitiveTopology
	{
		RJE_PT_UNDEFINED					= 0,
		RJE_PT_POINTLIST					= 1,
		RJE_PT_LINELIST						= 2,
		RJE_PT_LINESTRIP					= 3,
		RJE_PT_TRIANGLELIST					= 4,
		RJE_PT_TRIANGLESTRIP				= 5,
		RJE_PT_LINELIST_ADJ					= 10,
		RJE_PT_LINESTRIP_ADJ				= 11,
		RJE_PT_TRIANGLELIST_ADJ				= 12,
		RJE_PT_TRIANGLESTRIP_ADJ			= 13,
		RJE_PT_1_CONTROL_POINT_PATCHLIST
	};
	//=========================================


	//=========================================
	struct PosNormTanTex
	{
		PosNormTanTex(){}
		PosNormTanTex(const Vector3& p, const Vector3& n, const Vector3& t, const Vector2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv){}
		PosNormTanTex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v)
			: Position(px,py,pz), Normal(nx,ny,nz), TangentU(tx, ty, tz), TexC(u,v){}

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
		PosNormalTex(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
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
		SpriteVertex(const Vector3& p, const Vector2& t, const Color& c)
			: pos(p), tex(t), color(c){}
		SpriteVertex(float px, float py, float pz, float u, float v, BYTE a, BYTE r, BYTE g, BYTE b)
			: pos(px,py,pz), tex(u,v), color(a,r,g,b){}

		Vector3 pos;
		Vector2 tex;
		Color	color;
	};
	//=========================================


	//=========================================
	struct ColorVertex
	{
		ColorVertex(){}
		ColorVertex(const Vector3& p, const Color& c)								: pos(p), color(c){}
		ColorVertex(float px, float py, float pz, const Color& c)					: pos(px,py,pz), color(c){}
		ColorVertex(float px, float py, float pz, BYTE a, BYTE r, BYTE g, BYTE b)	: pos(px,py,pz), color(a,r,g,b){}

		Vector3 pos;
		Color	color;
	};
	//=========================================


	//////////////////////////////////////////////////////////////////////////
	template <typename VertexType>
	struct Data
	{
		Data() {}
		Data(u32 vertexCount) : Vertices(vertexCount){}
		Data(u32 vertexCount, u32 indicesCount) : Vertices(vertexCount), Indices(indicesCount){}

		std::vector<VertexType>		Vertices;
		std::vector<u32>			Indices;
	};
	//////////////////////////////////////////////////////////////////////////
}