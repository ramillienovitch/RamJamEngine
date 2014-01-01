#include "GameObject.h"

//////////////////////////////////////////////////////////////////////////
GameObject::GameObject()
{
	mDrawable.mTransform = &mTransform;
	mDrawable.mMaterial  = &mMaterial;
}
