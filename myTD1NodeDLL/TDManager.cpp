#include "TDManager.h"
#include "rootWindow.h"

#include "NodeWin.h"

TDManager::TDManager()
	:m_NodeManager(nullptr), m_ContentManager(nullptr)
	, m_RootWindow(nullptr)//m_hinst(nullptr), m_hwnd(nullptr)
{
	m_Node_Contents.clear();
}

TDManager::~TDManager()
{
	cout << "calling " << __FUNCTION__ << endl;
	if (m_NodeManager)
	{
		delete m_NodeManager;
		m_NodeManager = nullptr;
	}
	if (m_ContentManager)
	{
		delete m_ContentManager;
		m_ContentManager = nullptr;
	}

	for (map<INode*, IContent*>::iterator it = m_Node_Contents.begin();
		it != m_Node_Contents.end(); ++it)
	{  //HPQ: To do later
		////if ((*it).second.size() > 0)
		//{
		//	for (IContent* l : it->second)
		//		if (l)
		//		{
		//			delete l;
		//		}
		//	(*it).second.clear();
		//	it++; //Q: Necessary? 
		//}
		//else
		//	it++;
	}
	m_Node_Contents.clear();

	if (m_RootWindow)
	{
		delete m_RootWindow;
		m_RootWindow = nullptr;
	}
//	DestroyWindow(m_hwnd);
//	m_hwnd = NULL;

	// Remove the application instance.
//	UnregisterClass(m_ClassName.c_str(), m_hinst);
//	m_hinst = NULL;
}


bool TDManager::createRootWindow()
{
	m_RootWindow = new RootWindow();

	//m_RootWindow->displayWindow();
	//m_RootWindow->run();

	return TRUE;
}

