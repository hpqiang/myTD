#include "NodeWin.h"
#include "TDSingleton.h"
#include "TDManager.h"

bool NodeWin::m_isInitialized = false;
string NodeWin::m_ClassName = "nodeWinClass";

//To do: should use the one in rootWindow.cpp????
void createNodePopUpMenu(HWND hwnd, LPARAM lParam)
{
	POINT point;
	HMENU hMenu;
	HMENU hSubMenu;

	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	hMenu = CreatePopupMenu();
	ClientToScreen(hwnd, &point);

	for (int i = 0; i < sizeof(NodePopUpMenu) / sizeof(NodePopUpMenu[0]); i++)
	{
		hSubMenu = CreatePopupMenu();

		for (int j = 0; j < sizeof(NodePopUpMenuItem) / sizeof(NodePopUpMenuItem[0]); j++)
		{
			if (NodePopUpMenu[i].id == NodePopUpMenuItem[j].menuId)
			{
				AppendMenu(hSubMenu, MF_STRING | MF_POPUP, NodePopUpMenuItem[j].id, NodePopUpMenuItem[j].menuItemTitle);
			}
		}
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, NodePopUpMenu[i].menuTitle);
	}
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);
	DestroyMenu(hMenu);
}

LRESULT CALLBACK NodeWin::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	static HWND prevH = nullptr; 
	static TDManager::myEvent prevEvent, curEvent;
	TDManager* td_Manager = nullptr;
	TDManager::myEvent e;

	switch (umsg)
	{
	//// Check if the window is being closed.
	case WM_CLOSE:
	{
		cout << "closing..." << endl;
		td_Manager = TDSingleton<TDManager>::getInstance();
		NodeManager* nM = td_Manager->getNodeManager();

		nM->removeNode(hwnd);
		td_Manager->removeFromTo(hwnd);
		// To do: destroy property window if this node win is an OP win

		DestroyWindow(hwnd);
		td_Manager->DrawConnections();

		return 0;
	}
	case WM_ENTERSIZEMOVE:
	{
		td_Manager = TDSingleton<TDManager>::getInstance();
		NodeManager* nM = td_Manager->getNodeManager();
		Node *curNode = nM->findNode(hwnd);

		curNode->drawConnection(true, true);

		return 0;
	}

	case WM_MOVE:
	{
		td_Manager = TDSingleton<TDManager>::getInstance();
		NodeManager* nM = td_Manager->getNodeManager();

		Node *curNode = nM->findNode(hwnd);

		if (curNode != nullptr)
			curNode->drawConnection(true, false);

		break;
	}
	case WM_SIZE:
	{
		td_Manager = TDSingleton<TDManager>::getInstance();
		NodeManager* nM = td_Manager->getNodeManager();

		Node *curNode = nM->findNode(hwnd);

		if (curNode != nullptr)
			curNode->drawConnection(true, false);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		prevH = hwnd;

		break;
	}
	case WM_LBUTTONUP:
	{
		if (prevH == hwnd)
		{
			cout << "mouse up detected in same window..." << endl;
			return 0;
		}

		td_Manager = TDSingleton<TDManager>::getInstance();
		NodeManager* nM = td_Manager->getNodeManager();

		Node *prevNode = nM->findNode(prevH);
		Node *curNode = nM->findNode(hwnd);

		//To do: If there is already a connection, don't do below
		prevNode->addNodeOut(curNode);
		curNode->addNodeIn(prevNode);

		curNode->drawConnection(true, false);

		break;
	}
	case WM_RBUTTONUP:
	{
		createNodePopUpMenu(hwnd, lparam);

		break;
	}

	case WM_COMMAND:
	{
		//step1: create if not exist(?) corresponding property window
		//for (int i = 0; i < sizeof(popUpMenu) / sizeof(popUpMenu[0]); i++)
		{
			for (int j = 0; j < sizeof(NodePopUpMenuItem) / sizeof(NodePopUpMenuItem[0]); j++)
			{
				if (NodePopUpMenuItem[j].id == LOWORD(wparam))
				{
					cout << "This: " << LOWORD(wparam) << " is clicked..." << endl;
					cout << "POPUP: **********with command: " << NodePopUpMenuItem[j].command.c_str() << endl;
					cout << "And HWND: " << hwnd << endl;
					e.event = NodePopUpMenuItem[j].id;//To do: Should translate msgs like this?
					e.hwnd = hwnd;
					e.command = NodePopUpMenuItem[j].command;
					e.wparam = wparam;
					e.lparam = lparam;
					td_Manager = TDSingleton<TDManager>::getInstance();
					//for (int i = 0; i < 2; i++)  //Hehe: testing
						td_Manager->sendEvent(e);
				}
			}
		}
		//step2: decide corresponding content in the client area
		//		 e.g., D3D geometry content???

		break;
	}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

NodeWin::NodeWin()
	: m_Style(0), m_hwnd(nullptr), m_isContainer(false)
	, m_isSelected(false), m_isCurrent(false)

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
		wcex.lpszClassName = m_ClassName.c_str();
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_WINLOGO);
		if (!RegisterClassEx(&wcex))
			return E_FAIL;

		m_isInitialized = true;
	}
	return true;
}

void NodeWin::DeInitialize()
{
	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
}

int NodeWin::createWindow(HWND parentHwnd, const string& title)
{
	static int i = 0;
	string myTitle = title;
	string s = "-" + to_string(i);
	myTitle += s;
	m_Title = myTitle;

	// Create the node window. 
	m_hwnd = CreateWindowEx(
		0,							// no extended styles           
		m_ClassName.c_str(),        // class name                   
		title.c_str(),				// window name                  
		WS_CHILD | WS_BORDER
		| WS_CAPTION
		| WS_THICKFRAME
		| WS_CLIPSIBLINGS
		| WS_OVERLAPPEDWINDOW
		,
		100 + i * 10, 100 + i * 10, 250, 200,
		parentHwnd,					// parent window handle
		NULL,						// class menu used              
		GetModuleHandle(NULL), 		// instance handle              
		NULL);						// no window creation data      

	if (!m_hwnd)
		return FALSE;

	++i;

	return TRUE;
}

bool NodeWin::displayWindow()
{
	BringWindowToTop(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW);

	return true;
}