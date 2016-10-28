#pragma once
#include <iostream>
using namespace std;

#include "Node.h"
#include "NodeWin.h"
#include "Content.h"
#include "rootWindow.h"
#include "TDSingleton.h"
#include "TDFactory.h"

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

#define MYRESULT uint
//For LNK2005 error, add /FORCE:MULTIPLE in linker option: http://stackoverflow.com/questions/10046485/error-lnk2005-already-defined

//Q: To do: Better use of variadic template???
/*MYRESULT*/int  test1(string s, uint x = 0, long y = 0L)
{
	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
	return 1;
}
/*MYRESULT*/ int test2(string s, uint x = 0, long y = 0L)
{
	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
	return 2;
}
/*MYRESULT*/int test3(string s, uint x = 0, long y = 0L)
{
	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
	return 3;
}

template<typename R, typename... Args>
struct testcallback
{
	string c;
	//void (*func)(int i);
	R (*func)(Args... args);
	//template<typename... Args>
	//MYRESULT (*func)(Args... args);

};

testcallback<int, string, uint, long> testCallBack[] =
//testcallback testCallBack[] = 
{
	{ "1", test1 },
	{ "2", test2 },
	{ "3", test3 }
};



//Q: Cannot be inside the TDManager class def? 


//template<typename... Args>
//struct myCommand_ActionList
//{
//	string command;
//	MYRESULT (*func)(Args... args);
//};
//
//myCommand_ActionList<int> Command_ActionList[] =
//{
//	{"Create Node Win", &(TDManager::test)},
//	{"", TDManager::noCommand}
//};

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

	//typedef myCallback<myEvent> *PmyCallback;

	template<typename R, typename... Args>
	struct myCommand_Callback
	{
		string command;  //command
		R (*func)(Args... args);
	};

	myCommand_Callback<int, void *, string, myEvent> Command_Callback[1] = //Q: Why 1 is necessary here?
	{
		{ "Create Node Win", TDManager::createNodeWin }//this->createNodeWin }
											//{ "", test3 }
	};


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

		////cout << "event processed ..." << endl;
		//for (int i = 0; i < sizeof(testCallBack)/sizeof(testCallBack[0]); i++)
		//{
		//	//cout << testCallBack[i].c.c_str() << endl;
		//	int x = (*(testCallBack[i].func))("This is a test string", e.wparam, e.lparam);  //Note: Mind the notation!!!
		//	//cout << "returning: " << x << endl;
		//}

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

	MYRESULT test(int i)
	{
		cout << "i= " << i << endl;
		return 1;
	}

	MYRESULT noCommand()
	{
		cout << "noCommand " << endl;
		return 1;
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
		for (int i = 0; i < sizeof(Command_Callback) / sizeof(Command_Callback[0]); i++)
		{
			if (e.command == Command_Callback[i].command)
			{
				int x = (*(Command_Callback[i].func))(this, "This is a test string", e);
			}
		}
		//if (e.command == "Create Node Win")
		//{
		//	//step1: create and display the node window
		//	//NodeWin* nW = new NodeWin();
		//	TDFactory<NodeWin> *f = new TDFactory<NodeWin>();
		//	NodeWin *nW = f->getInstance();

		//	nW->createWindow(e.hwnd);
		//	nW->createInputObject();
		//	nW->displayWindow();  //Q: Should nW display it OR let NodeManager to display?OR let TDManager to display????

		//	//delete nW;
		//	//nW = nullptr;

		//	//step2: add this node window to NodeManager
		//}
		//else
		//{
		//	cout << "More to do hereafter..." << endl;
		//}
	}

	static int createNodeWin(void * this_Ptr, string s, myEvent e)
	{
		//Q: Jesus, took me so long to find this! 
		TDManager* self = (TDManager *)this_Ptr;

		cout << __FUNCTION__ << endl;
		//step1: create and display the node window
		//NodeWin* nW = new NodeWin();
		TDFactory<NodeWin> *f = new TDFactory<NodeWin>();
		NodeWin *nW = f->getInstance();

		nW->createWindow(e.hwnd);
		nW->createInputObject();
		nW->displayWindow();  //Q: Should nW display it OR let NodeManager to display?OR let TDManager to display????

		//delete nW;
		//nW = nullptr;

		//step2: add this node window to NodeManager
		self->m_NodeManager = TDSingleton<NodeManager>::getInstance();
		self->m_NodeManager->addNode(nW);
		cout << self->m_NodeManager->getNodesSize() << endl;
		return 1;
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


