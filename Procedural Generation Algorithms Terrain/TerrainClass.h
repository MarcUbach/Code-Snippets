////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>
#include <directxmath.h>
#include "PerlinNoise.h"
#include <time.h>
#include "../DXFramework/DXF.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*);
	bool InitializeTerrain(ID3D11Device* device, int terrainWidth, int terrainHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool GenerateHeightMap(ID3D11Device* device, bool perlin, bool fault, int iterations, float displacement);
	int  GetIndexCount();
	void sendData(ID3D11DeviceContext* deviceContext);
	bool GeneratePerlin(ID3D11Device* device, float frequency, float amplitude, int smoothValue);
	bool GenerateFaulting(ID3D11Device* device, int iterationsFaulting, float displacement, int smoothValue);
	bool GenerateCirclesAlgorithm(ID3D11Device* device, int iterationsCircle, int displacement, int smoothValue);
	void FaultAlgorithm(int iterations, float displacement);
	void CirclesAlgorithm(int iterations, int displacement);
	void Smoothing(int passes);

	int iterationsDone = 0;
	int minIterations = 200;
	float initialDisp;
	float maxDisp = 0.5f;
	float minDisp = 0.3f;
	

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	bool m_terrainGeneratedToggle;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
};

#endif