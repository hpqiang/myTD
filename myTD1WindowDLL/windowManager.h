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

	size_t getNodeWindowsSize() const
	{
		return m_NodeWindows.size();
	}

	void addNodeWindow(NodeWindow* nw)
	{
		try 
		{
			//if (m_Mutex.try_lock())
			std::lock_guard<mutex> lck(m_Mutex);
			//{
				m_NodeWindows.push_back(nw);
			//}
			//m_Mutex.unlock();
		}
		catch(exception& e)
		{
			cout << "exception occured" << endl;
		}
	}

	NodeWindow* findNodeWindow(const int idx) const
	{
		list<NodeWindow *>::const_iterator it;
		int i = 0;
		for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
		{
			if (i == idx)
				return *it;
			++i;
		}
		return nullptr;
	}

	NodeWindow* findNodeWindow(HWND hwnd) const
	{
		list<NodeWindow *>::const_iterator it;
		for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
		{
			if ((*it)->getNodeWindowHandle() == hwnd)
				return *it;
		}
		return nullptr;
	}

	void deleteNodeWindow(HWND hwnd)
	{
		try {
			std::lock_guard<mutex> lck(m_Mutex);

			list<NodeWindow *>::iterator it;
			for (it = m_NodeWindows.begin(); it != m_NodeWindows.end(); ++it)
			{
				if ((*it)->getNodeWindowHandle() == hwnd)
				{
					m_NodeWindows.erase(it);
					return;
				}
			}
		}
		catch (exception& e)
		{
			cout << "exception here" << endl;
		}
		return;
	}

private:
	list<NodeWindow *> m_NodeWindows;
	mutex m_Mutex;
	condition_variable m_CondVar;
};