#pragma once
#include "Node.h"
#include "Content.h"
#include "rootWindow.h"

class NodeManager
{
public:
	// to do : add CRUD for Node handling

protected:
	bool isConnectable(INode* nodeFrom, INode* nodeTo)
	{
		return true;
	}

	bool detectNodeLoop()
	{

	}
private:
	list<INode *>*	m_Nodes;
	mutex	m_Mutex;
	condition_variable m_CondVar;

	//To do: Should process using graph???
};

class ContentManager
{
public:
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
	TDManager();
	~TDManager();

	bool createRootWindow();

	// to do:add CRUD for Node handling


private:
	NodeManager*	m_NodeManager;
	ContentManager* m_ContentManager;
	map<INode*, list<IContent*>> m_Node_Contents; //One to One only?
	mutex	m_Mutex;
	condition_variable m_CondVar;

	RootWindow*		m_RootWindow;
	InputClass*		m_Input;   //Temp: use DirectInput8 for now, this one for root window

};

