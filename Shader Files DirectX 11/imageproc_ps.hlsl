Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer TimeBuffer : register(cb0)
{
    float redValue;
    float greenValue;
    float blueValue;
    float intensity;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;

};

float4 main(InputType input) : SV_TARGET
{
    float weight0, weight1, weight2;
    float4 colour;

    // Create the weights that each neighbor pixel will contribute to the blur.
    weight0 = 0.4062f;
    weight1 = 0.2442f;
    weight2 = 0.0545f;

    // Initialize the colour to black.
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add the nine horizontal pixels to the colour by the specific weight of each.
    colour += shaderTexture.Sample(SampleType, input.texCoord1) * weight2;
    colour += shaderTexture.Sample(SampleType, input.texCoord2) * weight1;
    colour += shaderTexture.Sample(SampleType, input.texCoord3) * weight0;
    colour += shaderTexture.Sample(SampleType, input.texCoord4) * weight1;
    colour += shaderTexture.Sample(SampleType, input.texCoord5) * weight2;


    //  //colour.r = colour.r * sin(input.texCoord1) * 2;
    colour.r = colour.r * sin(input.tex * redValue) * intensity;
    colour.g = colour.g * sin(input.tex * greenValue) * intensity;
    colour.b = colour.b * sin(input.tex * blueValue) * intensity;


   return colour;
}
