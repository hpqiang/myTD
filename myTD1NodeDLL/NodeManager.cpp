#include "NodeManager.h"

NodeManager::NodeManager()
{
	m_Nodes.clear();  //Q: Why doing this here?
}
NodeManager::~NodeManager()
{
	// to do: more processing
	m_Nodes.clear();
}

// to do : add CRUD for Node handling

void NodeManager::addNode(INode* node)
{
	m_Nodes.push_back(node);
}

void NodeManager::removeNode(HWND hwnd)
{
	list<INode *>::iterator it;

	for (it = m_Nodes.begin(); it != m_Nodes.end(); ) //; it++)
	{
		NodeWin* node = dynamic_cast<NodeWin *>(*it);
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

NodeWin* NodeManager::findNodeAt(int idx)
{
	list<INode *>::iterator it;
	int i = 0;

	for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
	{
		if (i == idx)
		{
			NodeWin* node = dynamic_cast<NodeWin *>(*it);
			return node;
		}
		else
			i++;
	}
	cout << "Did not find the INode* with idx" << endl;
	return nullptr;
}

NodeWin* NodeManager::getObjectByHwnd(HWND hwnd)
{
	list<INode *>::iterator it;
	for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
	{
		NodeWin *node = dynamic_cast<NodeWin *>(*it);
		if (node->getNodeWinHandle() == hwnd)
		{
			return node;
		}
	}
	cout << "Did not find the Inode with hwnd" << endl;
	return nullptr;
}

void NodeManager::Render()
{
	if (m_Nodes.empty())
		return;
	list<INode *>::iterator it;

	for (it = m_Nodes.begin(); it != m_Nodes.end(); it++)
	{
		//To do: eliminate the dynamic_cast
		dynamic_cast<NodeWin *>(*it)->Render();
	}
}

bool NodeManager::isConnectable(INode* nodeFrom, INode* nodeTo)
{
	return true;
}

bool NodeManager::detectNodeLoop()
{
	return true;
}