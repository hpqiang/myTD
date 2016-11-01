#pragma once
#include <iostream>
using namespace std;

#include "Node.h"
#include "NodeWin.h"
#include "NodeOP.h"
#include "NodeOPD3D.h"

#include "Content.h"
#include "rootWindow.h"
#include "TDSingleton.h"
#include "TDFactory.h"

#include "PropertyWin.h"

#include <thread>

class NodeManager
{
public:
	NodeManager() 
	{ 
		m_Nodes.clear();
		//cout << "In cstr, m_nodes size: " << m_Nodes.size() << endl;
	}
	~NodeManager() 
	{
		// to do: more processing
		m_Nodes.clear();
	} 
	
	// to do : add CRUD for Node handling
	
	void addNode(INode* node)
	{
		m_Nodes.push_back(node);
	}
	
	uint getNodesSize()
	{
		return m_Nodes.size();
	}

	NodeWin* findNodeAt(int idx)
	{
		list<INode *>::iterator it;
		int i = 0;

		for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
		{
			if (i == idx)
			{
				NodeWin* node = dynamic_cast<NodeWin *>(*it);
				return node;
			}
			else
				i++;
		}
		cout << "Did not find the INode* with idx" << endl;
		return nullptr;
	}

	NodeWin* getObjectByHwnd(HWND hwnd)
	{
		list<INode *>::iterator it;
		for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
		{
			NodeWin *node = dynamic_cast<NodeWin *>(*it);
			if ( node->getNodeWinHandle() == hwnd )
			{
				return node;
			}
		}
		cout << "Did not find the Inode with hwnd" << endl;
		return nullptr;
	}

	void Render()
	{
		//cout << __FUNCTION__ << endl;
		if (m_Nodes.empty())
			return; 
		list<INode *>::iterator it;
		//int rot = 0;

		for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
		{
			//To do: eliminate the dynamic_cast
			dynamic_cast<NodeWin *>(*it)->Render();
		}
	}

protected:
	bool isConnectable(INode* nodeFrom, INode* nodeTo)
	{
		return true;
	}

	bool detectNodeLoop()
	{

	}
private:
	list<INode *>	m_Nodes;
	mutex	m_Mutex;
	condition_variable m_CondVar;

	NodeManager* m_Composite;  //should support composite pattern?
	//To do: Should process using graph???
};

class ContentManager
{
public:
	ContentManager() {}
	~ContentManager() {} //Prevent others to call it
						 // to do : add CRUD for Node handling
private:
	list<IContent *>	m_Nodes;
	mutex				m_Mutex;
	condition_variable	m_CondVar;

};

//For LNK2005 error, add /FORCE:MULTIPLE in linker option: http://stackoverflow.com/questions/10046485/error-lnk2005-already-defined

//Q: To do: Better use of variadic template???
int  test1(string s, uint x = 0, long y = 0L)
{
	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
	return 1;
}
int test2(string s, uint x = 0, long y = 0L)
{
	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
	return 2;
}
int test3(string s, uint x = 0, long y = 0L)
{
	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
	return 3;
}

template<typename R, typename... Args>
struct testcallback
{
	string c;
	R (*func)(Args... args);
};

testcallback<int, string, uint, long> testCallBack[] =
{
	{ "1", test1 },
	{ "2", test2 },
	{ "3", test3 }
};

//Q: Cannot be inside the TDManager class def? 

//Q: Callback funcs and shared_ptr???
//Q: For Input, to depend on DirectInput8 only????
//Q: Which class should be __declspec(dllexport) ???
//To do: Be a central point? Use Observer?
class __declspec(dllexport) TDManager
{
public:
	typedef struct /*__declspec(dllexport)*/ myEvent
	{
		uint	event;
		HWND	hwnd;
		string	command;
		uint	wparam;
		long	lparam;
	}*PmyEvent;

	template<typename R, typename... Args>
	struct myCommand_Callback
	{
		string command;  //command
		R (*func)(Args... args);
	};

	//Q: Why 1 or other number is necessary here? Why cannot be static?
	/*static*/ TDManager::myCommand_Callback<int, void *, string, TDManager::myEvent> //, uint> 
		Command_Callback[6] =
	{
		// Events: creating 'first-level' windows
		{ "Create Node Win D3D", TDManager::createNodeWinD3D },  //Temp: This NodeWinD3D should be NodeWinD3DGeometry?
		{ "Create Node OP D3D", TDManager::createNodeOPD3D },

		// Events: creating a property window
		{ "D3D Geometry", TDManager::createPropertyWinD3DGeometry },

		//// Events: updating information from property window
		//{ "D3D Geometry update", TDManager::updateFromPropertyWinD3DGeometry }

		// Events: drawing lines
		// Q: Way to pick and move a line???
		{ "Prepare Draw Line", TDManager::prepareDrawLine},
		{ "Draw Line", TDManager::DrawLineFromTo}
													   
	};


public:
	TDManager();
	~TDManager();

	bool createRootWindow();
	RootWindow* getRootWindow()
	{
		return m_RootWindow;
	}

	NodeManager* getNodeManager()
	{
		m_NodeManager = TDSingleton<NodeManager>::getInstance();
		return m_NodeManager;
	}

	bool findFrom(HWND hwnd, HWND* foundStart, int *num)
	{
		list<std::pair<myEvent, myEvent>>::iterator it;
		it = m_Connections.begin();

		for (; it != m_Connections.end(); it++)
		{
			if (it->second.hwnd == hwnd)
			{
				foundStart[*num] = it->first.hwnd;
				*num += 1;
			}
		}
		if (*num >= 1)
			return true;
		return false;
	}

	void Render()
	{
		//list<std::pair<myEvent, myEvent>>::iterator it;
		//it = m_Connections.begin();
		//int rot = 0;

		//if (m_Connections.size() > 0)
		//{
		//	for (; it != m_Connections.end(); it++)
		//	{
		//		for (uint i = 0; i < m_NodeManager->getNodesSize(); i++)
		//		{
		//			//cout << "finding at: " << i << endl;
		//			NodeWin* node = m_NodeManager->findNodeAt(i);
		//			HWND h = node->getNodeWinHandle();
		//			if (h == nullptr)
		//			{
		//				cout << "Node handle is nullptr????" << endl;
		//				continue;
		//			}
		//			else
		//			{
		//				//cout << "h != nullptr " << endl;
		//				if (node->getNodeWinHandle() == it->second.hwnd)
		//				{
		//					NodeWin *nW = m_NodeManager->getObjectByHwnd(it->first.hwnd);
		//					rot = nW->m_Rotation;
		//					//cout << "**********rot : " << rot <<
		//						//"\t" << "typeid name is : " << typeid(nW).name() << 
		//						//endl;
		//					//if (typeid(nW).name() == "NodeWinD3D *")
		//					{
		//						//dynamic_cast<NodeWinD3D *>(nW)->Render(rot);
		//						//nW->Render(rot);
		//					}
		//					//return;
		//				}
		//				else
		//				{
		//					//cout << "Not find" << endl;
		//				}
		//			}
		//		}
		//	}
		//}



		m_NodeManager->Render();
	}

	void sendEvent(myEvent event)
	{
		//Q: this lock has side effect when WM_SIZE in NodeWin is happening?
		//std::lock_guard<mutex> lck(m_QueueMutex); // supposing RAII 

		//cout << "received event: " << event.command.c_str() << endl;

		m_Events.push(event);
		
	}
	void processEvent()
	{
		std::lock_guard<mutex> lck(m_QueueMutex); //Q: What's wrong if using unique_lock?
		//lck.lock();
		myEvent e = m_Events.front(); //Q: if usiing myEvent&, then side effect????
		cout << "event:  " << e.event << " is poped" << endl;
		cout << "e.command is : " << e.command.c_str() << endl;
		m_Events.pop();
		//lck.unlock();

		processEachEvent(e);

		//static int i = 0;
		//std::thread t = std::thread(&TDManager::createNode<Node>, e, i++);

		////static int j = 0;
		////std::thread t2 = std::thread(&TDManager::func, this, ++j);

		//t.join();
		////t2.join();
	}

	uint getEventQueueSize()
	{
		return m_Events.size();
	}

	// to do:add CRUD for Node handling
protected:
	//Q: One class can have one thread? No, each object can.
	// Not each object? Using this as above t2
	// what is return value used? Not known yet

	//Q: To do: registration pattern? refer to : http://stackoverflow.com/questions/1096700/instantiate-class-from-name
	void processEachEvent(const myEvent& e)
	{
		//cout << "---------->Poping up e.command is : " << e.command.c_str() << endl;
		for (int i = 0; i < sizeof(Command_Callback) / sizeof(Command_Callback[0]); i++)
		{
			if (e.command == Command_Callback[i].command)
			{
				int x = (*(Command_Callback[i].func))(this, "This is a test string", e);
			}
		}
	}

	template<class T>
	int createNodeWin(HWND hwnd, const string& title)
	{
		//step1: create and display the node window
		TDFactory<T> *f = new TDFactory<T>();
		T* nW = f->getInstance();

		nW->createWindow(hwnd, title);
		nW->createInputObject();
		nW->createGraphicsObject(); //Q: Temp: for NodeWinD3D it's valid
		nW->displayWindow();  //Q: Should nW display it OR let NodeManager to display?OR let TDManager to display????

  		//step2: add this node window to NodeManager
		m_NodeManager = TDSingleton<NodeManager>::getInstance();
		m_NodeManager->addNode(nW);

		return 1;
	}

	template<class T>
	int createPropertyWin(HWND parentHwnd, HWND sourceNodeHwnd, const string& title)
	{
		//cout << __FUNCTION__ << endl;
		TDFactory<T> *f = new TDFactory<T>();
		T* pW = f->getInstance();

		pW->createWindow(parentHwnd, sourceNodeHwnd, title);
		pW->displayWindow();

		return 1;
	}

	//Q: Temp: Take this NodeWinD3D as NodeWinD3DGeometry for now 
	static int createNodeWinD3D(void *this_Ptr, string s, myEvent e)//, myEvent *e, uint numOfEvents)
	{
		//Q: Jesus, took me so long to find this! 
		//refer to first answer at : http://stackoverflow.com/questions/400257/how-can-i-pass-a-class-member-function-as-a-callback
		TDManager* self = (TDManager *)this_Ptr;

		self->createNodeWin<NodeWinD3D>(e.hwnd, "NodeWinD3DGeometry");
		return 1;
	}

	static int createNodeOPD3D(void * this_Ptr, string s, myEvent e)//, myEvent *e, uint numOfEvents)
	{
		TDManager* self = (TDManager *)this_Ptr;

		self->createNodeWin<NodeOPD3D>(e.hwnd, "NodeOPD3D");

		return 1;
	}
	
	static int createPropertyWinD3DGeometry(void * this_Ptr, string s, myEvent e)//, myEvent *e, uint numOfEvents)
	{
		TDManager* self = (TDManager *)this_Ptr;
		//Q: Why rW is nullptr below????
		//RootWindow* rW = self->getRootWindow();
		//if (rW == nullptr)
		//{
		//	cout << "What? RootWindow pointer null????" << endl;
		//	return -1;
		//}
		//HWND parentHwnd = rW->getHwnd();
		//Q: Temp walkaround....
		HWND parentHwnd = GetParent(e.hwnd);

		self->createPropertyWin<PropertyWinD3DOPGeometry>(parentHwnd, e.hwnd, "D3DGeometry");

		return 1;
	}

	//static int updateFromPropertyWinD3DGeometry(void * this_Ptr, string s, myEvent e)//, myEvent *e, uint numOfEvents)
	//{
	//	TDManager* self = (TDManager *)this_Ptr;

	//	return 1;
	//}
	
	static int prepareDrawLine(void * this_Ptr, string s, myEvent e)
	{
		TDManager* self = (TDManager *)this_Ptr;
		self->m_PrevEvent = e;

		return 1;
	}

	static int DrawLineFromTo(void * this_Ptr, string s, myEvent e)
	{
		TDManager* self = (TDManager *)this_Ptr;

		//Q: Temp
		if (self->m_PrevEvent.hwnd == e.hwnd)
		{
			cout << "Why same hwnd goes here???" << endl;
			//return -1;
		}

		if (!self->existFromTo(self->m_PrevEvent,e))
		{
			if (self->m_PrevEvent.hwnd != e.hwnd)  //REally temp here???
			{
				cout << "Adding to pair for: " << self->m_PrevEvent.hwnd <<
					"and : " << e.hwnd;
				//Save necessary information to m_Connections
				std::pair<myEvent, myEvent> myPair;
				myPair = std::make_pair(self->m_PrevEvent, e);
				self->m_Connections.push_back(myPair);
			}
		}
		self->DrawConnections();

		//self->m_PrevEvent = *(myEvent *)nullptr;

		return 1;
	}

	bool existFromTo(myEvent prevEvent, myEvent e)
	{
		list<pair<myEvent, myEvent>>::iterator it;
		for (it = m_Connections.begin(); it != m_Connections.end(); it++)
		{
			if (it->first.hwnd == prevEvent.hwnd && it->second.hwnd == e.hwnd)
				return true;
		}
		return false;
	}

	int DrawConnections()
	{
		list<std::pair<myEvent, myEvent>>::iterator it;
		//cout << "*********** size of m_Connections: " << m_Connections.size() << endl;
		for (it = m_Connections.begin(); it != m_Connections.end(); it++)
		{
			std::pair<myEvent, myEvent> myPair;
			myPair = *it;

			DrawLine(myPair.first, myPair.second);
		}

		return 1;
	}

	int DrawLine(myEvent prevEvent, myEvent curEvent)
	{
		DrawLine(prevEvent.hwnd, curEvent.hwnd);

		return 1;
	}

	int DrawLine(HWND From, HWND To)
	{
		RECT startRECT;
		RECT endRECT;
		HDC hdc;
		HGDIOBJ original = NULL;

		//cout << "From HWND: " << From << endl;
		//cout << "To HWND: " << To << endl;

		if (To == From)
		{
			cout << "Waring: same from and to!!!!!" << endl;
			return -1;
		}

		if (To != nullptr)
		{
			static POINT prevFrom, prevTo;

			HWND parentHwnd = GetParent(To);
			RECT parentClientRect;
			GetClientRect(parentHwnd, &parentClientRect);
			//InvalidateRect(parentHwnd, &parentClientRect, false);
			//ShowWindow(parentHwnd, SW_SHOW);
			//RedrawWindow(parentHwnd, &parentClientRect, NULL, RDW_UPDATENOW); // RDW_INVALIDATE); // | RDW_ERASE);

			hdc = GetDC(GetParent(To));

			//SelectObject(hdc, GetStockObject(DC_PEN));
			//SetDCPenColor(hdc, RGB(255, 0, 0));

			GetWindowRect(From, &startRECT);
			cout << " startRECT: " << startRECT.right <<
				": " << startRECT.top << endl;
			//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
			MapWindowPoints(HWND_DESKTOP, GetParent(From), (LPPOINT)&startRECT, 2);
			GetWindowRect(To, &endRECT);
			cout << "                end RECT: " << endRECT.left <<
				": " << endRECT.bottom << endl;
			MapWindowPoints(HWND_DESKTOP, GetParent(To), (LPPOINT)&endRECT, 2);

			////HPQ: refer to http://winapi.freetechsecrets.com/win32/WIN32Drawing_Lines_with_the_Mouse.htm
			SetROP2(hdc, R2_NOTXORPEN);
			
			POINT start;
			POINT end;
			start.x = startRECT.right;
			start.y = startRECT.top + (startRECT.bottom - startRECT.top) / 2;
			end.x = endRECT.left;
			end.y = endRECT.bottom - (endRECT.bottom - endRECT.top) / 2;

			//cout << "Start: " << start.x << ": " << start.y << endl;
			//cout << "End: " << end.x << ": " << end.y << endl;

			MoveToEx(hdc, prevFrom.x, prevFrom.y,
				(LPPOINT)NULL);
			LineTo(hdc, prevTo.x, prevTo.y);

			//SelectObject(hdc, GetStockObject(DC_PEN));
			//SetDCPenColor(hdc, RGB(255, 0, 0));

			MoveToEx(hdc, start.x, start.y, NULL);
			LineTo(hdc, end.x, end.y);

			prevFrom.x = start.x;
			prevFrom.y = start.y;
			prevTo.x = end.x;
			prevTo.y = end.y;
		}
		return 1;
	}

private:
	NodeManager*	m_NodeManager;
	ContentManager* m_ContentManager;
	map<INode*, IContent*> m_Node_Contents; //One to One only?
	mutex	m_Mutex;
	condition_variable m_CondVar;

	list<std::pair<myEvent, myEvent>> m_Connections; //Q: Also serve as a mediator???
	mutex	m_MutexConnection;
	myEvent		m_PrevEvent;

	queue<myEvent>  m_Events;
	mutex	m_QueueMutex;
	//condition_variable m_QueueCondVar;

	RootWindow*		m_RootWindow;
	//Q: InputClass: one total or each for a nodewin?
	//InputClass*		m_Input;   //Temp: use DirectInput8 for now, this one for root window
};