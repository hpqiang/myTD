#include "CommonDefs.h"
#include "TDSingleton.h"
#include "TDManager.h"
#include "rootWindow.h"

#include "inputclass.h"

//HPQ: refer to http://zetcode.com/gui/winapi/menus/
void AddMenu(HWND hwnd)
{
	HMENU hMenubar; 
	HMENU hMenu; 
	HMENU hSubMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();
	hSubMenu = CreatePopupMenu();

	for (int i = 0; i < sizeof(mainMenu) / sizeof(mainMenu[0]); i++)
	{
		hMenu = CreateMenu();
		for (int j = 0; j < sizeof(mainMenuItem) / sizeof(mainMenuItem[0]); j++)
		{
			if (mainMenu[i].id == mainMenuItem[j].menuId)
			{
				AppendMenu(hMenu, MF_STRING, mainMenuItem[j].id, mainMenuItem[j].menuItemTitle);
			}
		}
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, mainMenu[i].menuTitle);
	}
	SetMenu(hwnd, hMenubar);

	//To do: How to track which menu item is clicked?
}

void createPopUpMenu(HWND hwnd, LPARAM lParam)
{
	POINT point;
	HMENU hMenu;
	HMENU hSubMenu;

	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	hMenu = CreatePopupMenu();
	ClientToScreen(hwnd, &point);

	for (int i = 0; i < sizeof(popUpMenu) / sizeof(popUpMenu[0]); i++)
	{
		hSubMenu = CreatePopupMenu();

		for (int j = 0; j < sizeof(popUpMenuItem) / sizeof(popUpMenuItem[0]); j++)
		{
			if (popUpMenu[i].id == popUpMenuItem[j].menuId)
			{
				AppendMenu(hSubMenu, MF_STRING | MF_POPUP, popUpMenuItem[j].id, popUpMenuItem[j].menuItemTitle);
			}
		}
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, popUpMenu[i].menuTitle);
	}
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);
	DestroyMenu(hMenu);
}

LRESULT CALLBACK RootWindow::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	static RECT prevRECT;
	static bool bPrevLine = false;
	TDManager* td_Manager = nullptr;
	NodeManager* nM = nullptr;
	myEvent e;

	switch (umsg)
	{
		case WM_CREATE:
		{
			AddMenu(hwnd);
			return 0;
		}

		case WM_RBUTTONUP:
		{
			createPopUpMenu(hwnd, lparam);

			break;
		}

		case WM_COMMAND:
		{
			for (int i = 0; i < sizeof(mainMenu) / sizeof(mainMenu[0]); i++)
			{
				for (int j = 0; j < sizeof(mainMenuItem) / sizeof(mainMenuItem[0]); j++)
				{
					if (mainMenuItem[j].id == LOWORD(wparam))
					{
						cout << "MAIN: This: " << LOWORD(wparam)<< " is clicked..." << endl;
					}
				}
			}

			//for (int i = 0; i < sizeof(popUpMenu) / sizeof(popUpMenu[0]); i++)
			{
				for (int j = 0; j < sizeof(popUpMenuItem) / sizeof(popUpMenuItem[0]); j++)
				{
					if (popUpMenuItem[j].id == LOWORD(wparam))
					{
						e.event = popUpMenuItem[j].id;//To do: Should translate msgs like this?
						e.hwnd = hwnd;
						e.command = popUpMenuItem[j].command;
						e.wparam = wparam;
						e.lparam = lparam;
						td_Manager = TDSingleton<TDManager>::getInstance();
						//for (int i = 0; i < 2; i++)  //Hehe: testing
							td_Manager->sendEvent(e);
					}
				}
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

		case WM_LBUTTONDOWN:
		{
			td_Manager = TDSingleton<TDManager>::getInstance();
			NodeManager* nM = td_Manager->getNodeManager();

			if (nM->isHittingConnLine(LOWORD(lparam), HIWORD(lparam)) == true)
				;

			break;
		}

		case WM_KEYDOWN:
		{
			switch (wparam)
			{
			case VK_DELETE:
			{
				td_Manager = TDSingleton<TDManager>::getInstance();
				NodeManager* nM = td_Manager->getNodeManager();

				nM->deleteDeleteCandidateNodes();
				nM->drawConnections(false);

				break;
			}
			}
		}

	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

RootWindow::RootWindow()
	: m_ClassName(""), m_Title("")
	, m_Style(0), m_hinst(nullptr), m_hwnd(nullptr)
{
	Initialize();

//	m_Input = new InputClass;
//	if (!m_Input)
//	{
//		return; // false;
//	}
//
//	//// Initialize the input object.
//	RECT rect;
//	//GetWindowRect(hwnd, &rect);
//	//GetClientRect(hwnd, &rect);
//	//int w = rect.right - rect.left;
//	//int h = rect.bottom - rect.top;
//	int w = GetSystemMetrics(SM_CXSCREEN);
//	int h = GetSystemMetrics(SM_CYSCREEN);
////	HWND parentHwnd = GetParent(hwnd);
//	//GetWindowRect(parentHwnd, &rect);
//	//int w = rect.right - rect.left;
//	//int h = rect.bottom - rect.top;
//	m_Input->Initialize(m_hinst, m_hwnd, w, h);
//
//
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
	wcex.lpszMenuName = "simple menu";
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

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ClassName, m_hinst);
	m_hinst = NULL;

	return;
}

bool RootWindow::isHittingConnLine(long x, long y, long startX, long startY,
	long endX, long endY)
{
	if (x < min(startX, endX) || x > max(startX, endX)
		|| y < min(startY, endY) || y > max(startY, endY))
	{
		return false;
	}
	
	float k1 = (float)(y - startY) / (float)(x - startX);
	float k2 = (float)(endY - startY) / (float)(endX - startX);
	if (((k1 - k2) < 0.1f) && ((k1 - k2) > -0.1f))
	{
		return true;
	}
	else
		return false;
}

bool RootWindow::getWindowMiddle(HWND hwnd, long *x, long *y, bool isRight)
{
	HWND parentHwnd = GetParent(hwnd);
	RECT parentClientRect, winRect;
	GetClientRect(parentHwnd, &parentClientRect);

	//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
	GetWindowRect(hwnd, &winRect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hwnd), (LPPOINT)&winRect, 2);

	if (isRight)
	{
		*x = winRect.right;
		*y = winRect.top + (winRect.bottom - winRect.top) / 2;
	}
	else
	{
		*x = winRect.left;
		*y = winRect.bottom - (winRect.bottom - winRect.top) / 2;
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
	bool done; // , result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	TDManager* td_Manager = TDSingleton<TDManager>::getInstance();

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		while (td_Manager->getEventQueueSize() > 0)
		{
			td_Manager->processEvent();
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			// To do: Why this Frame will have m_mouse always return nullptr????
			// Refer rastertut's rendertotexture, why they are successful???
			//m_Input->Frame();

			td_Manager->Update();
			td_Manager->Render();  //To do: Render should have a return value
//			result = Frame();
			//if (!result)
			//{
			//	done = true;
			//}
		}
	}
}