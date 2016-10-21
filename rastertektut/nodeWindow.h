#pragma once

#include <Windows.h>
#include "inputclass.h"
#include "graphicsclass.h"

class Ops
{
public:
	Ops() {}
	~Ops() {}

	float getRotation()
	{
		return m_Rotation;
	}
	void setRotation(float r)
	{
		m_Rotation = r;
	}

private:
	float m_Rotation;
};

class NodeWindow
{
public:
	NodeWindow();
	~NodeWindow();

	HWND getNodeWindowHandle()
	{
		return m_hwnd;
	}
	void setNodeWindowHandle(HWND h)
	{
		m_hwnd = h;
	}

	Ops *getOps()
	{
		return m_Ops;
	}
	void setOps(Ops *op)
	{
		m_Ops = op;
	}

	HWND getConnectedFrom()
	{
		return m_ConnectedFrom;
	}
	void setConnectedFrom(HWND h)
	{
		m_ConnectedFrom = h;
	}

	HWND getConnectedTo()
	{
		return m_ConnectedTo;
	}
	void setConnectedTo(HWND h)
	{
		if (h == nullptr)
			return;
		m_ConnectedTo = h;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int createWindow(HWND);
	void displayWindow();

	bool createInputObject();
	bool createGraphicsObject();

	bool Frame();

protected:
	int Initialize();
	void DeInitialize();

private:
	static LPCTSTR m_ClassName;
	static LPCTSTR m_Title;
	UINT    m_Style;
	static HINSTANCE m_hinst;
	static HWND	m_ParentHwnd;
	HWND	m_hwnd;

	static bool m_isInitialized;

	bool m_isContainer;
	bool m_isSelected; 
	bool m_isCurrent;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

	HWND m_ConnectedFrom;
	HWND m_ConnectedTo;

	Ops *m_Ops;
};
