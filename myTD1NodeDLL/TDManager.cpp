#include "TDSingleton.h"
#include "TDFactory.h"
#include "TDManager.h"
#include "rootWindow.h"

#include "Node.h"
#include "NodeWin.h"
#include "PropertyWin.h"

#include "Content.h"
#include "D3DBase.h"
#include "ContentD3D.h"
#include "ContentD3DDefaultScene.h"
#include "ContentD3DOPGeometry.h"

//Q: Why putting this to .h will cause issue??
myCommand_Callback<int, string, myEvent>
Command_Callback[] =
{
	{ "New Node Win", &createNewNodeWin },

	// Events: creating a property window
	{ "D3D Scene", &loadD3DScene },
	{ "D3D Geometry", &loadD3DOPGeometry },
};

TDManager::TDManager()
	:m_NodeManager(nullptr)
	, m_RootWindow(nullptr)
{
	m_NodeManager = TDSingleton<NodeManager>::getInstance();
}

TDManager::~TDManager()
{
	if (m_NodeManager)
	{
		delete m_NodeManager;
		m_NodeManager = nullptr;
	}
	
	if (m_RootWindow)
	{
		delete m_RootWindow;
		m_RootWindow = nullptr;
	}
}

bool TDManager::createRootWindow()
{
	m_RootWindow = TDSingleton<RootWindow>::getInstance();

	return true;
}

RootWindow* TDManager::getRootWindow()
{
	return m_RootWindow;
}

int TDManager::createNewNodeWin(HWND hwnd, const string& title)
{
	//step1: create and display the node window
	Node *n = new Node();
	NodeWin* nW = new NodeWin();
	//Note: Content should be decided later

	nW->createWindow(hwnd, title);
	nW->displayWindow();  
	
	n->setNodeWin(nW);
	
	//step2: add this node window to NodeManager
	m_NodeManager = TDSingleton<NodeManager>::getInstance();
	m_NodeManager->addNode(n);

	return 1;
}

template<class T>
int TDManager::loadContent(HWND hwnd, const string& title)
{
	//step1: find the suitable node window
	Node *n = m_NodeManager->findNode(hwnd);
	if (n == nullptr)
	{
		cout << "No Node found!!!!" << endl;
		return -1;
	}
	
	//step2: set content
//	Content *content = new T();
	Content *content = new ContentD3D(new T(), hwnd);
//	content->createInputObject();
//	content->createGraphicsObject(hwnd);

	n->setContent(content);

	return 1;
}

int createNewNodeWin(string s, myEvent e)
{
	TDManager* self = TDSingleton<TDManager>::getInstance();
	
	self->createNewNodeWin(e.hwnd, "NodeWin"); 
	return 1;
}

int loadD3DScene(string s, myEvent e)
{
	TDManager* self = TDSingleton<TDManager>::getInstance();

	self->loadContent<ContentD3DDefaultScene>(e.hwnd, "Default D3D Scene");
	//self->loadContent<ContentD3D>(e.hwnd, "Default D3D Scene");

	return 1;
}

int loadD3DOPGeometry(string s, myEvent e)
{

	////step1: load the OP scene
	TDManager* self = TDSingleton<TDManager>::getInstance();

	self->loadContent<ContentD3DOPGeometry>(e.hwnd, "Default D3D Geometry");
	//self->loadContent<ContentD3D>(e.hwnd, "Default D3D Geometry");
	//step2: create property window
	
	return 1;
}

void TDManager::Update()
{
	m_NodeManager->Update();
}

void TDManager::Render()
{
	m_NodeManager->Render();
}

NodeManager* TDManager::getNodeManager()
{
	m_NodeManager = TDSingleton<NodeManager>::getInstance();

	return m_NodeManager;
}

void TDManager::sendEvent(myEvent event)
{
	//Q: this lock has side effect when WM_SIZE in NodeWin is happening?
	//std::lock_guard<mutex> lck(m_QueueMutex); // supposing RAII 

	//cout << "received event: " << event.command.c_str() << endl;

	m_Events.push(event);

}
void TDManager::processEvent()
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

uint TDManager::getEventQueueSize()
{
	return m_Events.size();
}

//Q: To do: registration pattern? refer to : http://stackoverflow.com/questions/1096700/instantiate-class-from-name
void TDManager::processEachEvent(const myEvent& e)
{
	for (int i = 0; i < sizeof(Command_Callback) / sizeof(Command_Callback[0]); i++)
	{
		if (e.command == Command_Callback[i].command)
		{
			int x = (*(Command_Callback[i].func))("test string", e);
		}
	}
}