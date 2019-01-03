// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TessBuffer : register(cb0)
{
	float tessFactor;
	float3 padding;
}

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;
};

struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct OutputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> patch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;
	//float tessFactor;
	// Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = tessFactor;
	output.edges[1] = tessFactor;
	output.edges[2] = tessFactor;
	//output.edges[3] = 4;
	//output.edges[3] = 3;

	// Set the tessellation factor for tessallating inside the triangle.
	output.inside = tessFactor; //u-axis (columns)
								//output.inside[1] = 4;  //v-axis (rows)

	return output;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	OutputType output;

	// Set the position for this control point as the output position.
	output.position = patch[pointId].position;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;

	// Set the input color as the output color.
	output.colour = patch[pointId].colour;

	return output;
}