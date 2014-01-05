#pragma once

#if (RJE_GRAPHIC_API == DIRECTX_11)
	#include "DX11Drawable.h"
#endif

//////////////////////////////////////////////////////////////////////////
struct GameObject
{
	std::string		mName;

	Transform	mTransform;
#if (RJE_GRAPHIC_API == DIRECTX_11)
	DX11Drawable	mDrawable;
#endif

	GameObject();

	// TODO: Add GameObject Everything !!!
};
