// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer TimeBuffer : register(cb1)
{
	float time;
	float frequency;
	float height;
	float speed;
};

struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;
};

[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
	float3 vertexPosition;
	OutputType output;


	//float3 v1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.x);
	//float3 v2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.x);
	//float3 vertexPosition = lerp(v1, v2, uvwCoord.y);
	//input.position.y += height * (sin(10 * input.position.x + time) + sin(10 * input.position.z + time));
	// Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
	float2 vertexUV = uvwCoord.x * patch[0].tex + uvwCoord.y * patch[1].tex + uvwCoord.z * patch[2].tex;
	vertexUV.x = vertexUV.x + time * speed;
	//vertexPosition.y += height * sin(frequency * vertexPosition.x + time);

	float4 heightMap = shaderTexture.SampleLevel(SampleType, vertexUV, 0);

	vertexPosition.y = heightMap.x * height;
	// vertexPosition.y += height * sin(frequency * vertexPosition.x + time);
	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);


	output.tex = vertexUV;
	output.normal = patch[0].normal;
	// Send the input color into the pixel shader.
	output.colour = patch[0].colour;

	return output;
}

