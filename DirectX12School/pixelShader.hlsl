


struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse  : COLOR;
};

Texture2D<float4> tex0 : register(t0);
SamplerState spr0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 lightDirection = float3(1.0, -1.0, 1.0);
    lightDirection = normalize(lightDirection);
    
    float3 normal = normalize(input.Normal.xyz);
    float3 light = saturate(-dot(normal, lightDirection));
    float4 color = tex0.Sample(spr0, input.TexCoord);
    color.rgb *=  light;
    
    return color;
}
