


struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse : COLOR;
    float4 WorldPosition : POSITION;
};

struct PS_OUTPUT
{
    float4 Normal : SV_TARGET0;
    float4 Diffuse : SV_TARGET1;
    float4 Position : SV_TARGET2;
    float4 Depth : SV_TARGET3;
};

Texture2D<float4> tex0 : register(t0);
SamplerState spr0 : register(s0);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    output.Normal = input.Normal;

    output.Diffuse = tex0.Sample(spr0, input.TexCoord);
    
    output.Position = float4(input.WorldPosition.xyz, 1.0f);
    
    output.Depth = float4(input.WorldPosition.www, 1.0);
    
    return output;
}
