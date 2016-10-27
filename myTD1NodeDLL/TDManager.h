#pragma once
#include <iostream>
using namespace std;

#include "Node.h"
#include "NodeWin.h"
#include "Content.h"
#include "rootWindow.h"
#include "TDSingleton.h"

#include <thread>

class NodeManager
{
public:
	NodeManager() {}
	~NodeManager() {} //Prevent others to call it
					  // to do : add CRUD for Node handling
	void addNode(INode* node)
	{
		m_Nodes.push_back(node);
	}
	uint getNodesSize()
	{
		return m_Nodes.size();
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

	//To do: Should process using graph???
};

class ContentManager
{
public:
	ContentManager() {}
	~ContentManager() {} //Prevent others to call it
						 // to do : add CRUD for Node handling
private:
	list<IContent *>*	m_Nodes;
	mutex		m_Mutex;
	condition_variable m_CondVar;

};

//Q: Callback funcs and shared_ptr???
//Q: For Input, to depend on DirectInput8 only????
//Q: Which class should be __declspec(dllexport) ???
//To do: Be a central point? Use Observer?
class __declspec(dllexport) TDManager
{
public:
	typedef struct /*__declspec(dllexport)*/ myEvent
	{
		uint event;
		HWND hwnd;
		string command;
		uint* wparam;
		long* lparam;
	}*PmyEvent;

	template<typename... Args>
	struct myCommand_Action
	{
		string command;
		void   (*action)(Args... args); //Q: ???
	};
	
	template<typename... Args>
	struct myCallback
	{
		void (*func)(Args...);
	};
	 
	//typedef myCallback<myEvent> *PmyCallback;


public:
	TDManager();
	~TDManager();

	bool createRootWindow();
	RootWindow* getRootWindow()
	{
		return m_RootWindow;
	}

	void sendEvent(myEvent event)
	{
		std::lock_guard<mutex> lck(m_QueueMutex); // supposing RAII 

		//cout << "received event: " << event.command.c_str() << endl;

		m_Events.push(event);
	}
	void processEvent()
	{
		std::lock_guard<mutex> lck(m_QueueMutex); //Q: What's wrong if using unique_lock?
		//lck.lock();
		myEvent e = m_Events.front(); //Q: if usiing myEvent&, then side effect????
		//cout << "event:  " << e.event << " is poped" << endl;
		//cout << "e.command is : " << e.command.c_str() << endl;
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

	////template<class T>
	//void func(int i) //, T* t)
	//{
	//	cout << "i=" << i << endl;
	//}
	// to do:add CRUD for Node handling
protected:
	////Q: One class can have one thread? No
	//// Not each object? Using this as above t2
	//// what is return value used? Not known yet
	//template<class T>
	//static uint createNode(TDManager::myEvent e, int i)
	//{
	//	cout << "***********called with e = " << e.event << " and i= " << i << endl;
	//	
	//	T* t = nullptr;
	//	t = new T();
	//	cout << "T: " << typeid(T).name() << endl;

	//	NodeManager* nM = TDSingleton<NodeManager>::getInstance();
	//	nM->addNode(t);
	//	cout << "size of Nodes: " << nM->getNodesSize() << endl;

	//	//delete nM; //Q: necessary? correct?
	//	//nM = nullptr; 
	//	delete t;
	//	t = nullptr;

	//	return e.event;
	//}

public:
	//Q: To do: registration pattern? refer to : http://stackoverflow.com/questions/1096700/instantiate-class-from-name
	void processEachEvent(const myEvent& e)
	{
		//cout << "---------->Poping up e.command is : " << e.command.c_str() << endl;
		if (e.command == "Create Node Win")
		{
			//step1: create and display the node window
			NodeWin* nW = new NodeWin();

			nW->createWindow(e.hwnd);
			nW->createInputObject();
			nW->displayWindow();  //Q: Should nW display it OR let NodeManager to display?OR let TDManager to display????

			//delete nW;
			//nW = nullptr;

			//step2: add this node window to NodeManager
		}
		else
		{
			cout << "More to do hereafter..." << endl;
		}
	}

private:
	NodeManager*	m_NodeManager;
	ContentManager* m_ContentManager;
	map<INode*, IContent*> m_Node_Contents; //One to One only?
	mutex	m_Mutex;
	condition_variable m_CondVar;

	queue<myEvent>  m_Events;
	mutex	m_QueueMutex;
	//condition_variable m_QueueCondVar;
	//vector<map<PmyEvent, myCallback>> m_Event_Callbacks;
	
	//template<typename T>  //T: should be args...
	//list<myCommand_Action<T>> m_Command_Action; //Q: list or map?

	RootWindow*		m_RootWindow;
	//Q: InputClass: one total or each for a nodewin?
	//InputClass*		m_Input;   //Temp: use DirectInput8 for now, this one for root window

};

