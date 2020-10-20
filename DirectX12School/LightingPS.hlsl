


struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse : COLOR;
};

Texture2D<float4> NorTex : register(t0);
Texture2D<float4> DifTex : register(t1);
Texture2D<float4> PosTex : register(t2);
Texture2D<float4> DepthTex : register(t3);
SamplerState spr0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 lightDirection = float3(1.0, -1.0, 1.0);
    float3 lightPos = float3(0.0, 10.0, 0.0);
    lightDirection = normalize(lightDirection);
    float3 cameraPos = float3(0.0, 0.0, -5.0);
    
   
    float4 n = NorTex.Sample(spr0, input.TexCoord);
    float4 diffuseColor = DifTex.Sample(spr0, input.TexCoord);
    float4 worldPos = PosTex.Sample(spr0, input.TexCoord);
    float depth = DepthTex.Sample(spr0, input.TexCoord).r;
    
    float light = saturate(0.5 - dot(n.rgb, lightDirection) * 0.5);
    diffuseColor.rgb *= light;
    
    float3 eye = worldPos.xyz - cameraPos;
    float3 ref = reflect(eye, n.xyz);
    ref = normalize(ref);
    float spec = -dot(ref, lightDirection);
    spec = saturate(spec);
    spec = pow(spec, 20.0);
    
    diffuseColor.rgb += spec;
    
    return diffuseColor;
   // float3 viewDir = normalize(cameraPos - worldPos.xyz);

 //   float attConst = 2.0;
 //   float attLin = 0.7;
 //   float attQuad = 1.8;
 //   float ambient = diffuseColor * 1.0; // hard-coded ambient component
 
 //   	// fragment to light vector data
 //   const float3 vToL = lightPos - worldPos.xyz;
 //   const float distToL = length(vToL);
 //   const float3 dirToL = vToL / distToL;
	//// attenuation
 //   const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	//// diffuse intensity
 //   const float3 diffuse = diffuseColor  * att * max(0.0f, dot(dirToL, n));
	//// reflected light vector
 //   const float3 w = n * dot(vToL, n);
 //   const float3 r = w * 2.0f - vToL;
	//// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
 //   const float3 specular = att * (diffuseColor)  * pow(max(0.0f, dot(normalize(-r), normalize(worldPos.xyz))), 1.0);
	//// final color
 //   return float4(saturate(diffuse + ambient + specular), 1.0f) ;
}
