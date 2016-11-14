#include <iostream>
#include <exception>
using namespace std;

#include "ContentD3D.h"

ContentD3D::ContentD3D()
{
	m_Input = nullptr;
	//m_Graphics = nullptr;
	m_D3DBase = nullptr;
}

ContentD3D::ContentD3D(void *t, HWND hwnd)
{
	m_D3DBase = (D3DBase *)t;
	try
	{
		if (createD3DBaseObject(hwnd) == false)
		{
			cout << "Error in creating D3DBase Object!!!" << endl;
		}
	}
	catch(exception& e)
	{
		cout << "Error: " << e.what() << endl;
	}
}

ContentD3D::~ContentD3D()
{
	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}
	//if (m_Graphics)
	//{
	//	delete m_Graphics;
	//	m_Graphics = nullptr;
	//}
	if (m_D3DBase)
	{
		delete m_D3DBase;
		m_D3DBase = nullptr;
	}
}

bool ContentD3D::contentOP1(HWND hwnd)
{
	Update(hwnd);

	return true;
}
void ContentD3D::contentOP2(HWND hwnd)
{
	Render(0, hwnd);

	return;
}

bool ContentD3D::createInputObject(HWND hwnd)
{
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	//// Initialize the input object.
	RECT rect;
	//GetWindowRect(hwnd, &rect);
	GetClientRect(hwnd, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	//int w = GetSystemMetrics(SM_CXSCREEN);
	//int h = GetSystemMetrics(SM_CYSCREEN);
	//HWND parentHwnd = GetParent(hwnd);
	//GetWindowRect(parentHwnd, &rect);
	//int w = rect.right - rect.left;
	//int h = rect.bottom - rect.top;
	m_Input->Initialize(GetModuleHandle(NULL), hwnd, w, h);

	return true;
}

//bool ContentD3D::createGraphicsObject(HWND hwnd)
//{
//	bool result;
//
//	m_Graphics = new GraphicsClass();
//	if (!m_Graphics)
//	{
//		return false;
//	}
//
//	// Initialize the graphics object.
//	int w = GetSystemMetrics(SM_CXSCREEN);
//	int h = GetSystemMetrics(SM_CYSCREEN);
//	result = m_Graphics->Initialize(w, h, hwnd);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}

bool ContentD3D::createD3DBaseObject(HWND hwnd)
{
	m_D3DBase->initializeD3DEnv(hwnd);

	m_D3DBase->initializeCamera();
	m_D3DBase->initializeModel();
	m_D3DBase->initializeTextureShader(hwnd);

	return true;
}

void ContentD3D::Update(HWND hwnd)
{
	//cout << "Updating contentD3D" << endl;
	/*if (m_Input == nullptr)
	{
		createInputObject(hwnd);
	}
	m_Input->Frame();*/
}

void ContentD3D::Render(int a, HWND hwnd)
{
	m_D3DBase->D3DRender(a);
}