#include "ContentD3D.h"

bool ContentD3D::loadContent(IContent** content, const string& source)
{
	return true;
}

bool ContentD3D::createInputObject()
{
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	//m_Input->Initialize();

	return true;
}

bool ContentD3D::createGraphicsObject(HWND hwnd)
{
	bool result;

	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	result = m_Graphics->Initialize(w, h, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ContentD3D::Render(int a)
{
	m_Graphics->Render(a);
}