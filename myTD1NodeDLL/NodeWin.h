#pragma once

#include "Node.h"

class __declspec(dllexport) NodeWin :public Node
{
public:
	//To do: How to deal with WndProc?
	NodeWin();
	~NodeWin();

	HWND getNodeWinHandle() const
	{
		return m_hwnd;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int createWindow(HWND);
	bool displayWindow();

	bool createInputObject();
	//bool createGraphicsObject();

	//bool Frame();

protected:
	int Initialize();
	void DeInitialize();


private:
	static LPCTSTR m_ClassName;  //To do: No LPCTSTR in a file like this
	static LPCTSTR m_Title;
	uint    m_Style;
	HWND	m_hwnd;

	static bool m_isInitialized;

	bool m_isContainer;
	bool m_isSelected;
	bool m_isCurrent;

	InputClass* m_Input;
};
