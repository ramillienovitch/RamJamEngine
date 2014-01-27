//=================================================
// Compute Shader to render tiled deferred shading
//=================================================

#include "PerFrameConstants.hlsl"

technique11 TiledDeferred
{
	pass P0
	{
		SetVertexShader( NULL );
		SetPixelShader( NULL );
		//SetComputeShader( CompileShader( cs_5_0, TiledDeferredCS() ) );
	}
}