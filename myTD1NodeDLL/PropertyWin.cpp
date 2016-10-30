#include <iostream>
using namespace std;

#include <Windows.h>
#include <CommCtrl.h>

#include "PropertyWin.h"

#include <string>

bool PropertyWin::m_isInitialized = false;
string PropertyWin::m_ClassName = "propertyWinClass";
HWND PropertyWin::m_sourceNodeHwnd = nullptr;

HWND hTrack;
HWND hlbl;


void PropertyWin::CreateControls(HWND hwnd) {

	HWND hLeftLabel = CreateWindowW(L"Static", L"0",
		WS_CHILD | WS_VISIBLE, 0, 0, 10, 30, hwnd, (HMENU)1, NULL, NULL);

	HWND hRightLabel = CreateWindowW(L"Static", L"100",
		WS_CHILD | WS_VISIBLE, 0, 0, 30, 30, hwnd, (HMENU)2, NULL, NULL);

	hlbl = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE,
		270, 20, 30, 30, hwnd, (HMENU)3, NULL, NULL);

	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;

	//refer to: 1. Right click project and click "Properties"
	//			2. Link--Input--Additional Dependencies--add "Comctl32.lib"
	InitCommonControlsEx(&icex);

	hTrack = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control",
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		20, 20, 170, 30, hwnd, (HMENU)3, NULL, NULL);

	SendMessageW(hTrack, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
	SendMessageW(hTrack, TBM_SETPAGESIZE, 0, 1);
	SendMessageW(hTrack, TBM_SETTICFREQ, 10, 0);
	SendMessageW(hTrack, TBM_SETPOS, FALSE, 0);
	SendMessageW(hTrack, TBM_SETBUDDY, TRUE, (LPARAM)hLeftLabel);
	SendMessageW(hTrack, TBM_SETBUDDY, FALSE, (LPARAM)hRightLabel);
}

void PropertyWin::UpdateLabel()
{
	LRESULT pos = SendMessageW(hTrack, TBM_GETPOS, 0, 0);
	
	cout << "pos = " << pos << endl;
	cout << "m_sourceNodeHwnd: " << m_sourceNodeHwnd << endl;
	SendMessageW(m_sourceNodeHwnd, WM_NOTIFY, 0, pos);
	//SendMessageW(NULL, 1000, 0, pos);
}

LRESULT CALLBACK PropertyWin::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{

	switch (umsg)
	{
	case WM_CREATE:
	{
		//refer to : http://zetcode.com/gui/winapi/controlsII/
		PropertyWin::CreateControls(hwnd);
		return 0;
	}
	case WM_HSCROLL:
	{
		PropertyWin::UpdateLabel();
		return 0;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	case WM_SIZE:
	{
		return 0;
	}
	case WM_COMMAND:
	{
		return 0;
	}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
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

int PropertyWin::createWindow(HWND parentHwnd, HWND sourceNodeHwnd, const string& title)
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
		//WS_CHILD | WS_BORDER
		//| WS_CAPTION
		//| WS_THICKFRAME
		//| WS_CLIPSIBLINGS
		//| WS_OVERLAPPEDWINDOW
		WS_OVERLAPPEDWINDOW | WS_VISIBLE
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

	m_sourceNodeHwnd = sourceNodeHwnd;

	++i;

	return TRUE;
}

bool PropertyWin::displayWindow()
{
	BringWindowToTop(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW);

	return true;
}