#pragma once
#include <d3d11.h>
#include <d3dx10math.h>

#include "textureclass.h"

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device*, LPCWSTR);
	
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool getVertices(VertexType **v, int *nV);
	bool getIndices(unsigned long **indices, int *nI);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, LPCWSTR);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
};