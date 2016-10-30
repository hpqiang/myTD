#pragma once

//#include <CommCtrl.h>
#include "NodeWin.h"

class __declspec(dllexport) PropertyWin// : public NodeWin //Temp: Should not derived from NodeWin?
{
	//INITCOMMONCONTROLSEX icex;
public:
	//To do: How to deal with WndProc?
	PropertyWin();
	virtual ~PropertyWin();

	HWND getNodeWinHandle() const
	{
		return m_hwnd;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static void CreateControls(HWND hwnd);
	static void UpdateLabel();

	int createWindow(HWND, HWND, const string& title);
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

class PropertyWinD3DGeometry : public PropertyWin
{
public:

private:

};