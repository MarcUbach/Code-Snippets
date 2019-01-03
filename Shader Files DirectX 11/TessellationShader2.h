// Light shader.h
// Basic single light shader setup
#ifndef _TESSELLATIONSHADER2_H_
#define _TESSELLATIONSHADER2_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"

using namespace std;
using namespace DirectX;


class TessellationShader2 : public BaseShader
{
private:
	struct TessBufferType
	{
		float tessFactor;
		XMFLOAT3 padding;
	};

	struct TimeBufferType
	{
		float time;
		float frequency;
		float height;
		float speed;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT3 direction;
		float padding3;
	};

public:

	TessellationShader2(ID3D11Device* device, HWND hwnd);
	~TessellationShader2();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float tessFactor, float time, float frequency, float height, float speed, Light* light);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* tessBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11SamplerState* sampleState;
};

#endif