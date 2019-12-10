
cbuffer ConstantBuffer : register( b0 )
{
	matrix worldViewProj;
}

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float3 normal : TEXCOORD0;
};

VertexShaderOutput vertexShaderMain( float4 position : position,
									 float3 normal : normal,
									 float4 texCoord : texCoord,
									 float4 color : color)
{
    VertexShaderOutput output;
    output.position = mul( position, worldViewProj );
	output.normal = normal;
    return output;
}

float4 pixelShaderMain( VertexShaderOutput input ) : SV_Target
{
	float3 n = normalize(input.normal) * 0.5 + float3(0.5, 0.5, 0.5);
    return float4( n.x, n.y, n.z, 1.0f );
}
