//==============================
// Simple shader for shadow map
//==============================

cbuffer cbObject
{
	float4x4 gWVP;
	float4x4 gDiffuseMapTransform;
};

Texture2D gDiffTex;


//-------------------
struct ShadowVertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float3 TanL    : TANGENT;
	float2 Tex     : TEXCOORD;
};

//-------------------
struct ShadowVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float3 TanW    : TANGENT;
	float2 Tex     : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////
ShadowVertexOut ShadowVS(ShadowVertexIn vin)
{
	ShadowVertexOut vout;

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWVP);
	vout.Tex  = mul(float4(vin.Tex, 0.0f, 1.0f), gDiffuseMapTransform).xy;
	vout.NormalW = float3(0,0,0);
	vout.PosW    = float3(0,0,0);
	vout.TanW    = float3(0,0,0);

	return vout;
}

RasterizerState DepthShadow
{
	// [From MSDN]
	// If the depth buffer currently bound to the output-merger stage has a UNORM format or
	// no depth buffer is bound the bias value is calculated like this: 
	//
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// where r is the minimum representable value > 0 in the depth-buffer format converted to float32.
	// [/End MSDN]
	// 
	// For a 24-bit depth buffer, r = 1 / 2^24.
	//
	// Example: DepthBias = 100000 ==> Actual DepthBias = 100000/2^24 = .006

	// You need to experiment with these values for your scene.
	DepthBias = 100000;
	DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 1.0f;
};

technique11 ShadowMap
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, ShadowVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( NULL );

		SetRasterizerState(DepthShadow);
	}
}