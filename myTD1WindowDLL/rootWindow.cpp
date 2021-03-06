#include <list>
#include <thread>

#include "windowManager.h"
#include "nodeWindow.h"
#include "rootWindow.h"

#include <iostream>
using namespace std;

void RootWindow::insertOneNodeWindow(NodeWindow& nW)
{
	WindowManager* wM = WindowManager::getWindowManager();
	wM->addNodeWindow(&nW);
}

//HPQ: refer to http://zetcode.com/gui/winapi/menus/
void AddMenu(HWND hwnd)
{
	HMENU hMenubar; // [2];
	HMENU hMenu; // [2];

	hMenubar/*[0]*/ = CreateMenu();
	//hMenubar[1] = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
	SetMenu(hwnd, hMenubar);

	//hMenu[1] = CreateMenu();

	//AppendMenuW(hMenu[1], MF_STRING, IDM_EDIT_CUT, L"&Cut");
	//AppendMenuW(hMenu[1], MF_STRING, IDM_EDIT_COPY, L"&Copy");
	////AppendMenuW(hMenu[1], MF_SEPARATOR, 0, NULL);
	//AppendMenuW(hMenu[1], MF_STRING, IDM_EDIT_PASTE, L"&Paste");

	//AppendMenuW(hMenubar[1], MF_POPUP, (UINT_PTR)hMenu[1], L"&Edit");
	//SetMenu(hwnd, hMenubar[1]);

}

LRESULT CALLBACK RootWindow::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	static RECT prevRECT;
	static bool bPrevLine = false;

	switch (umsg)
	{
		case WM_CREATE:
		{
			AddMenu(hwnd);
			return 0;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wparam)) {

			case IDM_FILE_NEW:
			case IDM_FILE_OPEN:

				MessageBeep(MB_ICONINFORMATION);
				break;

			case IDM_FILE_QUIT:

				SendMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			}

			break;
		}
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
		{
			NodeWindow& nW = addOneNodeWindow(hwnd);
			
			//HPQ: have to use std::ref????
			//std::thread t = std::thread(insertOneNodeWindow,nW);
			//HPQ: Below works fine
			//std::thread t = std::thread(insertOneNodeWindow,std::ref(nW));
			//HPQ: Why below works? Not clear invoking method conceptually
			std::thread t = std::thread([&](NodeWindow& nW) 
			{
				insertOneNodeWindow(std::ref(nW));
			}, std::ref(nW) );

			t.join();//HPQ: do we need to call join? RAII??

			return 0;
		}

		case WM_ENTERSIZEMOVE: 
		{
			GetWindowRect(hwnd, &prevRECT);
			MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&prevRECT, 2);
			return 0;
		}
		case WM_SIZE:   // main window changed size 
		{
			moveChildWindows(hwnd, prevRECT);
			SendMessage(hwnd, USER_1, 0, 0);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			bPrevLine = true;
			return 0;
		}

		case WM_LBUTTONUP:
		{
			bPrevLine = false;
			return 0;
		}

		case USER_1:
		{
			drawConnection(hwnd, bPrevLine);// , hdc);
			//RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);

			return 0;
		}

	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void RootWindow::moveChildWindows(HWND hwnd, RECT prevRECT)
{
	RECT rc;
	// Get the dimensions of the main window's client 
	// area, and enumerate the child windows. Pass the 
	// dimensions to the child windows during enumeration. 
	GetWindowRect(hwnd, &rc);
	MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&rc, 2);

	WindowManager* wM = WindowManager::getWindowManager();

	for (int i = 0; i < wM->getNodeWindowsSize(); ++i)
	{
		HWND h = nullptr;
		NodeWindow *nW = wM->findNodeWindow(i);
		RECT rcChild;
		float ratioW = ((float)(rc.right - rc.left) / (float)(prevRECT.right - prevRECT.left));
		float ratioH = ((float)(rc.bottom - rc.top) / (float)(prevRECT.bottom - prevRECT.top));

		//cout << "ratioW: " << ratioW << "\t";
		//cout << "ratioH: " << ratioH << endl;

		h = nW->getNodeWindowHandle();

		GetWindowRect(h, &rcChild);
		MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&rcChild, 2);

		MoveWindow(h, rcChild.left * ratioW, rcChild.top * ratioH, 
			(rcChild.right - rcChild.left) * ratioW, //Q: Unsafe, is there a winapi?
			(rcChild.bottom - rcChild.top) * ratioH,
			true
		);
	}

}

void RootWindow::drawConnection(HWND hwnd, bool bPrevLine)
{
	RECT startRECT;
	RECT endRECT;
	HDC hdc;
	HGDIOBJ original = NULL;
	static POINT prevFrom;
	static POINT prevTo;
	//static bool bPrevLine = false;

	hdc = GetDC(hwnd);

	//SelectObject(hdc, GetStockObject(DC_PEN));
	//SetDCPenColor(hdc, RGB(255, 0, 0));

	WindowManager* wM = WindowManager::getWindowManager();

	for (int i = 0; i < wM->getNodeWindowsSize(); ++i)
	{
		HWND startH = nullptr;
		HWND toH = nullptr;
		NodeWindow *nW = wM->findNodeWindow(i);
		bPrevLine = false;

		startH = nW->getNodeWindowHandle();
		toH = nW->getConnectedTo();
		//nW->getConnectedTo(&toH);

		if (toH != nullptr)
		{
			GetWindowRect(startH, &startRECT);
			//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
			MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&startRECT, 2);
			GetWindowRect(toH, &endRECT);
			MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&endRECT, 2);

			//HPQ: refer to http://winapi.freetechsecrets.com/win32/WIN32Drawing_Lines_with_the_Mouse.htm
			SetROP2(hdc, R2_NOTXORPEN);

			if (bPrevLine) {
				MoveToEx(hdc, prevFrom.x, prevFrom.y,
					(LPPOINT)NULL);
				LineTo(hdc, prevTo.x, prevTo.y);
			}

			//SelectObject(hdc, GetStockObject(DC_PEN));
			//SetDCPenColor(hdc, RGB(255, 0, 0));

			MoveToEx(hdc, startRECT.right, startRECT.bottom, NULL);
			LineTo(hdc, endRECT.left, endRECT.top);

			prevFrom.x = startRECT.right;
			prevFrom.y = startRECT.bottom;
			prevTo.x = endRECT.left;
			prevTo.y = endRECT.top;
			bPrevLine = true;
		}

	}
	ReleaseDC(hwnd, hdc);
}

RootWindow::RootWindow()
	: m_ClassName(""), m_Title("")
	, m_Style(0), m_hinst(nullptr), m_hwnd(nullptr)
	, m_Input(nullptr), m_Graphics(nullptr)
{
	Initialize();
}

RootWindow::~RootWindow()
{
	DeInitialize();
}

int RootWindow::Initialize()
{
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
	wcex.lpszMenuName = "simple menu"; //MAKEINTRESOURCE(IDM_MYMENURESOURCE); //NULL;
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

NodeWindow& RootWindow::addOneNodeWindow(HWND hwnd)
{
	NodeWindow* nodeWindow = new NodeWindow();

	nodeWindow->createWindow(hwnd);
	nodeWindow->createInputObject();
	nodeWindow->createGraphicsObject();

	BringWindowToTop(nodeWindow->getNodeWindowHandle());
	ShowWindow(nodeWindow->getNodeWindowHandle(), SW_SHOW);

	return *nodeWindow;
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