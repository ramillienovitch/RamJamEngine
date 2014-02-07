//======================================
// Skybox Shader (Forward And Deferred)
//======================================

cbuffer cbPerFrame
{
	float4x4 gWorldViewProj;
};

// Nonnumeric values cannot be added to a cbuffer.
TextureCube gCubeMap;

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VertexOut SkyboxVS(VertexIn vin)
{
	VertexOut vout;

	// Set z = w so that z/w = 1 (i.e., skybox always on far plane).
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;

	return vout;
}

float4 SkyboxPS(VertexOut pin) : SV_Target
{
	uint2 size;
	gCubeMap.GetDimensions(size.x, size.y);
	return (size.x == 0U ? float4(0.0f, 0.0f, 0.0f, 1.0f) : gCubeMap.Sample(samTriLinearSam, pin.PosL));
}

RasterizerState NoCull
{
	CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
	DepthFunc = LESS_EQUAL;
};

technique11 SkyboxTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SkyboxVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, SkyboxPS() ) );

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}
