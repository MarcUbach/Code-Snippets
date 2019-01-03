// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer TimeBuffer : register(cb1)

{
	float frequency;
	float height;
	float time;
	float altitude;
	float padding;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	//float3 viewDirection : TEXCOORD1;
};

OutputType main(InputType input)
{
	OutputType output;

	float4 worldPosition;
	float4 heightMapValue;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	////offset position based on sine wave
	//input.position.y += height * sin(frequency * input.position.x + time);
	////modify normals
	//input.normal.x = 1.0 - cos(input.position.x + time);
	//input.normal.y = abs(cos(input.position.x + time));    

	heightMapValue = texture0.SampleLevel(Sampler0, input.tex, 0);

	input.position.y += input.position.y + (altitude * heightMapValue.x);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);

	return output;
}