#include "DX11Device.h"

DX11Device::DX11Device()
{
	md3dDevice				= nullptr;
	md3dImmediateContext	= nullptr;
}

void DX11Device::Release()
{
	if (md3dImmediateContext)
		md3dImmediateContext->ClearState();

	RJE_SAFE_RELEASE(md3dImmediateContext);
	RJE_SAFE_RELEASE(md3dDevice);
}