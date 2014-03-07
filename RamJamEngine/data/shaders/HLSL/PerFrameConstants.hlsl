#ifndef PER_FRAME_CONSTANTS_HLSL
#define PER_FRAME_CONSTANTS_HLSL

cbuffer cbPerFrame : register (b0)
{
	float4 gAmbientLightColor;
	//-----------
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
	float4x4 gCameraViewToLightProj;
	//-----------
	float3 gEyePosW;
	float2 gNearFar;
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
	bool gVisualizePerSampleShading;
	bool gVisualizeLightCount;
	//-----------
	uint gFramebufferSizeX;
	uint gFramebufferSizeY;
	//-----------
	bool   gUseFaceNormals;
	bool   gUseFog;
	bool   gUseTexture;
	bool   gUseAlphaClip;
};

#endif