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

class __declspec(dllexport) NodeWindow
{
public:
	NodeWindow();
	~NodeWindow();

	HWND getNodeWindowHandle() const
	{
		return m_hwnd;
	}
	//void setNodeWindowHandle(HWND h)
	//{
	//	m_hwnd = h;
	//}

	////Ops *getOps() const
	//bool getOps(Ops **op)
	//{
	//	*op = m_Ops;
	//	//return m_Ops;
	//}
	void setOps(Ops *const op)
	{
		m_Ops = op;
	}

	//HWND getConnectedFrom() const
	//{
	//	return m_ConnectedFrom;
	//}
	void setConnectedFrom(const HWND& h)
	{
		m_ConnectedFrom = h;
	}

	HWND getConnectedTo() const
	{
		return m_ConnectedTo;
	}
	//HPQ: Below function?
	//bool getConnectedTo(HWND *h) 
	//{
	//	*h = m_ConnectedTo;
	//	return true;
	//}
	void setConnectedTo(const HWND& h)
	{
		//if (h == nullptr)  //HPQ: ????
		//{
		//	m_ConnectedTo = nullptr;
		//	return;
		//}
		m_ConnectedTo = h;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int createWindow(const HWND&);
	bool displayWindow();

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
