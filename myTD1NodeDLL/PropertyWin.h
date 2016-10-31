#pragma once

//#include <CommCtrl.h>
#include "NodeWin.h"
#include "NodeOPD3D.h"

//#pragma comment(lib, "CommCtl32.dll")

//class IPropertyWin
//{
//	virtual static void CreateControls(HWND hwnd) = 0;
//
//};

class __declspec(dllexport) PropertyWin// : public NodeWin //Temp: Should not derived from NodeWin?
{
public:
	//To do: How to deal with WndProc?
	PropertyWin();
	virtual ~PropertyWin();

	HWND getNodeWinHandle() const
	{
		return m_hwnd;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	virtual int DerivedWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
//	/*static*/ void CreateControls(HWND hwnd);
//	/*static*/ void UpdateLabel();

	int baseCreateWindow(HWND parentHwnd, /*HWND sourceNodeHwnd, */const string& title);
	//virtual int createWindow(HWND parentHwnd, HWND sourceNodeHwnd, const string& title) = 0;
	bool displayWindow();

protected:
	int Initialize();
	void DeInitialize();

protected:
	static string m_ClassName;
	string	m_Title;
	uint    m_Style;
	HWND	m_hwnd;
	static HWND	m_sourceNodeHwnd;

	static bool m_isInitialized;

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
	//HWND *m_Track; // [10];
	HWND m_Track[sizeof(GeometryOP) / sizeof(float)];
};