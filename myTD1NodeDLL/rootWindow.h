#pragma once

#include <Windows.h>

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

//	InputClass* m_Input;
//	GraphicsClass* m_Graphics;
};