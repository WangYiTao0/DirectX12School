


struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse : COLOR;
};

struct PS_OUTPUT
{
    float4 Normal : SV_TARGET0;
    float4 Diffuse : SV_TARGET1;
};

Texture2D<float4> tex0 : register(t0);
SamplerState spr0 : register(s0);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    output.Normal = input.Normal;

    output.Diffuse = tex0.Sample(spr0, input.TexCoord);
    
    return output;
}
