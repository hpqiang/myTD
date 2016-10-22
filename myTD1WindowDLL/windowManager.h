#pragma once
#include <list>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <iostream>
using namespace std;

#include "rootWindow.h"
#include "NodeWindow.h"
//#include "inputclass.h"

class WindowManager
{
public:
	static WindowManager *getWindowManager();

	WindowManager() {}
	~WindowManager() {}

	size_t getNodeWindowsSize()
	{
		return m_NodeWindows.size();
	}

	void addNodeWindow(NodeWindow* nw)
	{
		m_NodeWindows.push_back(nw);
	}

	NodeWindow* findNodeWindow(int idx)
	{
		list<NodeWindow *>::iterator it;
		int i = 0;
		for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
		{
			if (i == idx)
				return *it;
			++i;
		}
		return nullptr;
	}

	NodeWindow* findNodeWindow(HWND hwnd)
	{
		list<NodeWindow *>::iterator it;
		for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
		{
			if ((*it)->getNodeWindowHandle() == hwnd)
				return *it;
		}
		return nullptr;
	}

	void deleteNodeWindow(HWND hwnd)
	{
		list<NodeWindow *>::iterator it;
		for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
		{
			if ( (*it)->getNodeWindowHandle() == hwnd)
			{
				m_NodeWindows.erase(it);
				return;
			}
		}
		return;
	}

private:
	RootWindow* m_RootWindow;
	HWND	    m_RootWindowHandle;

	list<NodeWindow *> m_NodeWindows;
	mutex muNode;
	condition_variable condNode;
};