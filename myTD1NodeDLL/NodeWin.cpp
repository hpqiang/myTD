#include <string>

#include "NodeWin.h"

bool NodeWin::m_isInitialized = false;
LPCTSTR NodeWin::m_ClassName = "nodeWinClass";
LPCTSTR NodeWin::m_Title = "nodeWin";

LRESULT CALLBACK NodeWin::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	//RECT rcClient;
	//int i;
	//static HWND prevH = nullptr, curH = nullptr;

	switch (umsg)
	{
		//// Check if the window is being closed.
	case WM_CLOSE:
	{
		//cout << "closing..." << endl;

		DestroyWindow(hwnd);
		return 0;
	}
	case WM_MOVE:
	{
		//SendMessage(GetParent(hwnd), USER_1, 0, 0);
		return 0;
	}
	case WM_SIZE:
	{
		//cout << "sizing..." << endl;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		//prevH = hwnd;
		return 0;
	}
	case WM_LBUTTONUP:
	{
		//curH = hwnd;
		//float delta = 0.0f;
		//static float rot = 0.01f;

		//if (prevH == curH)
		//{
		//	cout << "Clicked on same child window" << endl;
		//	return 0;
		//}

		//WindowManager* wM = WindowManager::getWindowManager();
		//NodeWindow *prevNW = wM->findNodeWindow(prevH);
		//NodeWindow *curNW = wM->findNodeWindow(curH);

		//curNW->setConnectedFrom(prevH);
		//prevNW->setConnectedTo(curH);

		//Ops *op = new Ops();
		//delta += rot;
		//rot += 0.01f;
		//op->setRotation(rot);
		//curNW->setOps(op);

		//SendMessage(GetParent(curH), USER_1, 0, 0);

		return 0;
	}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

NodeWin::NodeWin()
	: m_Style(0), m_hwnd(nullptr), m_isContainer(false)
	, m_isSelected(false), m_isCurrent(false)
	, m_Input(nullptr)

{
	Initialize();
}

NodeWin::~NodeWin()
{
	DeInitialize();
}

int NodeWin::Initialize()
{
	HINSTANCE hinst;

	if (!m_isInitialized)
	{
		m_Style = CS_HREDRAW | CS_VREDRAW;
		hinst = GetModuleHandle(NULL);
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = m_Style;
		wcex.lpfnWndProc = NodeWin::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hinst;
		wcex.hIcon = LoadIcon(hinst, IDI_WINLOGO);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 12);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_ClassName;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_WINLOGO);
		if (!RegisterClassEx(&wcex))
			return E_FAIL;

		m_isInitialized = true;
	}
	return true;
}

void NodeWin::DeInitialize()
{
	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
}

int NodeWin::createWindow(HWND parentHwnd)
{
	static int i = 0;
	string title = string(m_Title);

	string s = "-" + to_string(i);
	title += s;

	// Create the node window. 
	m_hwnd = CreateWindowEx(
		0,                      // no extended styles           
		m_ClassName,            // class name                   
		(LPCTSTR)title.c_str(),				// window name                  
		WS_CHILD | WS_BORDER
		| WS_CAPTION
		| WS_THICKFRAME
		| WS_CLIPSIBLINGS
		| WS_OVERLAPPEDWINDOW
		,
		100 + i * 10, 100 + i * 10, 250, 200,
		parentHwnd,           // parent window handle
		NULL, //(HMENU)(int)(ID_FIRSTCHILD + 1),	// class menu used              
		GetModuleHandle(NULL), //m_hinst,				// instance handle              
		NULL);                  // no window creation data      

	if (!m_hwnd)
		return FALSE;

	//Q: Does m_hwnd get a parent hwnd now? 

	//HPQ: suitable place to set these values?
	//if (!NodeWindow::m_ParentHwnd)
	//	NodeWindow::m_ParentHwnd = parentHwnd;
	//if (!NodeWindow::m_hinst)
	//	NodeWindow::m_hinst = GetModuleHandle(NULL);
	++i;

	return TRUE;
}

#include <iostream>
using namespace std;

bool NodeWin::displayWindow()
{
	BringWindowToTop(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW);

	//cout << "displaying window..." << m_hwnd << endl;

	//ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	//UpdateWindow(m_hwnd);

	return true;
}

bool NodeWin::createInputObject()
{
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	//m_Input->Initialize();
}
