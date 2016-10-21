#include <string>

#include "windowManager.h"
#include "NodeWindow.h"

bool NodeWindow::m_isInitialized = false;
HWND NodeWindow::m_ParentHwnd = nullptr;
HINSTANCE NodeWindow::m_hinst = nullptr;
LPCTSTR NodeWindow::m_ClassName = "nodeClass";
LPCTSTR NodeWindow::m_Title = "nodeWindow";

LRESULT CALLBACK NodeWindow::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	RECT rcClient;
	int i;
	static HWND prevH=nullptr, curH = nullptr;

	switch (umsg)
	{
		//// Check if the window is being closed.
		case WM_CLOSE:
		{
			//cout << "closing..." << endl;
			DestroyWindow(hwnd);
			return 0;
		}
		case WM_SIZE:
		{
			//cout << "sizing..." << endl;
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			prevH = hwnd;
			return 0;
		}
		case WM_LBUTTONUP:
		{
			curH = hwnd;
			float delta = 0.0f;
			static float rot = 0.01f;

			if (prevH == curH)
			{
				cout << "Clicked on same child window" << endl;
				return 0;
			}

			WindowManager* wM = WindowManager::getWindowManager();
			NodeWindow *prevNW = wM->findNodeWindow(prevH);
			NodeWindow *curNW = wM->findNodeWindow(curH);

			curNW->setConnectedFrom(prevH);
			prevNW->setConnectedTo(curH);
			
			Ops *op = new Ops();
			delta += rot;
			rot += 0.01f;
			op->setRotation(rot);
			curNW->setOps(op);

			return 0;
		}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

NodeWindow::NodeWindow()
{
	Initialize();
}

NodeWindow::~NodeWindow()
{
	DeInitialize();
}

int NodeWindow::Initialize()
{
	m_Ops = nullptr;

	if (!m_isInitialized)
	{
		m_Style = CS_HREDRAW | CS_VREDRAW;
		m_hinst = GetModuleHandle(NULL);
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = m_Style;
		wcex.lpfnWndProc = NodeWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hinst;
		wcex.hIcon = LoadIcon(m_hinst, IDI_WINLOGO);
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

void NodeWindow::DeInitialize()
{
	if (m_Ops)
	{
		delete m_Ops;
		m_Ops = nullptr;
	}

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
}

int NodeWindow::createWindow(const HWND& parentHwnd)
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
		| WS_OVERLAPPEDWINDOW,
		100 + i * 50, 100 + i * 50, 250, 200,
		parentHwnd,           // parent window handle
		NULL, //(HMENU)(int)(ID_FIRSTCHILD + 1),	// class menu used              
		GetModuleHandle(NULL), //m_hinst,				// instance handle              
		NULL);                  // no window creation data      

	if (!m_hwnd)
		return FALSE;

	//HPQ: suitable place to set these values?
	if (!NodeWindow::m_ParentHwnd)
		NodeWindow::m_ParentHwnd = parentHwnd;
	if (!NodeWindow::m_hinst)
		NodeWindow::m_hinst = GetModuleHandle(NULL);
	++i;

	return TRUE;
}

bool NodeWindow::createInputObject()
{
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();
}

bool NodeWindow::createGraphicsObject()
{
	bool result;

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	result = m_Graphics->Initialize(w, h, m_hwnd);
	if (!result)
	{
		return false;
	}
}

bool NodeWindow::Frame()
{
	bool result;

	result = m_Graphics->Render(this->m_Ops);
	if(!result)
	{
		return false;
	}

	return true;
}

bool NodeWindow::displayWindow()
{
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
	
	return true;
}