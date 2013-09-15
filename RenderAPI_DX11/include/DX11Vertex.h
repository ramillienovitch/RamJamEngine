#pragma once

#include "DX11Helper.h"

namespace Vertex
{
	struct PosNormalTex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct SpriteVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		DirectX::PackedVector::XMCOLOR Color;
	};
}