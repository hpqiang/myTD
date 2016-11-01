////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
//#include "nodeWindow.h"

#include "graphicsclass.h"

#include <iostream>
using namespace std;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

static void printMatrix(D3DXMATRIX m) {
	cout << endl;
	cout << m._11 << " " << m._12 << " " << m._13 << " " << m._14;
	cout << endl;
	cout << m._21 << " " << m._22 << " " << m._23 << " " << m._24;
	cout << endl;
	cout << m._31 << " " << m._32 << " " << m._33 << " " << m._34;
	cout << endl;
	cout << m._41 << " " << m._42 << " " << m._43 << " " << m._44;
	cout << endl;

}

#include <string>

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_D3D = new D3DClass;
	if (!m_D3D) {
		return false;
	}

	result = m_D3D->createD3DEnv(VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera) return false;

	//HPQ: Set the initial view Matrix
	m_Camera->SetPosition(0.0f, 0.0f, -2.5f);
	//m_Camera->Render();

	m_Model = new ModelClass;
	if (!m_Model) return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), L"C:/Users/lcuser/Documents/Visual Studio 2015/Projects/myTD/myTD1D3D11DLL/Data/seafloor.dds");

	//LPCWSTR str = (LPCWSTR)TARGET_FILE;
	////cout <<  << endl;
	//result = m_Model->Initialize(m_D3D->GetDevice(), (LPCWSTR)TARGET_FILE );
	if (!result)
	{
		return false;
	}

	//HPQ
	//m_Model->Render(m_D3D->GetDeviceContext());

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, /*L*/"Could not initialize the texture shader object.", /*L*/"Error", MB_OK);
		return false;
	}

	//D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	//// Get the view, projection, and world matrices from the camera and d3d objects.
	//m_Camera->GetViewMatrix(viewMatrix);
	//m_D3D->GetProjectionMatrix(projectionMatrix);
	//m_D3D->GetWorldMatrix(worldMatrix);
	//
	//cout << "Initialize: " << endl;
	//cout << "viewMatrix: " << endl;
	//printMatrix(viewMatrix);
	//cout << "projectionMatrix: " << endl;
	//printMatrix(projectionMatrix);
	//cout << "worldMatrix: " << endl;
	//printMatrix(worldMatrix);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	bool result;

	//// Set the location of the mouse.
	//result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	//if (!result)
	//{
	//	return false;
	//}

	//// Set the position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	//return true;

	// Render the graphics scene.
	//	result = Render(nullptr);
	result = Render(0);
	if (!result)
	{
		return false;
	}

	return true;
}

//To do: add more parameters
//Q: Cannot pass in myD3DConnectionOP since it's a different dll?
bool GraphicsClass::Render(float rotX, float rotY, float rotZ,
	float tX, float tY, float tZ,
	float sX, float sY, float sZ)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	static float blue = 1.0f;
	//float rotation = 0.0f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, blue, 0.5f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	//D3DXMatrixRotationY(&worldMatrix, rotation);
	//float rotX = op->myGeometryOP->getRotation().m_Rx;
	//To do: add more operations
	//cout << "rotX : " << rotX << "Y: " << rotY << "Z: " << rotZ << endl;
	D3DXMatrixRotationX(&worldMatrix, rotX);
	//D3DXMatrixRotationY(&worldMatrix, rotY);
	//D3DXMatrixRotationZ(&worldMatrix, rotZ);

	//if (rotation > (90.0f / (float)D3DX_PI))
	//{
	//	rotation = 0; // -= 180.0f / ((float)D3DX_PI);
	//}

	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::Render(int rot)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	static float i = 1.0f;
	/*static*/ float rotation = 0.0f;
	static float noOpRotation = 0.0f;
	/*static*/ float opRotation = 0.0f;
	static float prevOpRotation = 0.0f;
	//static float prevRotation = 0.0f;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, i, 0.5f);

	//if (i <= 0.0f)
	//{
	//	i = 1.0f;
	//}

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

//	if (op != nullptr)
	{
//		opRotation += op->getRotation();
		//prevRotation += opRotation;
	/*else
		rotation += 0.0005f;*/

	//rotation += opRotation;
		rotation += (float)rot / 10;
	//rotation += prevOpRotation;
	//cout << "op: " << op << '\t';
	//cout << "opRotation--->" << opRotation << endl;
//	cout << "rotation--->" << rotation << endl;

	D3DXMatrixRotationY(&worldMatrix, rotation);
	prevOpRotation += rotation;
	//D3DXMatrixRotationY(&worldMatrix, opRotation);
	}
	//else
	//{
	//	//rotation += 0.05f; 
	//	//noOpRotation += rotation;
	//	//	D3DXMatrixRotationY(&worldMatrix, noOpRotation);
	//}
	if (rotation > (90.0f / (float)D3DX_PI))
	{
		rotation = 0; // -= 180.0f / ((float)D3DX_PI);
	}

	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}