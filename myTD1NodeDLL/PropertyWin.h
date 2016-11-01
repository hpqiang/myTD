#pragma once

#include "NodeWin.h"
#include "NodeOPD3D.h"

//class IPropertyWin
//{
//	virtual static void CreateControls(HWND hwnd) = 0;
//
//};

class __declspec(dllexport) PropertyWin
{
public:
	PropertyWin();
	virtual ~PropertyWin();

	HWND getNodeWinHandle() const
	{
		return m_hwnd;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	virtual int DerivedWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;

	int baseCreateWindow(HWND parentHwnd, const string& title);
	//virtual int createWindow(HWND parentHwnd, HWND sourceNodeHwnd, const string& title) = 0;
	bool displayWindow();

protected:
	int Initialize();
	void DeInitialize();

protected:
	static string	m_ClassName;
	string			m_Title;
	uint			m_Style;
	HWND			m_hwnd;
	static HWND		m_sourceNodeHwnd;

	static bool m_isInitialized;
};

struct myMinMax
{
	int min;
	int max;
};

struct myLabel
{
	string startLabel;
	string endLabel;
	myMinMax minMax;
};

myLabel my_Labels[] =
{
	{ "RxStart", "RxEnd",{ 0, 100 } },
	{ "RyStart", "RyEnd",{ 0, 100 } },
	{ "RzStart", "RzEnd",{ 0, 100 } },

	{ "TxStart", "TxEnd",{ 0, 100 } },
	{ "TyStart", "TyEnd",{ 0, 100 } },
	{ "TzStart", "TzEnd",{ 0, 100 } },

	{ "SxStart", "SxEnd",{ 0, 100 } },
	{ "SyStart", "SyEnd",{ 0, 100 } },
	{ "SzStart", "SzEnd",{ 0, 100 } },
};

class __declspec(dllexport) PropertyWinD3DOPGeometry : public PropertyWin
{
public:
	PropertyWinD3DOPGeometry();
	~PropertyWinD3DOPGeometry();

	int createWindow(HWND parentHwnd, HWND sourceNodeHwnd, const string& title); // override;
	
	virtual int DerivedWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
	
	void CreateControls(HWND hwnd);
	
	void UpdateLabel();

private:
	PGeometryOP m_OP3DGeometry;
	HWND m_Track[sizeof(my_Labels) / sizeof(my_Labels[0])];  //To do: better way?
};