#pragma once

#include "Content.h"
#include "rootWindow.h"

#include "NodeManager.h"
#include "ContentManager.h"

//For LNK2005 error, add /FORCE:MULTIPLE in linker option: http://stackoverflow.com/questions/10046485/error-lnk2005-already-defined
//Q: Cannot be inside the TDManager class def? 
//Q: Callback funcs and shared_ptr???
//Q: For Input, to depend on DirectInput8 only????
//Q: Which class should be __declspec(dllexport) ???
//To do: Be a central point? Use Observer?
class __declspec(dllexport) TDManager
{
public:
	typedef struct myEvent
	{
		uint	event;
		HWND	hwnd;
		string	command;
		uint	wparam;
		long	lparam;
	}*PmyEvent;

	struct myConnPoint
	{
		HWND hwnd;
		long *x;
		long *y;
	};

	struct myConnLine
	{
		std::pair<myConnPoint, myConnPoint> myLine;
		bool	isSelected;
		//RGB		color; //etc...
		int		style; //etc....
	};

	template<typename R, typename... Args>
	struct myCommand_Callback
	{
		string command;
		R(*func)(Args... args);  //Q: What's this all about? 
	};

	//Q: Why 1 or other number is necessary here? Why cannot be static?
	//Q: Strange... Refer to test() in main.cpp for simple consideration, still strange....
	/*static*/ TDManager::myCommand_Callback<int, void *, string, TDManager::myEvent> 
		Command_Callback[6] =
	{
		// Events: creating 'first-level' windows
		{ "Create Node Win D3D", TDManager::createNodeWinD3D },  //Temp: This NodeWinD3D should be NodeWinD3DGeometry?
		{ "Create Node OP D3D", TDManager::createNodeOPD3D },

		// Events: creating a property window
		{ "D3D Geometry", TDManager::createPropertyWinD3DGeometry },

		// Events: Node window status update
		
		{ "Node Window Moved", TDManager::nodeWinMove },

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
	RootWindow* getRootWindow();

	NodeManager* getNodeManager();

	bool findFrom(HWND hwnd, HWND* foundStart, int *num);
	bool findTo(HWND hwnd, HWND* foundStart, int *num);

	void Render();

	void sendEvent(myEvent event);
	void processEvent();
	uint getEventQueueSize();

	bool isHittingConnLine(long x, long y, int *num);

	int DrawConnections();
	int deleteSelectedLines();

	int removeFromTo(HWND hwnd);
	// to do:add CRUD for Node handling
//protected:
	//Q: One class can have one thread? No, each object can.
	// Not each object? Using this as above t2
	// what is return value used? Not known yet

	////Q: To do: registration pattern? refer to : http://stackoverflow.com/questions/1096700/instantiate-class-from-name
	void processEachEvent(const myEvent& e);

	template<class T>
	int createNodeWin(HWND hwnd, const string& title);

	template<class T>
	int createPropertyWin(HWND parentHwnd, HWND sourceNodeHwnd, const string& title)
	{
		TDFactory<T> *f = new TDFactory<T>();
		T* pW = f->getInstance();

		pW->createWindow(parentHwnd, sourceNodeHwnd, title);
		pW->displayWindow();

		return 1;
	}

	template<class T>
	int moveNodeWin(myEvent e)
	{
		list<myConnLine *>::iterator it;

		for (it = m_Connections.begin(); it != m_Connections.end(); it++)
		{
			if ( (*it)->myLine.first.hwnd == e.hwnd)
			{
				DrawLine( (*it)->myLine.first, (*it)->myLine.second, true);
			}
			if ((*it)->myLine.second.hwnd == e.hwnd)
			{
				DrawLine((*it)->myLine.first, (*it)->myLine.second, true);
			}
		}

		return 1;
	}

	//Q: Temp: Take this NodeWinD3D as NodeWinD3DGeometry for now 
	static int createNodeWinD3D(void *this_Ptr, string s, myEvent e);
	static int createNodeOPD3D(void * this_Ptr, string s, myEvent e);
	static int createPropertyWinD3DGeometry(void * this_Ptr, string s, myEvent e);

	static int nodeWinMove(void * this_Ptr, string s, myEvent e);

	static int prepareDrawLine(void * this_Ptr, string s, myEvent e);
	static int DrawLineFromTo(void * this_Ptr, string s, myEvent e);

	bool existFromTo(myEvent prevEvent, myEvent e);

	int DrawLine(myConnPoint prev, myConnPoint cur, bool isSelected, bool movedFlag);
	int DrawLine(myConnPoint prev, myConnPoint cur, bool moveFlag);

private:
	NodeManager*	m_NodeManager;
	ContentManager* m_ContentManager;
	map<INode*, IContent*> m_Node_Contents; //One to One only?
	mutex	m_Mutex;
	condition_variable m_CondVar;

	list<myConnLine *> m_Connections; //Q: Also serve as a mediator???

	mutex	m_MutexConnection;
	myEvent		m_PrevEvent;

	queue<myEvent>  m_Events;
	mutex	m_QueueMutex;
	//condition_variable m_QueueCondVar;

	RootWindow*		m_RootWindow;
	//Q: InputClass: one total or each for a nodewin?
	//InputClass*		m_Input;   //Temp: use DirectInput8 for now, this one for root window
};