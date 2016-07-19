////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include <vector>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
//private:
protected:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	/*virtual*/ bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float, float, float);
	void GetPosition(float&, float&, float&);

//private:
protected:
	bool InitializeBuffers(ID3D11Device*);
	bool InitializeBuffersByVector(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

//private:
protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	vector<ModelType> m_models;
	float m_positionX, m_positionY, m_positionZ;
};

class CubeModelClass:public ModelClass {
public:
	bool Initialize(ID3D11Device*);// , ID3D11DeviceContext*); // , char*, WCHAR*);
	//bool Initialize1(ID3D11Device*); // , char*, WCHAR*);

protected:
	bool CreateCubicMesh(D3DXVECTOR3 start, D3DXVECTOR3 end, int seg);

	bool LoadTexture(ID3D11Device*); // , ID3D11DeviceContext*);
	//bool LoadTextureFromBitmap(ID3D11Device*);
private:
	bool CreatePlaneMesh(D3DXVECTOR3 start, D3DXVECTOR3 end, int hor, int vert);
	bool CreatePlaneMeshVector(D3DXVECTOR3 start, D3DXVECTOR3 end, int hor, int vert);
	bool AddPlaneMeshByRotation(float);// int, int);

//private:

};

class SphereModelClass:public ModelClass {

};

class PlaneModelClass :public ModelClass {

};

#endif