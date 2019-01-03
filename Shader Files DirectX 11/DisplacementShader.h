// Light shader.h
// Basic single light shader setup
#ifndef _DISPLACEMENTSHADER_H_
#define _DISPLACEMENTSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/Timer.h"

using namespace std;
using namespace DirectX;


class DisplacementShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT3 direction;

		float padding;
	};

	struct TimeBufferType
	{
		float frequency;
		float height;
		float time;
		float altitude;
		//float padding;
	};

public:

	DisplacementShader(ID3D11Device* device, HWND hwnd);
	~DisplacementShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap, Light* light, float time, float height, float frequency, float altitude);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
};

#endif
