#pragma once

#include "Node.h"
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

class __declspec(dllexport) NodeWin :public Node
{
public:
	static uint m_Rotation;  //Temp testing
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
	//bool createGraphicsObject();

	virtual void Render(int rot) = 0; // override
	//{
	//	cout << "Node**********rot : " << rot << endl;
	//}
	//bool Frame();

protected:
	int Initialize();
	void DeInitialize();


protected:
	static string m_ClassName;
	/*static*/ string m_Title;
	uint    m_Style;
	HWND	m_hwnd;

	static bool m_isInitialized;

	bool m_isContainer;
	bool m_isSelected;
	bool m_isCurrent;

	InputClass* m_Input;
};
uint NodeWin::m_Rotation = 0;

class __declspec(dllexport) NodeWinD3D :public NodeWin
{
public:
	static uint m_Rotation;  //Temp testing

	NodeWinD3D() {}
	~NodeWinD3D() {}

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
	}

	virtual void Render(int rot) override
	{
		cout << "D3D**********rot : " << rot << endl;

		m_Graphics->Render(rot);
	}

private:
	GraphicsClass *m_Graphics;  //Q: Why need to add myTD1NodeDLL as reference in myMain????
};

uint NodeWinD3D::m_Rotation = 0;

