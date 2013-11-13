#pragma once

#include "DX11Helper.h"

namespace Vertex
{
	struct PosNormalTex
	{
		Vector3 Pos;
		Vector3 Normal;
		Vector2 Tex;
	};

	struct SpriteVertex
	{
		Vector3 Pos;
		Vector2 Tex;
		DirectX::PackedVector::XMCOLOR Color;
	};
}