cbuffer cbPerFrame : register (b0)
{
	float4 gAmbientLightColor;
	//-----------
	float4x4 gViewProj;
	float4x4 gProj;
	//-----------
	float3 gEyePosW;
	//-----------
	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
	//-----------
	bool   gUseFaceNormals;
	bool   gUseLightingOnly;
	bool   gUseFog;
	bool   gUseTexture;
	bool   gUseAlphaClip;
	//-----------
	uint4 mFramebufferDimensions;
};