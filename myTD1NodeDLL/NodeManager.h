#pragma once
#include "Node.h"
#include "NodeWin.h"

class NodeManager
{
public:
	NodeManager();
	~NodeManager();

	// to do : add CRUD for Node handling

	void addNode(INode* node);
	void removeNode(HWND hwnd);
	uint getNodesSize();

	NodeWin* findNodeAt(int idx);

	NodeWin* getObjectByHwnd(HWND hwnd);

	void Render();

protected:
	bool isConnectable(INode* nodeFrom, INode* nodeTo);

	bool detectNodeLoop();  //Q: if putting the function here, no need to return a value????
	//{

	//}
private:
	list<INode *>	m_Nodes;
	mutex	m_Mutex;
	condition_variable m_CondVar;

	NodeManager* m_Composite;  //should support composite pattern?
							   //To do: Should process using graph???
};
