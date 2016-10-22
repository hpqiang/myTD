#include <Windows.h>

#include <iostream>
using namespace std;

HINSTANCE hinst = GetModuleHandle(NULL);

#define ID_FIRSTCHILD  100 
#define ID_SECONDCHILD 101 
#define ID_THIRDCHILD  102 

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	RECT rc;
	int i;

	switch (umessage)
	{
		// Check if the window is being destroyed.
		/*case WM_DESTROY:  //HPQ: Cannot put it here, it causes many calls
		{
			cout << "destroying..." << endl;
			DestroyWindow(hwnd);
			return 0;
		}*/

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			cout << "closing..." << endl;
			DestroyWindow(hwnd);
			return 0;
		}
		case WM_MOVE:
		{
			BringWindowToTop(hwnd);
			SetFocus(hwnd);
			UpdateWindow(hwnd);
			return 0;
		}

		case WM_SIZE:
		{
			BringWindowToTop(hwnd);
			SetFocus(hwnd);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			BringWindowToTop(hwnd);
			SetFocus(hwnd);
			break;
		}

		case WM_MOUSEWHEEL:  //HPQ: Have to be a window with SetFocus....
		//case WM_MOUSEHWHEEL:
		{
			short zDelta = GET_WHEEL_DELTA_WPARAM(wparam);

			HWND parentHwnd = GetParent(hwnd);
			RECT parentWinRECT;
			RECT parentClientRECT;
			GetWindowRect(parentHwnd, &parentWinRECT); //screen position
			GetWindowRect(hwnd, &rc);

			int miscHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) +
				GetSystemMetrics(SM_CXPADDEDBORDER));
			int miscWidth = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

			int posX = rc.left - parentWinRECT.left; 
			posX -= miscWidth; 
			int posY = rc.top - parentWinRECT.top; 
			posY -= miscHeight; 

			MoveWindow(hwnd, 
				posX, 
				posY, 
				rc.right - rc.left - zDelta/60,
				rc.bottom - rc.top - zDelta/60, true
			);
			UpdateWindow(hwnd);

			return 0;
		}

		case WM_CREATE:
		{
		//	cout << "Creating..." << endl;
		//	CreateWindowEx(0,
		//		"ChildWClass",
		//		"ChildTitle", //(LPCTSTR)NULL,
		//		WS_CHILD | WS_BORDER
		//		| WS_CAPTION
		//		| WS_THICKFRAME
		//		| WS_CLIPSIBLINGS
		//		| WS_OVERLAPPEDWINDOW,
		//		//| WS_SYSMENU ,
		//		//| WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		//		100, 100, 250, 100 ,
		//		hwnd,
		//		NULL, //(HMENU)(int)(ID_FIRSTCHILD + i),
		//		hinst,
		//		NULL);
			break;
		}
		//case WM_NCHITTEST: 
		//{
		//	//LRESULT hit = DefWindowProc(hwnd, umessage, wparam, lparam);
		//	//if (hit == HTCLIENT) hit = HTCAPTION;
		//	//return hit;
		//	return HTCAPTION;
		//}

	}
	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

BOOL isPointInWin(LPPOINT p, LPRECT rect)
{
	if (p->x >= rect->left && p->x < rect->right
		&& p->y >= rect->top && p->y < rect->bottom)
		return TRUE;
	return FALSE;
}

BOOL CALLBACK EnumChildIntersection(HWND hwndChild, LPARAM lParam)
{
	LPRECT childWinRect = nullptr;
	GetWindowRect(hwndChild, childWinRect);
	LPPOINT p = (LPPOINT)lParam;

	if (isPointInWin(p, childWinRect) == TRUE)
		return TRUE;
	return FALSE;
}

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	LPRECT rcParent;
	int i, idChild;

	// Retrieve the child-window identifier. Use it to set the 
	// position of the child window. 
	idChild = GetWindowLong(hwndChild, GWL_ID);

	if (idChild == ID_FIRSTCHILD)
		i = 0;
	else if (idChild == ID_SECONDCHILD)
		i = 1;
	else
		i = 2;

	// Size and position the child window.  
	rcParent = (LPRECT)lParam;
	MoveWindow(hwndChild,
		(rcParent->right / 6) * i,
		0,
		rcParent->right / 6,
		rcParent->bottom / 2,
		TRUE);

	// Make sure the child window is visible. 
	ShowWindow(hwndChild, SW_SHOW);

	return TRUE;
}

int prevMouseX = 0;
int prevMouseY = 0;
int curMouseX = 0;
int curMouseY = 0;
bool isInSelectMode = false;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	RECT rcClient;
	int i;

	switch (umessage)
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
	case WM_CREATE: // creating main window  
		HWND h[3];
		// Create three invisible child windows. 
		for (i = 0; i < 3; i++)
		{
			h[i] = CreateWindowEx(
				0,
				"ChildWClass",
				"ChildTitle", //(LPCTSTR)NULL,
				WS_CHILD | WS_BORDER
				| WS_CAPTION
				| WS_THICKFRAME
				| WS_CLIPSIBLINGS
				| WS_OVERLAPPEDWINDOW,
				//| WS_SYSMENU ,
				//| WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
				100, 100, 150, 100 * i,
				hwnd,
				(HMENU)(int)(ID_FIRSTCHILD + i),
				hinst,
				NULL);

			//HPQ: Oops, windows home does not suport this, on OS level, it's using 'enalbe transparency glass'
			// Set WS_EX_LAYERED on this window 
			SetWindowLong(h[i],
				GWL_EXSTYLE,
				GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

			// Make this window 50% alpha
			SetLayeredWindowAttributes(h[i], 0, (255 * 50) / 100, LWA_ALPHA);
		}

		return 0;

	case WM_SIZE:   // main window changed size 
	{
		// Get the dimensions of the main window's client 
		// area, and enumerate the child windows. Pass the 
		// dimensions to the child windows during enumeration. 
		GetClientRect(hwnd, &rcClient);
		EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&rcClient);
		return 0;
	}
	//case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	{
		HWND hh;
		// Create three invisible child windows. 
		{
			hh = CreateWindowEx(
				0,
				"ChildWClass",
				"ChildTitle", //(LPCTSTR)NULL,
				WS_CHILD | WS_BORDER
				| WS_CAPTION
				| WS_THICKFRAME
				| WS_CLIPSIBLINGS
				| WS_OVERLAPPEDWINDOW,
				//| WS_SYSMENU ,
				//| WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
				300, 200, 250, 200,
				hwnd,
				NULL, //(HMENU)(int)(ID_FIRSTCHILD + i),
				hinst,
				NULL);

			ShowWindow(hh, SW_SHOW);
		}

		return 0;
	}
	case WM_PAINT:
	{
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		prevMouseX = LOWORD(lparam); //GET_X_LPARAM(lparam);
		prevMouseY = HIWORD(lparam); //GET_Y_LPARAM(lparam);

		POINT p;
		GetCursorPos(&p);
		HWND h = WindowFromPoint(p);

		//cout << "h= " << h << endl;
		//GetClientRect(hwnd, &rcClient);
		//EnumChildWindows(hwnd, EnumChildIntersection, (LPARAM)&p);

		//if (!isInSelectMode)
		if (h == hwnd)
			isInSelectMode = true;
		else
			isInSelectMode = false;

		return 0;
	}

	case WM_LBUTTONUP:
	{
		if (isInSelectMode)
		{
			HDC hdc;
			HGDIOBJ original = NULL;

			hdc = GetDC(hwnd);
			//    Saving the original object
//			original = SelectObject(hdc, GetStockObject(DC_PEN));

			//  The default brush is WHITE_BRUSH
			//SelectObject(hdc, GetStockObject(BLACK_PEN));
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 0, 0));

			MoveToEx(hdc, prevMouseX, prevMouseY, NULL);
			//LineTo(hdc, LOWORD(lparam), HIWORD(lparam));

			Rectangle(hdc,
				prevMouseX, prevMouseY,
				curMouseX, curMouseY);

			ReleaseDC(hwnd, hdc);

			isInSelectMode = false;
		}
		else
		{
			cout << "Not in selection mode" << endl;

			HDC hdc;
			HGDIOBJ original = NULL;
			hdc = GetDC(hwnd);

			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 0, 0));

			MoveToEx(hdc, prevMouseX, prevMouseY, NULL);

			POINT p[3];
			p[0].x = prevMouseX;
			p[0].y = prevMouseY;
			p[1].x = (prevMouseX + curMouseX)/2;
			p[1].y = (prevMouseY + curMouseY) / 2;
			p[2].x = curMouseX;
			p[2].y = curMouseY;

			PolyBezier(hdc, p, 3);

			ReleaseDC(hwnd, hdc);

			isInSelectMode = false;
		}
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		curMouseX = LOWORD(lparam); // GET_X_LPARAM(lparam);
		curMouseY = HIWORD(lparam); // GET_Y_LPARAM(lparam);

		if (isInSelectMode)
		{
			HDC hdc;

			hdc = GetDC(hwnd);
			MoveToEx(hdc, prevMouseX, prevMouseY, NULL);
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(0, 0, 255));

			//Rectangle(hdc,
			//	prevMouseX, prevMouseY,
			//	LOWORD(lparam), HIWORD(lparam));

			POINT p[4];
			p[0].x = prevMouseX;
			p[0].y = prevMouseY;
			p[1].x = (prevMouseX) / 3; // +curMouseX) / 3;
			p[1].y = (prevMouseY + curMouseY) / 3;
			p[2].x = (prevMouseX) * 2 / 3; // +curMouseX) * 2 / 3;
			p[2].y = (prevMouseY + curMouseY) * 2 / 3;
			p[3].x = curMouseX;
			p[3].y = curMouseY;
			PolyBezier(hdc, p, 4);

			ReleaseDC(hwnd, hdc);
		}
		else
		{
//			cout << "Not in selection mode" << endl;

			HDC hdc;
			HGDIOBJ original = NULL;
			hdc = GetDC(hwnd);

			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 0, 0));

			MoveToEx(hdc, prevMouseX, prevMouseY, NULL);

			POINT p[3];
			p[0].x = prevMouseX;
			p[0].y = prevMouseY;
			p[1].x = (prevMouseX + curMouseX) / 2;
			p[1].y = (prevMouseY + curMouseY) / 2;
			p[2].x = curMouseX;
			p[2].y = curMouseY;

			PolyBezier(hdc, p, 3);

			ReleaseDC(hwnd, hdc);

		}
		return 0;
		// Process other messages. 
	}

	}
	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

int main()
{
	HWND hwndMain;
	WNDCLASSEX wcex, childWCEX;
	MSG msg;


	//GdiplusStartupInput gdiplusStartupInput;
	//ULONG_PTR           gdiplusToken;
	//// Initialize GDI+.
	//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wcex.cbSize = sizeof(WNDCLASSEX);
	//CS_DBLCLKS for double clicks to work: https://msdn.microsoft.com/en-us/library/windows/desktop/ms645606(v=vs.85).aspx
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; 
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinst;
	wcex.hIcon = LoadIcon(hinst, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "MainWClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_WINLOGO);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	childWCEX.cbSize = sizeof(WNDCLASSEX);
	childWCEX.style = CS_HREDRAW | CS_VREDRAW;
	childWCEX.lpfnWndProc = ChildWndProc;
	childWCEX.cbClsExtra = 0;
	childWCEX.cbWndExtra = 0;
	childWCEX.hInstance = hinst;
	childWCEX.hIcon = LoadIcon(hinst, IDI_WINLOGO);
	childWCEX.hCursor = LoadCursor(NULL, IDC_ARROW);
	childWCEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 12);
	childWCEX.lpszMenuName = NULL;
	childWCEX.lpszClassName = "ChildWClass";
	childWCEX.hIconSm = LoadIcon(childWCEX.hInstance, IDI_WINLOGO);
	if (!RegisterClassEx(&childWCEX))
		return E_FAIL;

	// Create the main window. 
	hwndMain = CreateWindowEx(
		0,                      // no extended styles           
		"MainWClass",           // class name                   
		"Main Window",          // window name                  
		WS_OVERLAPPEDWINDOW |   // overlapped window            
		WS_HSCROLL |   // horizontal scroll bar        
		WS_VSCROLL,    // vertical scroll bar          
		CW_USEDEFAULT,          // default horizontal position  
		CW_USEDEFAULT,          // default vertical position    
		CW_USEDEFAULT,          // default width                
		CW_USEDEFAULT,          // default height               
		(HWND)NULL,            // no parent or owner window    
		(HMENU)NULL,           // class menu used              
		hinst,              // instance handle              
		NULL);                  // no window creation data      

	if (!hwndMain)
		return FALSE;

	ShowWindow(hwndMain, SW_SHOWDEFAULT);
	UpdateWindow(hwndMain);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};

	//GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}