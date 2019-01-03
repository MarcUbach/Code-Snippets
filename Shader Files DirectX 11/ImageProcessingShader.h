// Horizontal blur shader handler
// Loads horizontal blur shaders (vs and ps)
// Passes screen width to shaders, for sample coordinate calculation

#ifndef _IMAGEPROCESSINGSHADER_H_
#define _IMAGEPROCESSINGSHADER_H_

#include "../DXFramework/BaseShader.h"


using namespace std;
using namespace DirectX;


class ImageProcessingShader : public BaseShader
{
private:
	struct ScreenSizeBufferType
	{
		float screenWidth;
		XMFLOAT3 padding;
	};

	struct TimeBufferType
	{
		float redValue;
		float greenValue;
		float blueValue;
		float intensity;
	};

public:

	ImageProcessingShader(ID3D11Device* device, HWND hwnd);
	~ImageProcessingShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float width, float redValue, float greenValue, float blueValue, float intensity);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;
	ID3D11Buffer* timeBuffer;
};

#endif