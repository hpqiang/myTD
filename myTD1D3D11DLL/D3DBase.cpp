#include "D3DBase.h"

D3DBase::D3DBase()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
}

D3DBase::~D3DBase()
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

bool D3DBase::initializeD3DEnv(HWND hwnd)
{
	bool result;

	m_D3D = new D3DClass;
	if (!m_D3D) {
		return false;
	}

	result = m_D3D->createD3DEnv(isVSYNC_ENABLED, hwnd, isFULL_SCREEN, fSCREEN_DEPTH, fSCREEN_NEAR);
	if (!result) {
		return false;
	}

	return result;
}

bool D3DBase::initializeCamera()
{
	return true;
}

bool D3DBase::initializeModel()
{
	return true;
}
bool D3DBase::initializeTextureShader(HWND hwnd)
{
	return true;
}

void D3DBase::D3DRender(int rot)
{
	return;
}