#include "DX11DepthBuffer.h"

DX11DepthBuffer::DX11DepthBuffer()
{
	mDepthStencilBuffer	= nullptr;
	mDepthStencilView	= nullptr;
}

//////////////////////////////////////////////////////////////////////////
void DX11DepthBuffer::Release()
{
	RJE_SAFE_RELEASE(mDepthStencilView);
	RJE_SAFE_RELEASE(mDepthStencilBuffer);
}