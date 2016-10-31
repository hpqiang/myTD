#include <iostream>
using namespace std;

#include <Windows.h>
#include <CommCtrl.h>

#include "PropertyWin.h"

#include <string>

bool PropertyWin::m_isInitialized = false;
string PropertyWin::m_ClassName = "propertyWinClass";
HWND PropertyWin::m_sourceNodeHwnd = nullptr;

//HWND hTrack[10];
////HWND hlbl[2];
//
//
//void PropertyWin::CreateControls(HWND hwnd) 
//{
//	//static int labelPos = 0;
//
//	for (int i = 0; i < 10; i++)
//	{
//		HWND hLeftLabel = CreateWindowEx(0, "Static", "0",
//			WS_CHILD | WS_VISIBLE, 0, 0+i*40, 10, 30, hwnd, NULL/*(HMENU)1*/, NULL, NULL);
//
//		HWND hRightLabel = CreateWindowEx(0, "Static", "100",
//			WS_CHILD | WS_VISIBLE, 0, 0+i*40, 30, 30, hwnd, NULL/*(HMENU)2*/, NULL, NULL);
//
//		//hlbl[i] = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE,
//		//	270, 20, 30, 30, hwnd, (HMENU)3, NULL, NULL);
//
//		INITCOMMONCONTROLSEX icex;
//
//		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
//		icex.dwICC = ICC_LISTVIEW_CLASSES;
//
//		//refer to: 1. Right click project and click "Properties"
//		//			2. Link--Input--Additional Dependencies--add "Comctl32.lib"
//		InitCommonControlsEx(&icex);
//
//		hTrack[i] = CreateWindowEx(0, TRACKBAR_CLASSA, "Trackbar Control",
//			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
//			20, 20+i*40, 170, 30, hwnd, NULL/*(HMENU)3*/, NULL, NULL);
//
//		SendMessage(hTrack[i], TBM_SETRANGE, TRUE, MAKELONG(0, 100));
//		SendMessage(hTrack[i], TBM_SETPAGESIZE, 0, 10);
//		SendMessage(hTrack[i], TBM_SETTICFREQ, 10, 0);
//		SendMessage(hTrack[i], TBM_SETPOS, FALSE, 0);
//		SendMessage(hTrack[i], TBM_SETBUDDY, TRUE, (LPARAM)hLeftLabel);
//		SendMessage(hTrack[i], TBM_SETBUDDY, FALSE, (LPARAM)hRightLabel);
//	}
//}
//
//void PropertyWin::UpdateLabel()
//{
//	LRESULT pos[10];
//	for (int i = 0; i < 10; i++)
//	{
//		pos[i] = SendMessage(hTrack[i], TBM_GETPOS, 0, 0);
//
//		cout << "pos[ " << i << "] = " << pos[i] << endl;
//		//cout << "m_sourceNodeHwnd: " << m_sourceNodeHwnd << endl;
//		SendMessage(m_sourceNodeHwnd, WM_NOTIFY, 0, pos[i]);
//		//SendMessageW(NULL, 1000, 0, pos);
//	}
//}

//refer to : http://stackoverflow.com/questions/14661865/use-object-method-as-winapi-wndproc-callback
LRESULT CALLBACK PropertyWin::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	PropertyWin *pW = (PropertyWin *)GetWindowLong(hwnd, GWLP_USERDATA);

//	cout << "hwnd: " << hwnd << endl;
//	cout << "pW--->" << pW << endl;
	if (pW == nullptr)
		return DefWindowProc(hwnd, umsg, wparam, lparam);

	pW->DerivedWinProc(hwnd, umsg, wparam, lparam);
	return DefWindowProc(hwnd, umsg, wparam, lparam);
	//switch (umsg)
	//{
	//case WM_CREATE:
	//{
	//	//refer to : http://zetcode.com/gui/winapi/controlsII/
	//	///*PropertyWin::*/CreateControls(hwnd);
	//	return 0;
	//}
	//case WM_HSCROLL:
	//{
	//	///*PropertyWin::*/UpdateLabel();
	//	return 0;
	//}
	//case WM_CLOSE:
	//{
	//	DestroyWindow(hwnd);
	//	return 0;
	//}
	//case WM_SIZE:
	//{
	//	return 0;
	//}
	//case WM_COMMAND:
	//{
	//	return 0;
	//}
	//}
	//return DefWindowProc(hwnd, umsg, wparam, lparam);
}

PropertyWin::PropertyWin()
	: m_Style(0), m_hwnd(nullptr)

{
	Initialize();
}

PropertyWin::~PropertyWin()
{
	DeInitialize();
}

int PropertyWin::Initialize()
{
	HINSTANCE hinst;

	if (!m_isInitialized)
	{
		m_Style = CS_HREDRAW | CS_VREDRAW;
		hinst = GetModuleHandle(NULL);
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = m_Style;
		wcex.lpfnWndProc = PropertyWin::WndProc;
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

void PropertyWin::DeInitialize()
{
	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
}

int PropertyWin::baseCreateWindow(HWND parentHwnd, /*HWND sourceNodeHwnd, */const string& title)
{
	static int i = 0;
	RECT parentClient;
	//string title = string(m_Title);
	string myTitle = title;
	string s = "-" + to_string(i);
	myTitle += s;
	m_Title = myTitle;

	GetWindowRect(parentHwnd, &parentClient);

	// Create the node window. 
	m_hwnd = CreateWindowEx(
		0,                      // no extended styles           
		m_ClassName.c_str(),            // class name                   
		title.c_str(),				// window name                  
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER
		,
		//100 + i * 10, 100 + i * 10, 250, 200,
		parentClient.right - 200, parentClient.top, 100, 250,
		parentHwnd,           // parent window handle
		NULL, //(HMENU)(int)(ID_FIRSTCHILD + 1),	// class menu used              
		GetModuleHandle(NULL), //m_hinst,				// instance handle              
		NULL);                  // no window creation data      

	if (!m_hwnd)
	{
		cout << "Property Window creation failure" << endl;
		return FALSE;
	}

	//m_sourceNodeHwnd = sourceNodeHwnd;

	++i;

	//refer to : http://stackoverflow.com/questions/14661865/use-object-method-as-winapi-wndproc-callback
	// save the address of the class as the Window's USERDATA.
//	SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);

	return TRUE;
}

bool PropertyWin::displayWindow()
{
	BringWindowToTop(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW);

	return true;
}


PropertyWinD3DOPGeometry::PropertyWinD3DOPGeometry()
{
	//cout << __FUNCTION__ << " with m_hwnd=" << m_hwnd 
	//	<< " and this--> " << this << endl;
	//SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);

}

PropertyWinD3DOPGeometry::~PropertyWinD3DOPGeometry()
{

}


int PropertyWinD3DOPGeometry::createWindow(HWND parentHwnd, HWND sourceNodeHwnd, const string& title)
{
	baseCreateWindow(parentHwnd, title);
	SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);
	m_sourceNodeHwnd = sourceNodeHwnd;
	CreateControls(m_hwnd); //Q: Temp, should not be called here

	//cout << "Size: " << sizeof(GeometryOP) << endl;

	return TRUE;
}

int PropertyWinD3DOPGeometry::DerivedWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//cout << __FUNCTION__ << endl;
	switch (msg)
	{
	case WM_CREATE: //Q: Who ate this WM_CREATE message???
		cout << "Create???" << endl;
		CreateControls(hwnd);
		break;
	case WM_HSCROLL:
	{
		//cout << "Update??? " << endl;
		UpdateLabel();
		return 0;
	}
	default:
		//return DefWindowProc(hwnd, msg, wparam, lparam);
		break;
	}
}

void PropertyWinD3DOPGeometry::CreateControls(HWND hwnd)
{
	
	int i = 0;
	//for (int i = 0; i < sizeof(GeometryOP) / sizeof(float); i++)
	{
		HWND hLeftLabel = CreateWindowEx(0, "Static", "0",
			WS_CHILD | WS_VISIBLE, 0, 0 + i * 40, 10, 30, hwnd, NULL/*(HMENU)1*/, NULL, NULL);

		HWND hRightLabel = CreateWindowEx(0, "Static", "100",
			WS_CHILD | WS_VISIBLE, 0, 0 + i * 40, 30, 30, hwnd, NULL/*(HMENU)2*/, NULL, NULL);

		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_LISTVIEW_CLASSES;

		//refer to: 1. Right click project and click "Properties"
		//			2. Link--Input--Additional Dependencies--add "Comctl32.lib"
		InitCommonControlsEx(&icex);

		/*hTrack[i]*/m_Track[i] = CreateWindowEx(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			20, 20 + i * 40, 170, 30, hwnd, NULL/*(HMENU)3*/, NULL, NULL);

		SendMessage(m_Track[i], TBM_SETRANGE, TRUE, MAKELONG(0, 100));
		SendMessage(m_Track[i], TBM_SETPAGESIZE, 0, 10);
		SendMessage(m_Track[i], TBM_SETTICFREQ, 10, 0);
		SendMessage(m_Track[i], TBM_SETPOS, FALSE, 0);
		SendMessage(m_Track[i], TBM_SETBUDDY, TRUE, (LPARAM)hLeftLabel);
		SendMessage(m_Track[i], TBM_SETBUDDY, FALSE, (LPARAM)hRightLabel);
	}
}

void PropertyWinD3DOPGeometry::UpdateLabel()
{
	//LRESULT pos[10];
	LRESULT pos[1];
	int i = 0;
	//for (int i = 0; i < sizeof(GeometryOP) / sizeof(float); i++)
	{
		pos[i] = SendMessage(m_Track[i], TBM_GETPOS, 0, 0);

		//cout << "pos[ " << i << "] = " << pos[i] << endl;
		//cout << "m_sourceNodeHwnd: " << m_sourceNodeHwnd << endl;
		SendMessage(m_sourceNodeHwnd, WM_NOTIFY, 0, pos[i]);
	}
}
