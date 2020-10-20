


struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse : COLOR;
};

Texture2D<float4> NorTex : register(t0);
Texture2D<float4> DifTex : register(t1);
SamplerState spr0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 lightDirection = float3(1.0, -1.0, 1.0);
    lightDirection = normalize(lightDirection);

    float4 nor = NorTex.Sample(spr0, input.TexCoord);
    float4 dif = DifTex.Sample(spr0, input.TexCoord);
    
    float light = saturate(0.5 - dot(nor.rgb, lightDirection) * 0.5);
    
    float4 color = float4(dif * light);
    return color;
}
