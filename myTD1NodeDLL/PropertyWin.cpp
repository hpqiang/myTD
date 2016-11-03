#include <iostream>
#include <string>
using namespace std;

#include <Windows.h>
#include <CommCtrl.h>

#include "PropertyWin.h"

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
//Q: WM_CREATE cannot be sent to derived winproc???
LRESULT CALLBACK PropertyWin::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PropertyWin *pW = (PropertyWin *)GetWindowLong(hwnd, GWLP_USERDATA);

	//if (umsg == WM_CREATE)
	//{
	//	cout << __FUNCTION__ << "WM_CREATE!!!!" << endl;
	//}

	//	cout << "hwnd: " << hwnd << endl;
	//	cout << "pW--->" << pW << endl;
	if (pW == nullptr)
	{
		//cout << "pW is nullptr with umsg = " << msg << endl;
		//return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	else
	{
		pW->DerivedWinProc(hwnd, msg, wparam, lparam);
		//return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	//PropertyWinD3DOPGeometry *pThis;

	////cout << "msg: " << msg << endl;

	////Refer to: http://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function
	//if (msg == WM_NCCREATE)
	////if (msg == WM_CREATE)
	//{
	//	cout << "create msg: " << msg << endl;

	//	//pThis = static_cast<PropertyWinD3DOPGeometry *>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);
	//	pThis = (PropertyWinD3DOPGeometry *)(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

	//	SetLastError(0);
	//	//if (!SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
	//	if (!SetWindowLong(hwnd, GWL_USERDATA, (long)(pThis)))
	//	{
	//		if (pThis != nullptr)
	//		{
	//			pThis->CreateControls(hwnd);
	//		}
	//		else
	//		{
	//			cout << "pThis is nullptr" << endl;
	//		}
	//		if (GetLastError() != 0)
	//			return FALSE;
	//	}
	//}
	//else
	//{
	//	cout << "non create msg: " << msg << endl;
	//	//pW = reinterpret_cast<PropertyWin *>(GetWindowLongPtr(hwnd, GWL_USERDATA));
	//	pW = (PropertyWin *)(GetWindowLong(hwnd, GWL_USERDATA));
	//	if (pW != nullptr)
	//	{
	//		pW->DerivedWinProc(hwnd, msg, wparam, lparam);
	//	}
	//	else
	//	{
	//		cout << "pW is nullptr!" << endl;
	//	}
	//}


	//switch (msg)
	//{
	//case WM_CREATE:
	//{
	//	//refer to : http://zetcode.com/gui/winapi/controlsII/
	//	///*PropertyWin::*/CreateControls(hwnd);

	//	//SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);

	//	cout << __FUNCTION__ << "***************WM_CREATE!!!!" << endl;
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
	return DefWindowProc(hwnd, msg, wparam, lparam);
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

int PropertyWin::baseCreateWindow(HWND parentHwnd, const string& title)
{
	static int titleNum = 0;
	RECT parentClient;
	string myTitle = title;
	string s = "-" + to_string(titleNum);
	myTitle += s;
	m_Title = myTitle;

	GetWindowRect(parentHwnd, &parentClient);

	// Create the node window. 
	m_hwnd = CreateWindowEx(
		0,						// no extended styles           
		m_ClassName.c_str(),	// class name                   
		title.c_str(),			// window name                  
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER
		,
		parentClient.right - 200, parentClient.top, 100, 250,
		parentHwnd,				// parent window handle
		NULL,					// class menu used              
		GetModuleHandle(NULL), 	// instance handle              
		NULL);                  // no window creation data      

	if (!m_hwnd)
	{
		cout << "Property Window creation failure" << endl;
		return FALSE;
	}
	++titleNum;

	//SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);


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
	m_OP3DGeometry = new GeometryOP();
	memset(m_OP3DGeometry, 0, sizeof(GeometryOP));
}

PropertyWinD3DOPGeometry::~PropertyWinD3DOPGeometry()
{
	if (m_OP3DGeometry != nullptr)
	{
		delete m_OP3DGeometry;
		m_OP3DGeometry = nullptr;
	}
}

//refer to : http://stackoverflow.com/questions/14661865/use-object-method-as-winapi-wndproc-callback
// save the address of the class as the Window's USERDATA.
int PropertyWinD3DOPGeometry::createWindow(HWND parentHwnd, HWND sourceNodeHwnd, const string& title)
{
	baseCreateWindow(parentHwnd, title);
	SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);
	m_sourceNodeHwnd = sourceNodeHwnd;
	CreateControls(m_hwnd); //Q: Temp, should not be called here

	return TRUE;
}

int PropertyWinD3DOPGeometry::DerivedWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	////if (msg == WM_CREATE)
	//{
	//	cout << __FUNCTION__ << msg << endl;
	//}

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
		break;
	}
	return 0;
}

void PropertyWinD3DOPGeometry::CreateControls(HWND hwnd)
{
	for (int i = 0; i < sizeof(my_Labels) / sizeof(my_Labels[0]); i++)
	{
		HWND hLeftLabel = CreateWindowEx(0, "Static", my_Labels[i].startLabel.c_str(), //"0",
			WS_CHILD | WS_VISIBLE, 0, 0 + i * 40, 10, 30, hwnd, NULL/*(HMENU)1*/, NULL, NULL);

		HWND hRightLabel = CreateWindowEx(0, "Static", my_Labels[i].endLabel.c_str(),//"100",
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
	LRESULT pos[sizeof(my_Labels) / sizeof(my_Labels[0])];

	//LRESULT pos[1];
	//int i = 0;
	for (int i = 0; i < sizeof(my_Labels) / sizeof(my_Labels[0]); i++)
	{
		pos[i] = SendMessage(m_Track[i], TBM_GETPOS, 0, 0);

		//cout << "pos[ " << i << "] = " << pos[i] << endl;
		//cout << "m_sourceNodeHwnd: " << m_sourceNodeHwnd << endl;

		////Q: Risky doing below. Need to be 'atomic' and send to central controller: TDManager
		//SendMessage(m_sourceNodeHwnd, WM_NOTIFY, 0, pos[i]);
		//*m_OP3DGeometry++ = (float)pos[i] / (float) 100;
	}

	float temp[sizeof(my_Labels) / sizeof(my_Labels[0])];

	for (int i = 0; i < sizeof(my_Labels) / sizeof(my_Labels[0]); i++)
	{
		temp[i] = (float)pos[i];
	}
	memcpy(m_OP3DGeometry, temp, sizeof(GeometryOP));

	//cout << "m_OP3DGeometry Rx: " << m_OP3DGeometry->getRotation().m_Rx << endl;
	//cout << "m_OP3DGeometry Ry: " << m_OP3DGeometry->getRotation().m_Ry << endl;
	//cout << "m_OP3DGeometry Rz: " << m_OP3DGeometry->getRotation().m_Rz << endl;

	//cout << "m_OP3DGeometry Tx: " << m_OP3DGeometry->getTranslation().m_Tx << endl;
	//cout << "m_OP3DGeometry Ty: " << m_OP3DGeometry->getTranslation().m_Ty << endl;
	//cout << "m_OP3DGeometry Tz: " << m_OP3DGeometry->getTranslation().m_Tz << endl;

	//cout << "m_OP3DGeometry Sx: " << m_OP3DGeometry->getScalar().m_Sx << endl;
	//cout << "m_OP3DGeometry Sy: " << m_OP3DGeometry->getScalar().m_Sy << endl;
	//cout << "m_OP3DGeometry Sz: " << m_OP3DGeometry->getScalar().m_Sz << endl;

	SendMessage(m_sourceNodeHwnd, WM_NOTIFY, sizeof(GeometryOP), (LPARAM)m_OP3DGeometry);
}
