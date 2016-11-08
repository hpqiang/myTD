#pragma once

#if defined(_WIN32)
#include <windows.h>
#endif

#include "CommonDefs.h"

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
	{ 301, 403, "&D3D Scene","D3D Scene" },
	{ 301, 404, "&D3D Geometry","D3D Geometry" },

	{ 302, 501, "&Load OGL Texture","OGL Load Texture" },
	{ 302, 502, "&Load OGL Shader","OGL Load Shader" },
	{ 302, 503, "&OGL Geometry","OGL Geometry" },
};

class __declspec(dllexport) NodeWin
{
public:

public:
	NodeWin();
	~NodeWin();

	HWND getNodeWinHandle() const
	{
		return m_hwnd;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int createWindow(HWND, const string& title);
	bool displayWindow();

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
};
