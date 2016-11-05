#include "NodeWin.h"
#include "TDSingleton.h"
#include "TDManager.h"
#include "NodeOPD3D.h"

bool NodeWin::m_isInitialized = false;
string NodeWin::m_ClassName = "nodeWinClass";

//To do: should use the one in rootWindow.cpp????
void createNodePopUpMenu(HWND hwnd, LPARAM lParam)
{
	//cout << __FUNCTION__ << endl;
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
	static bool bCanDrawLine = false;
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

		DestroyWindow(hwnd);
//		td_Manager->DrawConnections();

		return 0;
	}
	case WM_MOVE:
	{
		if (!bCanDrawLine)
		{
			cout << "Not an actual move..." << endl;
			return 0;
		}

		curEvent.event = 0; //Q: Is ths event important here?
		curEvent.command = "Node Window Moved";
		curEvent.hwnd = hwnd;
		curEvent.wparam = wparam;
		curEvent.lparam = lparam;

		td_Manager = TDSingleton<TDManager>::getInstance();
		td_Manager->sendEvent(curEvent);

		prevEvent = {};

		break;
	}
	case WM_SIZE:
	{
		break;
	}
	case WM_LBUTTONDOWN:
	{

		prevH = hwnd;

		prevEvent.event = 0; //Q: Is ths event important here?
		prevEvent.command = "Prepare Draw Line";
		prevEvent.hwnd = hwnd;
		prevEvent.wparam = wparam;
		prevEvent.lparam = lparam;

		break;
	}
	case WM_LBUTTONUP:
	{
		if (prevH == hwnd)
		{
			cout << "mouse up detected in same window..." << endl;
			return 0;
		}

		bCanDrawLine = true;
		curEvent.event = 0; //Q: Is ths event important here?
		curEvent.command = "Draw Line";
		curEvent.hwnd = hwnd;
		curEvent.wparam = wparam;
		curEvent.lparam = lparam;

		RECT startRECT;
		RECT endRECT;

		GetWindowRect(prevEvent.hwnd, &startRECT);
		GetWindowRect(hwnd, &endRECT);

		cout << "Before sending: From " << prevEvent.hwnd << "To: " << hwnd << endl;
		cout << " startRECT: " << startRECT.right <<
			": " << startRECT.top << endl;
		cout << "                end RECT: " << endRECT.left <<
			": " << endRECT.bottom << endl;

		td_Manager = TDSingleton<TDManager>::getInstance();
		td_Manager->sendEvent(prevEvent);
		td_Manager->sendEvent(curEvent);

		prevEvent = {}; //Q: Not necessary since both prevEvent and curEvent had been consumed???
		curEvent = {};

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
	//case 1000:
	case WM_NOTIFY:
	{
		//Q: Should have a derived class WndProc???
		memcpy(m_OP3DGeometry, (PGeometryOP)lparam, wparam);

		//cout << "rotX : " << m_OP3DGeometry->getRotation().m_Rx << "\t";
		//cout << "rotY : " << m_OP3DGeometry->getRotation().m_Ry << "\t";
		//cout << "rotZ : " << m_OP3DGeometry->getRotation().m_Rz << endl;
		break;
	}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

NodeWin::NodeWin()
	: m_Style(0), m_hwnd(nullptr), m_isContainer(false)
	, m_isSelected(false), m_isCurrent(false)
	, m_Input(nullptr)

{
	m_OP3DGeometry = new GeometryOP();
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
	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

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
		0,                      // no extended styles           
		m_ClassName.c_str(),            // class name                   
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

bool NodeWin::createInputObject()
{
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	//m_Input->Initialize();
	return true;
}


//To do: This function should be in NodeWinD3DGeometry and other derived classes
void NodeWinD3D::getD3DConnectionOP(myD3DConnectionOP *op)
{
	TDManager* tM = TDSingleton<TDManager>::getInstance();
	NodeManager* nM = tM->getNodeManager();
	NodeWin*	nW = nullptr;
	//step1: find all From windows connected to this window
	HWND *hwndFrom = new HWND[5]; //temp: Assuming no more than 5 connections
	int foundNum = 0;

	if (tM->findFrom(m_hwnd, hwndFrom, &foundNum) != false)
	{
		//cout << "found From number is: " << foundNum << endl;
		//step2: find the node window associate with hwndFrom

		for (int i = 0; i < foundNum; i++)
		{
			nW = nM->getObjectByHwnd(hwndFrom[i]);
			//Temp: call NodeOPD3D's method directly, should eliminate dynamic cast
			dynamic_cast<NodeOPD3D *>(nW)->getD3DConnectionOP(m_D3DConnectionOP);
		}
	}
}