#pragma once
#include "Node.h"
#include "NodeWin.h"

class NodeManager
{
public:
	NodeManager();
	~NodeManager();

	void addNode(Node* node);
	Node *findNode(HWND hwnd);
	void removeNode(HWND hwnd);
	uint getNodesSize();

	bool isHittingConnLine(long x, long y);
	int drawConnections(bool isSelected);

	bool deleteDeleteCandidateNodes();

	//To do: Update();

	void Render();

private:
	list<Node *>		m_Nodes;
	mutex				m_Mutex;
	condition_variable	m_CondVar;
};
