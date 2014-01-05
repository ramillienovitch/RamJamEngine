#pragma once

#include "DX11Helper.h"

class InputLayoutDesc
{
public:
	//static const D3D11_INPUT_ELEMENT_DESC PosNormalTex[3];
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTanTex[4];
	static const D3D11_INPUT_ELEMENT_DESC PosColor[2];
};

class DX11InputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	//static ID3D11InputLayout* PosNormalTex;
	static ID3D11InputLayout* PosNormalTanTex;
	static ID3D11InputLayout* PosColor;
};