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
	{ 301, 401, "&Load Texture","Load Texture" },
	{ 301, 402, "&Load Shader","Load Shader" },
	{ 301, 403, "&Geometry","Geometry" },

	{ 302, 501, "&Load Texture","Load Texture" },
	{ 302, 502, "&Load Shader","Load Shader" },
	{ 302, 503, "&Geometry","Geometry" },
};

class __declspec(dllexport) NodeWin :public Node
{
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
	//bool createGraphicsObject();

	virtual void Render() override
	{}
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

class __declspec(dllexport) NodeWinD3D :public NodeWin
{
public:
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

	void Render() override
	{
		//m_Graphics->Render(nullptr);
	}

private:
	GraphicsClass *m_Graphics;  //Q: Why need to add myTD1NodeDLL as reference in myMain????
};

