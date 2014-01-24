cbuffer cbPerFrame : register (b0)
{
	float4 gAmbientLightColor;
	//-----------
	float4x4 gViewProj;
	//-----------
	float3 gEyePosW;
	//-----------
	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
	//-----------
	bool   gUseFog;
	bool   gUseTexture;
	bool   gUseAlphaClip;
};