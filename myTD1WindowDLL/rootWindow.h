#pragma once

#include <list>

#include <Windows.h>
#include "nodeWindow.h"
//#include "inputclass.h"
//#include "graphicsclass.h"

class __declspec(dllexport) RootWindow
{
public:
	RootWindow(); 
	~RootWindow();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	//HWND getRootWindowHandle() const
	//{
	//	return m_hwnd;
	//}
	//void setRootWindowHandle(const HWND&& h)
	//{
	//	m_hwnd = h;
	//}

	void displayWindow();
	void run();

	bool createInputObject();
	bool createGraphicsObject();

protected:
	int Initialize();
	void DeInitialize();

	bool Frame();

private:
	LPCTSTR m_ClassName;
	LPCTSTR m_Title;
	UINT    m_Style;
	HINSTANCE m_hinst;
	HWND	m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

	std::list<NodeWindow *> m_NodeWindows;
};

#define USER_1 10000