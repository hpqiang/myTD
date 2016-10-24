#pragma once

#include <list>

#include <Windows.h>
#include "nodeWindow.h"

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

#define IDM_EDIT_CUT 4
#define IDM_EDIT_COPY 5
#define IDM_EDIT_PASTE 6

class __declspec(dllexport) RootWindow
{
public:
	RootWindow(); 
	~RootWindow();

	static void insertOneNodeWindow(NodeWindow& nW);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void displayWindow();
	void run();

	bool createInputObject();
	bool createGraphicsObject();

protected:
	int Initialize();
	void DeInitialize();

	static NodeWindow& addOneNodeWindow(HWND hwnd);
	static void moveChildWindows(HWND hwnd, RECT prevRECT);
	static void drawConnection(HWND hwnd, bool);

	bool Frame();

private:
	LPCTSTR m_ClassName;
	LPCTSTR m_Title;
	UINT    m_Style;
	HINSTANCE m_hinst;
	HWND	m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

#define USER_1 10000