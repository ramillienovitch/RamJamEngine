#include "DX11SDSM.h"

//////////////////////////////////////////////////////////////////////////
DX11SDSM::DX11SDSM()
{
	// Set default constants
	mCurrentConstants.mScatterTileDim = 64;

	// TODO: This needs tweaking per resolution/GPU... we want to set it up so that there are approximately
	// as many tiles on the screen as there are cores on the GPU.
	mCurrentConstants.mReduceTileDim = 128;

	// 1% dilation on partition borders to cover standard filtering (mipmapping/aniso).
	// Blur kernels are handled separately so this should stay small or can even be removed in some scenes.
	mCurrentConstants.mDilationFactor = 0.01f;
}

//////////////////////////////////////////////////////////////////////////
void DX11SDSM::UpdateShaderConstants( /*SDSMEffect* fx*/ )
{
	//fx->SetConstants(mCurrentConstants);
}
