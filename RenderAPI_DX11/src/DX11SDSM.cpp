#include "DX11SDSM.h"
#include "..\..\RamJamEngine_Tools\include\Profiler.h"

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
void DX11SDSM::UpdateShaderConstants()
{
	DX11Effects::SDSMFX->SetLightSpaceBorder(mCurrentConstants.mLightSpaceBorder);
	DX11Effects::SDSMFX->SetMaxScale(        mCurrentConstants.mMaxScale);
	DX11Effects::SDSMFX->SetDilationFactor(  mCurrentConstants.mDilationFactor);
	DX11Effects::SDSMFX->SetReduceTileDim(   mCurrentConstants.mReduceTileDim);
}

//////////////////////////////////////////////////////////////////////////
ID3D11ShaderResourceView* DX11SDSM::ComputePartitionsFromGBuffer(	ID3D11DeviceContext* dc,
																	u32 gbufferTexturesNum, ID3D11ShaderResourceView** gbufferTextures,
																	const Vector3& lightSpaceBorder, const Vector3& maxScale, u32 screenWidth, u32 screenHeight )
{
	//----------------------------------
	mCurrentConstants.mLightSpaceBorder = Vector4(lightSpaceBorder, 0.0f);
	mCurrentConstants.mMaxScale         = Vector4(maxScale, 0.0f);
	UpdateShaderConstants();
	//----------------------------------

	ID3D11UnorderedAccessView* partitionUAV = mPartitionBuffer->GetUnorderedAccess();
	ID3D11ShaderResourceView*  partitionSRV = mPartitionBuffer->GetShaderResource();
	ID3D11UnorderedAccessView* partitionBoundsUAV = mPartitionBounds->GetUnorderedAccess();
	ID3D11ShaderResourceView*  partitionBoundsSRV = mPartitionBounds->GetShaderResource();

	// Clear out Z-bounds result for reduction
	DX11Effects::SDSMFX->ClearZBoundsTech->GetPassByIndex(0)->Apply(0, dc);
	dc->CSSetUnorderedAccessViews(6, 1, &partitionUAV, 0);
	dc->Dispatch(1, 1, 1);

	// Reduce Z-bounds
	DX11Effects::SDSMFX->ReduceZBoundsTech->GetPassByIndex(0)->Apply(0, dc);
	int dispatchWidth  = (screenWidth + mCurrentConstants.mReduceTileDim - 1) / mCurrentConstants.mReduceTileDim;
	int dispatchHeight = (screenHeight + mCurrentConstants.mReduceTileDim - 1) / mCurrentConstants.mReduceTileDim;
	dc->CSSetUnorderedAccessViews(6, 1, &partitionUAV, 0);
	dc->CSSetShaderResources(0, gbufferTexturesNum, gbufferTextures);
	dc->Dispatch(dispatchWidth, dispatchHeight, 1);

	// Generate partition intervals from Z-bounds
	DX11Effects::SDSMFX->ComputePartitionsTech->GetPassByIndex(0)->Apply(0, dc);
	dc->CSSetUnorderedAccessViews(6, 1, &partitionUAV, 0);
	dc->Dispatch(1, 1, 1);

	// =================================================
	// ReducePartitionBounds

	// Clear partition bounds buffer
	DX11Effects::SDSMFX->ClearPartitionBoundsTech->GetPassByIndex(0)->Apply(0, dc);
	dc->CSSetUnorderedAccessViews(7, 1, &partitionBoundsUAV, 0);
	dc->Dispatch(1, 1, 1);

	UnbindResources(dc);

	// Reduce bounds
	DX11Effects::SDSMFX->SetPartitionsSRV(partitionSRV);
	DX11Effects::SDSMFX->ReduceBoundsTech->GetPassByIndex(0)->Apply(0, dc);
	dc->CSSetShaderResources(0, gbufferTexturesNum, gbufferTextures);
	dc->CSSetUnorderedAccessViews(7, 1, &partitionBoundsUAV, 0);
	dc->Dispatch(dispatchWidth, dispatchHeight, 1);

	UnbindResources(dc);

	// Update partitions with new bounds
	DX11Effects::SDSMFX->SetPartitionBoundsSRV(partitionBoundsSRV);
	DX11Effects::SDSMFX->ComputeCustomPartitionsTech->GetPassByIndex(0)->Apply(0, dc);
	dc->CSSetUnorderedAccessViews(6, 1, &partitionUAV, 0);
	dc->Dispatch(1, 1, 1);

	UnbindResources(dc);

	return mPartitionBuffer->GetShaderResource();
}

//////////////////////////////////////////////////////////////////////////
void DX11SDSM::UnbindResources(ID3D11DeviceContext* dc)
{
	ID3D11ShaderResourceView*  dummySRV[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	ID3D11UnorderedAccessView* dummyUAV[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	dc->CSSetShaderResources(0, 8, dummySRV);
	dc->CSSetUnorderedAccessViews(0, 8, dummyUAV, 0);
}