#pragma once

#include "CommonDefs.h"
#include "rootWindow.h"

#include "NodeManager.h"

template<typename R, typename... Args>
struct myCommand_Callback
{
	string command;
	R(*func)(Args... args); 
};

typedef struct myEvent
{
	uint	event;
	HWND	hwnd;
	string	command;
	uint	wparam;
	long	lparam;
}*PmyEvent;

int createNewNodeWin(string s, myEvent e);
int loadD3DScene(string s, myEvent e);
int loadD3DOPGeometry(string s, myEvent e);

//For LNK2005 error, add /FORCE:MULTIPLE in linker option: http://stackoverflow.com/questions/10046485/error-lnk2005-already-defined
//Q: Callback funcs and shared_ptr???
//To do: Be a central point? Use Observer?
class __declspec(dllexport) TDManager
{
public:
	TDManager();
	~TDManager();

	bool createRootWindow();
	RootWindow* getRootWindow();

	NodeManager* getNodeManager();

	void Render();

	void sendEvent(myEvent event);
	void processEvent();
	uint getEventQueueSize();

	//Q: To do: registration pattern? refer to : http://stackoverflow.com/questions/1096700/instantiate-class-from-name
	void processEachEvent(const myEvent& e);


	int createNewNodeWin(HWND hwnd, const string& title);
	template<class T>
	int loadContent(HWND hwnd, const string& title);

private:
	NodeManager*	m_NodeManager;

	myEvent			m_PrevEvent;

	queue<myEvent>  m_Events;
	mutex	m_QueueMutex;
	//condition_variable m_QueueCondVar;

	RootWindow*		m_RootWindow;
};