#pragma once
#include <Windows.h>

class TabWindow
{
public:
	TabWindow();
	~TabWindow();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void displayWindow();
	void run();

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