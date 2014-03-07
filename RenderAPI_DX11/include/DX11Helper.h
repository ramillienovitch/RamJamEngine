#pragma once

#include "Globals.h"
#include "Types.h"
#include "Debug.h"
#include "Input.h"
#include "MathHelper.h"
#include "IniFile.h"
#include "../../RamJamEngine/include/MeshData.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11shader.h>
//#include <d3dx11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxerr.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include "..\..\Effects11\inc\d3dx11effect.h"
//#include "D3DX11Effect.h"		// from RenderAPI_DX11\include ==> in conflict with Effects11

#if defined(RJE_DEBUG)
#	include "Initguid.h"
#	include "DXGIDebug.h"
#endif



//////////////////////////////////////////////////////////////////////////
#define COMPILE_SHADER_RUNTIME 1
#define COMPILE_SHADER_OFFLINE 2

#ifndef COMPILE_SHADER
#	define COMPILE_SHADER COMPILE_SHADER_OFFLINE
//#	define COMPILE_SHADER COMPILE_SHADER_RUNTIME
#endif
//////////////////////////////////////////////////////////////////////////

#include "../../RamJamEngine/include/Light.h"
#include "../../RamJamEngine/include/Mesh.h"
#include "../../RamJamEngine/include/Transform.h"
#include "../../RamJamEngine/include/Material.h"

//////////////////////////////////////////////////////////////////////////

using namespace DirectX;

//////////////////////////////////////////////////////////////////////////
#include "DX11Profiler.h"
#include "DX11Device.h"
#include "DX11CommonStates.h"
#include "DX11Effect.h"
#include "DX11FontSheet.h"
#include "DX11SpriteBatch.h"
#include "DX11InputLayout.h"
#include "DX11StructuredBuffer.h"
#include "DX11TextureManager.h"
#include "DX11Texture2D.h"
#include "DX11Mesh.h"
#include "DX11SDSM.h"
#include "../../DirectXTex/DirectXTex.h"
//////////////////////////////////////////////////////////////////////////
