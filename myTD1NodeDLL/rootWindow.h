#pragma once

#include <Windows.h>

typedef struct Menu
{
	uint id;
	const char* menuTitle;
}PMenu;

typedef struct MenuItem
{
	uint menuId;
	uint id;
	const char * menuItemTitle;
	string command;
}PMenuItem;

Menu mainMenu[] =
{
	{ 1, "&File" },
	{ 2, "&Edit" },
	{ 3, "&Window" },
	{ 4, "&Help" },
	{ 5, "&Quit" }
};

MenuItem mainMenuItem[] =
{
	{ 1, 1, "&New", "" },
	{ 1, 2, "&Load", "" },
	{ 1, 3, "&Save", "" },
	{ 1, 4, "Save &As...","" },
	{ 1, 0, NULL, "" },
	{ 1, 6, "&Quit","" },

	{ 2, 1, "&Cut","" },
	{ 2, 2, "&Copy","" },
	{ 2, 3, "&Paste","" },

	{ 3, 1, "&Python","Create Python Win" },
	{ 3, 2, "&Text","Create Text Win" },
	{ 3, 3, "&Table","" },

	{ 4, 1, "&About","" },
	{ 4, 2, "&Help","" },
};

//Note: ids have to be unique
Menu popUpMenu[] =
{
	{ 101, "&Node Win" },
	{ 102, "&Node OP" },

	{ 103, "&New Node" }
};

MenuItem popUpMenuItem[] =
{
	{ 101, 201, "&2D Win","" },
	{ 101, 202, "&D3D Win","Create Node Win D3D" },
	{ 101, 203, "&OGL Win","" },
	{ 101, 204, "&Audio Win","" },
	{ 101, 205, "&Video Win","" },
	{ 101, 206, "&Camera Win","" },

	{ 102, 301, "&2D OP","" },
	{ 102, 302, "&D3D OP","Create Node OP D3D" },
	{ 102, 303, "&OGL OP","" },
	{ 102, 304, "&Camera OP","" },

	{ 103, 301, "&New Node Win", "New Node Win" },
};

class __declspec(dllexport) RootWindow
{
public:
	RootWindow(); 
	~RootWindow();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void displayWindow();
	void run();

	HINSTANCE getInstance()
	{
		return m_hinst;
	}
	HWND getHwnd()
	{
		return m_hwnd;
	}

	bool createInputObject();
	bool createGraphicsObject();

protected:
	int Initialize();
	void DeInitialize();

private:
	LPCTSTR m_ClassName;
	LPCTSTR m_Title;
	UINT    m_Style;
	HINSTANCE m_hinst;
	HWND	m_hwnd;
};