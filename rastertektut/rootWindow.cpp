#include <list>

#include "windowManager.h"
#include "nodeWindow.h"
#include "rootWindow.h"

#include <iostream>
using namespace std;

LRESULT CALLBACK 
RootWindow::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
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

		case WM_LBUTTONDBLCLK:
		//case WM_LBUTTONDOWN:
		{
			NodeWindow* nodeWindow = new NodeWindow();

			nodeWindow->createWindow(hwnd);

			nodeWindow->createInputObject();
			nodeWindow->createGraphicsObject();

			BringWindowToTop(nodeWindow->getNodeWindowHandle());
			ShowWindow(nodeWindow->getNodeWindowHandle(), SW_SHOW);

			WindowManager* wM = WindowManager::getWindowManager();
			wM->addNodeWindow(nodeWindow);

			return 0;
		}

		//case WM_TIMER:
		case WM_PAINT:
		{
			HDC hdc;
			HGDIOBJ original = NULL;
			RECT startRECT;
			RECT endRECT;

			hdc = GetDC(hwnd);

			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 0, 0));

			WindowManager* wM = WindowManager::getWindowManager();

			for (int i = 0; i < wM->getNodeWindowsSize(); ++i)
			{
				HWND startH = nullptr;
				HWND toH = nullptr;
				NodeWindow *nW = wM->findNodeWindow(i);
				
				startH = nW->getNodeWindowHandle();
				toH = nW->getConnectedTo();

				if (toH != nullptr)
				{
					GetWindowRect(startH, &startRECT);
					//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
					MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&startRECT, 2);
					GetWindowRect(toH, &endRECT);
					MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&endRECT, 2);

					MoveToEx(hdc, startRECT.right, startRECT.bottom, NULL);
					LineTo(hdc, endRECT.left, endRECT.top);
				}
			}

			ReleaseDC(hwnd, hdc);

			return 0;
		}

	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

RootWindow::RootWindow()
{
	Initialize();
}

RootWindow::~RootWindow()
{
	DeInitialize();
}

int RootWindow::Initialize()
{
	m_NodeWindows = {};

	m_ClassName = "rootClass";
	m_Title = "myTD";
	//CS_DBLCLKS for double clicks to work: https://msdn.microsoft.com/en-us/library/windows/desktop/ms645606(v=vs.85).aspx
	m_Style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	m_hinst = GetModuleHandle(NULL);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = m_Style;
	wcex.lpfnWndProc = RootWindow::WndProc;
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

void RootWindow::DeInitialize()
{
	// Show the mouse cursor.
	ShowCursor(true);

	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	if (m_Graphics)
	{
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	if (m_NodeWindows.size() > 0)
	{
		for(auto nW:m_NodeWindows)
		{
			DestroyWindow(nW->getNodeWindowHandle());
			delete nW;
			nW = nullptr;
		}
		//HPQ: Destroy the registered class for node windows? 
		m_NodeWindows.clear();
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ClassName, m_hinst);
	m_hinst = NULL;

	return;
}

bool RootWindow::createInputObject()
{
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	RECT rc;
	GetWindowRect(m_hwnd, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	// Initialize the input object.
	m_Input->Initialize();

	return true;
}

bool RootWindow::createGraphicsObject()
{
	bool result;
	RECT rc;
	GetWindowRect(m_hwnd, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(w, h, m_hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}

bool RootWindow::Frame()
{
	bool result;
	int mouseX, mouseY;
	WindowManager* wM = WindowManager::getWindowManager();

	for (int i = 0; i < wM->getNodeWindowsSize(); i++)
	{
		NodeWindow* nW = wM->findNodeWindow(i);
		if (nW != nullptr)
		{
			nW->Frame();
		}
		else
		{
			cout << "nullptr..." << endl;
		}

	}
	return true;
}

void RootWindow::displayWindow()
{
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
}

void RootWindow::run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}