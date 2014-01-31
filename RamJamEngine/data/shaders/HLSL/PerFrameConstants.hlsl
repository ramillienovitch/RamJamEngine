#ifndef PER_FRAME_CONSTANTS_HLSL
#define PER_FRAME_CONSTANTS_HLSL

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
	bool gVisualizePosition;
	bool gVisualizeAlbedo;
	bool gVisualizeNormals;
	bool gVisualizeDepth;
	bool gVisualizeSpecular;
	//-----------
	bool   gUseFaceNormals;
	bool   gUseFog;
	bool   gUseTexture;
	bool   gUseAlphaClip;
};

#endif