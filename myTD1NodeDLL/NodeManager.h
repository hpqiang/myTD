#pragma once
#include "Node.h"
#include "NodeWin.h"

class NodeManager
{
public:
	NodeManager();
	~NodeManager();

	// to do : add CRUD for Node handling

	void addNode(Node* node);
	Node *findNode(HWND hwnd);
	void removeNode(HWND hwnd);
	uint getNodesSize();

	int drawConnections();

	//To do: Update();

	void Render();

private:
	list<Node *>	m_Nodes;
	mutex	m_Mutex;
	condition_variable m_CondVar;
};
