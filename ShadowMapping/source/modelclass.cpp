////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


#include <iostream>
using namespace std;

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;

	//HPQ
	m_models = {};
	//eHPQ
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffersByVector(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		//m_models.at(i)->
		//vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		//vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		//vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].position = D3DXVECTOR3(m_models.at(i).x, m_models.at(i).y, m_models.at(i).z);
		vertices[i].texture = D3DXVECTOR2(m_models.at(i).tu, m_models.at(i).tv);
		vertices[i].normal = D3DXVECTOR3(m_models.at(i).nx, m_models.at(i).ny, m_models.at(i).nz);

		indices[i] = i;

		cout << "vertices [" << i << "] " << ": " << endl; 
		cout << " x = " << vertices[i].position.x << "\t";
		cout << "y = " << vertices[i].position.y << "\t";
		cout << "z = " << vertices[i].position.z << endl;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	//HPQ
	if (m_models.size() != 0) {
		m_models.clear();
	}
	//eHPQ

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

//It's at http://stackoverflow.com/questions/16493702/copying-gdi-bitmap-into-directx-texture
//#include <gdiplus.h>
//#pragma comment(lib, "Gdiplus.lib")
//using namespace Gdiplus;
//Bitmap bmp;


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

#ifdef _DEBUG
	std::cout << "Loading " << filename << "....\n";
#endif

	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}
	//HPQ
	if (m_models.size() != 0) {
		m_models.clear();
	}
	//eHPQ

	return;
}


void ModelClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void ModelClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}
#include <iostream>
#include <assert.h>

using namespace std;

bool CubeModelClass::AddPlaneMeshByRotation(float rotation) //int hor, int vert) 
{
	//float rotation = 45.0f; // / D3DX_PI;
	D3DXMATRIX worldMatrix;
	ModelType* m = new ModelType;
	D3DXMatrixIdentity(&worldMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);

	int size = m_models.size();
	//int size = 24;

	for (int i = 0; i < size; i++) {
		//m[0].x = 
		//	worldMatrix._11 * m_models.at(i).x
		//	+ worldMatrix._12 * m_models.at(i).y
		//	+ worldMatrix._13 * m_models.at(i).z; 
		//m[0].y = 
		//	worldMatrix._21 * m_models.at(i).x
		//	+ worldMatrix._22 * m_models.at(i).y
		//	+ worldMatrix._23 * m_models.at(i).z;
		//m[0].z =
		//	worldMatrix._31 * m_models.at(i).x
		//	+ worldMatrix._32 * m_models.at(i).y
		//	+ worldMatrix._33 * m_models.at(i).z;
		//m[0].tu = m_models.at(i).tu;
		//m[0].tv = m_models.at(i).tv;
		//m[0].nx = m_models.at(i).nx +1.0f;
		//m[0].ny = m_models.at(i).ny;
		//m[0].nz = m_models.at(i).nz +1.0f;

		m[0].x = m_models.at(i).x + 0.5f;
		m[0].y = m_models.at(i).y;
		m[0].z = m_models.at(i).z;

		D3DXVECTOR3 newCoords;
		D3DXVECTOR3 oldCoords = D3DXVECTOR3(m[0].x, m[0].y, m[0].z);

		D3DXVec3TransformCoord(&newCoords, &oldCoords, &worldMatrix);
		m[0].x = newCoords.x;
		m[0].y = newCoords.y;
		m[0].z = newCoords.z;

		m[0].tu = m_models.at(i).tu;
		m[0].tv = m_models.at(i).tv;
		m[0].nx = m_models.at(i).nx +1.0f;
		m[0].ny = m_models.at(i).ny;
		m[0].nz = m_models.at(i).nz +1.0f;

		m_models.push_back(m[0]);
	}

	cout << "m_models size: " << m_models.size() << endl;
	m_vertexCount = m_models.size();
	m_indexCount = m_vertexCount;

	delete m;

	return true;

}

bool CubeModelClass::CreatePlaneMeshVector(D3DXVECTOR3 start, D3DXVECTOR3 end, int hor, int vert) 
{
	assert(hor != 0);
	assert(vert != 0);
	assert(start != end);

	ModelType* m = new ModelType[6];

	float stepX = (end.x - start.x) / hor;
	float stepY = (end.y - start.y) / vert;

//	int index = 0;
	for (int row = 0; row < hor; row++)
	{
//		index += 6;
		for (int col = 0; col < vert; col++) {
			//index += col;
			m[0].x = row*stepX;
			m[0].y = col*stepY;
			m[0].z = 0.0f;
			m[0].tu = 1.0f;
			m[0].tv = 0.0f;
			m[0].nx = 0.0f;
			m[0].ny = 0.0f;
			m[0].nz = -1.0f;

			m_models.push_back(m[0]);

			m[1] = m[0];
			m[1].y = m[0].y + stepY;
			m[1].tu = 0.0f;
			m[1].tv = 0.0f;

			m_models.push_back(m[1]);

			m[2] = m[0];
			m[2].x = m[0].x + stepX;
			m[2].tu = 1.0f;
			m[2].tv = 1.0f;

			m_models.push_back(m[2]);

			m[3] = m[2];
			m_models.push_back(m[3]);

			m[4] = m[1];
			m_models.push_back(m[4]);

			m[5] = m[4];
			m[5].x = m[4].x + stepX;
			m[5].tu = 1.0f;
			m[5].tv = 0.0f;

			m_models.push_back(m[5]);
//			index += 6;
		}
	}

	m_vertexCount = m_models.size();
	m_indexCount = m_vertexCount;

	cout << "m_vertexCount: " << m_vertexCount << endl;

	vector<ModelType>::iterator it;
	for (it = m_models.begin(); it != m_models.end(); it++)
	{
		cout << "m_models x: " << (*it).x << "\t";
		cout << "m_models y: " << (*it).y << "\t";
		cout << "m_models z: " << (*it).z << endl;
	}

	delete[] m;

	return true;
}

bool CubeModelClass::CreatePlaneMesh(D3DXVECTOR3 start, D3DXVECTOR3 end, int hor, int vert) 
{
	assert(hor != 0);
	assert(vert != 0);
	assert(start != end);
	m_vertexCount = (hor + 1)*(vert + 1) * 6; //Note: Different from former stuff, e.g, not 6 for a rectangle, but 4 now
	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	float stepX = (end.x - start.x) / hor;
	float stepY = (end.y - start.y) / vert;

	//cout << "stepX: " << stepX << "\t";
	//cout << "stepY: " << stepY << "\n";

	int index = 0;
	for (int row = 0; row < hor; row++)
	{
		//index += row*hor;
		index += 6;
		for (int col = 0; col < vert; col++) {
			//index += col;
			m_model[index].x = row*stepX;
			m_model[index].y = col*stepY;
			m_model[index].z = 0.0f;
			m_model[index].tu = 1.0f;
			m_model[index].tv = 0.0f;
			m_model[index].nx = 0.0f;
			m_model[index].ny = 0.0f;
			m_model[index].nz = -1.0f;

			m_model[index + 1] = m_model[index];
			m_model[index + 1].y = m_model[index].y + stepY;
			m_model[index + 1].tu = 0.0f;
			m_model[index + 1].tv = 0.0f;

			m_model[index + 2] = m_model[index];
			m_model[index + 2].x = m_model[index].x + stepX;
			m_model[index + 2].tu = 1.0f;
			m_model[index + 2].tv = 1.0f;

			m_model[index + 3] = m_model[index + 2];
			//m_model[index + 3].nz = 1.0f;

			m_model[index + 4] = m_model[index + 1];

			m_model[index + 5] = m_model[index + 4];
			m_model[index + 5].x = m_model[index + 4].x + stepX;
			//m_model[index + 5].y = m_model[index + 4].y + stepY * 2;
			m_model[index + 5].tu = 1.0f;
			m_model[index + 5].tv = 0.0f;
			index += 6;
			cout << "index: " << index << endl;
		}
	}


	return true;
}

bool CubeModelClass::CreateCubicMesh(D3DXVECTOR3 start, D3DXVECTOR3 end, int seg) {
	//CreatePlaneMesh(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 1.0f, -1.0f), seg, seg);  // front

	CreatePlaneMeshVector(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 1.0f, -1.0f), seg, seg);  // front
	AddPlaneMeshByRotation(30.0f); // seg, seg);
	AddPlaneMeshByRotation(15.0f); // seg, seg);

	//CreatePlaneMesh(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), seg, seg);  // right
	//CreatePlaneMesh(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, -1.0f, 1.0f), seg, seg);  // back
	//CreatePlaneMesh(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, -1.0f, -1.0f), seg, seg);  // left
	//CreatePlaneMesh(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 1.0f, -1.0f), seg, seg);  // up
	//CreatePlaneMesh(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, -1.0f, -1.0f), seg, seg);  // bottom

	return true;
}

bool CubeModelClass::Initialize(ID3D11Device* device)//, ID3D11DeviceContext* deviceContext) // , char* modelFilename, WCHAR* textureFilename) 
{
	cout << "In this oen? " << endl;
	bool result;

	// Load in the model data,
//	result = LoadModel(modelFilename);
	result = CreateCubicMesh(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2);
	if (!result){
		return false;
	}

	// Initialize the vertex and index buffers.
	//	result = InitializeBuffers(device);
		result = InitializeBuffersByVector(device);
	if (!result){
		return false;
	}

	// Load the texture for this model.
//	result = LoadTexture(device, textureFilename);
	result = LoadTexture(device);// , deviceContext);
	if (!result){
		return false;
	}

	return true;
}


bool CubeModelClass::LoadTexture(ID3D11Device* device)//, ID3D11DeviceContext* deviceContext) 
{
	HRESULT result;
	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object
	result = m_Texture->Initialize(device); // , deviceContext);
	if (!result)
	{
		return false;
	}

}