#include "ContentD3DDefaultScene.h"

ContentD3DDefaultScene::ContentD3DDefaultScene()
{

}

ContentD3DDefaultScene::~ContentD3DDefaultScene()
{

}

bool ContentD3DDefaultScene::initializeCamera()
{
	m_Camera = new CameraClass;
	if (!m_Camera) return false;

	//HPQ: Set the initial view Matrix
	m_Camera->SetPosition(0.0f, 0.0f, -2.5f);

	return true;
}

bool ContentD3DDefaultScene::initializeModel()
{
	bool result;

	m_Model = new ModelClass;
	if (!m_Model) return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), L"C:/Users/lcuser/Documents/Visual Studio 2015/Projects/myTD/myTD1D3D11DLL/Data/seafloor.dds");

	if (!result)
	{
		return false;
	}
	return true;
}

bool ContentD3DDefaultScene::initializeTextureShader(HWND hwnd)
{
	bool result;

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ContentD3DDefaultScene::D3DRender(int rot)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	static float i = 1.0f;
	float rotation = 0.0f;
	static float noOpRotation = 0.0f;
	float opRotation = 0.0f;
	static float prevOpRotation = 0.0f;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, i, 0.5f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	rotation += (float)rot / 10;

	D3DXMatrixRotationY(&worldMatrix, rotation);
	prevOpRotation += rotation;
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
		return; // false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return;
}