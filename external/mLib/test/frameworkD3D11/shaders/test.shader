
cbuffer ConstantBuffer : register( b0 )
{
	matrix worldViewProj;
}

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float4 color : TEXCOORD0;
};

VertexShaderOutput vertexShaderMain( float4 position : position,
									 float4 normal : normal,
									 float4 color : color,
									 float4 texCoord : texCoord )
{
    VertexShaderOutput output;
    output.position = mul( position, worldViewProj );
	output.color = color;
    return output;
}

float4 pixelShaderMain( VertexShaderOutput input ) : SV_Target
{
    //return float4( input.color.x, input.color.y, input.color.z, 1.0f );
	return float4( 1.0, 1.0, 1.0, 1.0);
}
