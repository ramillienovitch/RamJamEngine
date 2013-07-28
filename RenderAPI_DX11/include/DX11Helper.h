#pragma once

#include "Globals.h"
#include "Types.h"
#include "Debug.h"
#include "Input.h"
#include "MathHelper.h"
#include "IniFile.h"

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
#include <DirectXColors.h>
#include "..\..\Effects11\inc\d3dx11effect.h"
//#include "D3DX11Effect.h"		// from RenderAPI_DX11\include ==> in conflict with Effects11



//////////////////////////////////////////////////////////////////////////
#define COMPILE_SHADER_RUNTIME 1
#define COMPILE_SHADER_OFFLINE 2

#ifndef COMPILE_SHADER
#	define COMPILE_SHADER COMPILE_SHADER_OFFLINE
//#	define COMPILE_SHADER COMPILE_SHADER_RUNTIME
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#include "DX11Math.h"
#include "DX11Device.h"
#include "DX11DepthBuffer.h"
#include "DX11CommonStates.h"
#include "DX11Light.h"
#include "DX11Effect.h"
#include "DX11Vertex.h"
#include "DX11InputLayout.h"
#include "DDSTextureLoader.h"
#include "GeometryGenerator.h"
//////////////////////////////////////////////////////////////////////////