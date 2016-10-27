#include "tabWindow.h"

#include <iostream>
using namespace std;

LRESULT CALLBACK
TabWindow::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

TabWindow::TabWindow()
{
	Initialize();
}

TabWindow::~TabWindow()
{
	DeInitialize();
}

int TabWindow::Initialize()
{
	WNDCLASSEX wcex;

	m_ClassName = "tabClass";
	m_Title = "myTAB";
	//CS_DBLCLKS for double clicks to work: https://msdn.microsoft.com/en-us/library/windows/desktop/ms645606(v=vs.85).aspx
	m_Style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	m_hinst = GetModuleHandle(NULL);

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = m_Style;
	wcex.lpfnWndProc = TabWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinst;
	wcex.hIcon = LoadIcon(m_hinst, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_ClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_WINLOGO);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create the main window. 
	m_hwnd = CreateWindowEx(
		0,                      // no extended styles           
		m_ClassName,            // class name                   
		m_Title,                // window name                  
		WS_OVERLAPPEDWINDOW |   // overlapped window            
		WS_HSCROLL |			// horizontal scroll bar        
		WS_VSCROLL,				// vertical scroll bar          
		CW_USEDEFAULT,          // default horizontal position  
		CW_USEDEFAULT,          // default vertical position    
		CW_USEDEFAULT,          // default width                
		CW_USEDEFAULT,          // default height               
		(HWND)NULL,				// no parent or owner window    
		(HMENU)NULL,			// class menu used              
		m_hinst,				// instance handle              
		NULL);                  // no window creation data      

	if (!m_hwnd)
		return FALSE;
	return TRUE;
}

void TabWindow::DeInitialize()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ClassName, m_hinst);
	m_hinst = NULL;

	return;
}


void TabWindow::displayWindow()
{
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
}

void TabWindow::run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}