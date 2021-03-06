#include "graphicsclass.h"

#include <iostream>
#include <string>
using namespace std;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
}

//GraphicsClass::GraphicsClass(const GraphicsClass& other)
//{
//}

GraphicsClass::~GraphicsClass()
{
}

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
		//MessageBox(hwnd, /*L*/"Could not initialize the texture shader object.", /*L*/"Error", MB_OK);
		return false;
	}

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
	result = Render(0);
	if (!result)
	{
		return false;
	}

	return true;
}

//To do: below geometry is for world matrix, not yet for object local space
bool GraphicsClass::Render(myD3DConnectionOP *op)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	static float blue = 1.0f;
	float rotX=0.0f, rotY = 0.0f, rotZ = 0.0f;
	float tX = 0.0f, tY = 0.0f, tZ = 0.0f;
	float sX = 0.0f, sY = 0.0f, sZ = 0.0f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, blue, 0.5f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	rotX = op->myGeometryOP->getRotation().m_Rx;
	rotY = op->myGeometryOP->getRotation().m_Ry;
	rotZ = op->myGeometryOP->getRotation().m_Rz;

	rotX /= 100.0f;
	rotY /= 100.0f;
	rotZ /= 100.0f;

	tX = op->myGeometryOP->getTranslation().m_Tx;
	tY = op->myGeometryOP->getTranslation().m_Ty;
	tZ = op->myGeometryOP->getTranslation().m_Tz;
	tX /= 100.0f;
	tY /= 100.0f;
	tZ /= 100.0f;

	sX = op->myGeometryOP->getScalar().m_Sx;
	sY = op->myGeometryOP->getScalar().m_Sy;
	sZ = op->myGeometryOP->getScalar().m_Sz;
	if (sX >= 1.0f)
	{
		sX = 0.0f;
	}
	if (sY >= 1.0f)
	{
		sY = 0.0f;
	}
	if (sZ >= 1.0f)
	{
		sZ = 0.0f;
	}
	sX = 1.0f - sX;
	sY = 1.0f - sY;
	sZ = 1.0f - sZ;
	//sX /= 100.0f;
	//sY /= 100.0f;
	//sZ /= 100.0f;

	//cout << "tX: " << tX << "\t" << "tY: " << tY << "tZ: " << tZ << endl;
	//cout << "sX: " << sX << "\t" << "sY: " << sY << "sZ: " << sZ << endl;


	D3DXMATRIX mR, mT, mS;
	//D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotX, rotY, rotZ);
	D3DXMatrixRotationYawPitchRoll(&mR, rotX, rotY, rotZ);
	//D3DXMatrixIdentity(&mT);
	D3DXMatrixTranslation(&mT, tX, tY, tZ);
	//D3DXMatrixIdentity(&mS);
	D3DXMatrixScaling(&mS, sX, sY, sZ);
	//worldMatrix = mR*mT*mS;
	D3DXMATRIX mRT;
	//D3DXMatrixIdentity(&mRT);

	D3DXMatrixMultiply(&mRT, &mR, &mT);
	D3DXMatrixMultiply(&worldMatrix, &mRT, &mS);

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

bool GraphicsClass::Render(float rotX, float rotY, float rotZ,
	float tX, float tY, float tZ,
	float sX, float sY, float sZ)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	static float blue = 1.0f;
	//float rotation = 0.0f;
	static float prevRotX = 0.0f;
	static float prevRotY = 0.0f;
	static float prevRotZ = 0.0f;

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
	//if (prevRotX != rotX)
		//D3DXMatrixRotationX(&worldMatrix, rotX / 100.0f);
	//if (prevRotY != rotY)
		//D3DXMatrixRotationY(&worldMatrix, rotY / 100.0f);
	//if (prevRotZ != rotZ)
	//cout << "rotZ : " << rotZ << endl;
//		D3DXMatrixRotationZ(&worldMatrix, rotZ / 100.0f);
	
	rotX /= 100.0f;
	rotY /= 100.0f;
	rotZ /= 100.0f;

	D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotX, rotY, rotZ);

	//prevRotX = rotX;
	//prevRotY = rotY;
	//prevRotZ = rotZ;

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