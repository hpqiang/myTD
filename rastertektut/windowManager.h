#pragma once
#include <list>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <iostream>
using namespace std;

#include "rootWindow.h"
#include "NodeWindow.h"
#include "inputclass.h"

class WindowManager
{
public:
	static WindowManager *getWindowManager();

	WindowManager() {}
	~WindowManager() {}

	RootWindow* getRootWindow()
	{
		return m_RootWindow;
	}
	void setRootWindow(RootWindow* rw)
	{
		m_RootWindow = rw;
	}

	HWND getRootWindowHandle()
	{
		return m_RootWindowHandle;
	}
	void setRootWindowHandle(HWND h)
	{
		m_RootWindowHandle = h;
	}

	size_t getNodeWindowsSize()
	{
		return m_NodeWindows.size();
	}

	void addNodeWindow(NodeWindow* nw)
	{
		m_NodeWindows.push_back(nw);
	}
	//void addNodeWindow()
	//{
	//	//if (i > m_NodeWindows.size())
	//	for (int i = 0; i < 3; i++)
	//	{
	//		NodeWindow* nodeWindow = new NodeWindow();
	//		nodeWindow->createWindow();
	//		m_NodeWindows.push_back(nodeWindow);

	//		nodeWindow->displayWindow();
	//		//nodeWindow->run();
	//	}
	//	//else
	//	//	cout << "already created" << endl;
	//}

	NodeWindow* findNodeWindow(int idx)
	{
		list<NodeWindow *>::iterator it;
		int i = 0;
		for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
		{
			//for (int i = 0; i < m_NodeWindows.size(); ++i)
			//{
				if (i == idx)
					return *it;
				++i;
			//}
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

	//InputClass* getInputClass()
	//{
	//	return m_Input;
	//}
	//void setInputClass(InputClass * in)
	//{
	//	m_Input = in;
	//}

	NodeWindow* getNodeWindowsHead() 
	{
		return m_NodeWindows.front();
	}
	void setNodeWindowsHead(NodeWindow* wn)
	{
		if (m_NodeWindows.size() == 0)
		{
			m_NodeWindows.push_back(wn);
		}
	}

protected:
	//void run();

private:
	RootWindow* m_RootWindow;
	HWND	    m_RootWindowHandle;

	list<NodeWindow *> m_NodeWindows;
	mutex muNode;
	condition_variable condNode;

	//InputClass* m_Input;
};