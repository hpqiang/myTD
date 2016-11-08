#include "NodeManager.h"

NodeManager::NodeManager()
{
	m_Nodes.clear();
}
NodeManager::~NodeManager()
{
	// to do: more processing
	m_Nodes.clear();
}

// to do : add CRUD for Node handling
void NodeManager::addNode(Node* node)
{
	m_Nodes.push_back(node);
}

Node* NodeManager::findNode(HWND hwnd)
{
	list<Node *>::iterator it;
	for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
	{
		NodeWin *nW = (*it)->getNodeWin();
		if ( nW->getNodeWinHandle() == hwnd )
		{
			return (*it);
		}
	}
	return nullptr;
}

void NodeManager::removeNode(HWND hwnd)
{
	list<Node *>::iterator it;

	for (it = m_Nodes.begin(); it != m_Nodes.end(); )
	{
		NodeWin* node = (*it)->getNodeWin();
		if (node->getNodeWinHandle() == hwnd)
		{
			//To do: Delete inner pointers
			it = m_Nodes.erase(it);
		}
		else
		{
			it++;
		}
	}
}

uint NodeManager::getNodesSize()
{
	return m_Nodes.size();
}

int NodeManager::drawConnections()
{
	list<Node *>::iterator it;

	for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
	{
		(*it)->drawConnection(false, false);
	}

	return 1;
}

void NodeManager::Render()
{
	if (m_Nodes.empty())
		return;
	list<Node *>::iterator it;

	for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
	{
		Content *content = (*it)->getContent();
		if (content != nullptr)  //Temp
			content->Render(0);
	}
}