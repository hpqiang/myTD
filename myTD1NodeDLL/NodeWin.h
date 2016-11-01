#pragma once

#include "CommonDefs.h"
#include "Node.h"
//#include "NodeOPD3D.h"
//#include "TDManager.h"
//#include "TDSingleton.h"
#include "graphicsclass.h"

typedef struct NodeMenu
{
	uint id;
	const char* menuTitle;
}PNodeMenu;

typedef struct NodeMenuItem
{
	uint menuId;
	uint id;
	const char * menuItemTitle;
	string command;
}PNodeMenuItem;

//Note: ids have to be unique
NodeMenu NodePopUpMenu[] =
{
	{ 301, "&D3D" },
	{ 302, "&OGL" },
};

NodeMenuItem NodePopUpMenuItem[] =
{
	{ 301, 401, "&Load D3D Texture","D3D Load Texture" },
	{ 301, 402, "&Load D3D Shader","D3D Load Shader" },
	{ 301, 403, "&D3D Geometry","D3D Geometry" },

	{ 302, 501, "&Load OGL Texture","OGL Load Texture" },
	{ 302, 502, "&Load OGL Shader","OGL Load Shader" },
	{ 302, 503, "&OGL Geometry","OGL Geometry" },
};

//To do: This struct should be in NodeWinD3D
struct myD3DConnectionOP
{
	PGeometryOP myGeometryOP;
	PLightOP	myLightOP;
	PMaterialOP myMaterialOP;
	PTextureOP	myTextureOP;
};

class __declspec(dllexport) NodeWin :public Node
{
public:

public:
	//To do: How to deal with WndProc?
	NodeWin();
	virtual ~NodeWin();

	HWND getNodeWinHandle() const
	{
		return m_hwnd;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int createWindow(HWND, const string& title);
	bool displayWindow();

	bool createInputObject();

	virtual void Render() = 0;

	//To do: This pure virtual should be in NodeWinD3D
	virtual void getD3DConnectionOP(myD3DConnectionOP *) = 0;

protected:
	int Initialize();
	void DeInitialize();

protected:
	static string 	m_ClassName;
	string			m_Title;
	uint			m_Style;
	HWND			m_hwnd;

	static bool		m_isInitialized;

	bool			m_isContainer;
	bool			m_isSelected;
	bool			m_isCurrent;

	InputClass*		m_Input;
};

class __declspec(dllexport) NodeWinD3D :public NodeWin
{
public:

	NodeWinD3D() 
	{
		m_D3DConnectionOP = new myD3DConnectionOP();
		//To do: for other component of m_D3DConnectionOP
		m_D3DConnectionOP->myGeometryOP = new GeometryOP();
		memset(m_D3DConnectionOP->myGeometryOP, 0.0f, sizeof(GeometryOP));
	}
	~NodeWinD3D() 
	{
		if (m_D3DConnectionOP->myGeometryOP != nullptr)
		{
			delete m_D3DConnectionOP->myGeometryOP;
			m_D3DConnectionOP->myGeometryOP = nullptr;
		}

		if (m_D3DConnectionOP != nullptr)
		{
			delete m_D3DConnectionOP;
			m_D3DConnectionOP = nullptr; 
		}
	}

	bool createGraphicsObject()
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
		result = m_Graphics->Initialize(w, h, m_hwnd);
		if (!result)
		{
			return false;
		}
		return true;
	}

	virtual void Render() override
	{
		//int rot = 10;
		//m_Graphics->Render(rot);
		getD3DConnectionOP(m_D3DConnectionOP);
		float rotX = m_D3DConnectionOP->myGeometryOP->getRotation().m_Rx;
		float rotY = m_D3DConnectionOP->myGeometryOP->getRotation().m_Ry;
		float rotZ = m_D3DConnectionOP->myGeometryOP->getRotation().m_Rz;
		float tX = m_D3DConnectionOP->myGeometryOP->getTranslation().m_Tx;
		float tY = m_D3DConnectionOP->myGeometryOP->getTranslation().m_Ty;
		float tZ = m_D3DConnectionOP->myGeometryOP->getTranslation().m_Tz;
		float sX = m_D3DConnectionOP->myGeometryOP->getScalar().m_Sx;
		float sY = m_D3DConnectionOP->myGeometryOP->getScalar().m_Sy;
		float sZ = m_D3DConnectionOP->myGeometryOP->getScalar().m_Sz;

		m_Graphics->Render(rotX, rotY, rotZ,
			tX, tY, tZ,
			sX, sY, sZ);
	}

	//To do: This function should be in NodeWinD3DGeometry and other derived classes
	virtual void getD3DConnectionOP(myD3DConnectionOP *op) override;

private:
	GraphicsClass		*m_Graphics;  //Q: Why need to add myTD1NodeDLL as reference in myMain????
	myD3DConnectionOP	*m_D3DConnectionOP;
};


