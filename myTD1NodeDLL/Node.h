#pragma once

#include <vector>
#include <queue>
#include <list>
#include <map>
#include <mutex>
#include <condition_variable>
using namespace std;

#if defined(_WIN32)
#include <windows.h>
//else
#endif

#include "CommonDefs.h"
#include "Content.h"
#include "inputclass.h"

class INode
{
	virtual bool	createNode(INode** node) = 0;
	virtual bool	deleteNode(INode& node) = 0;

	virtual INode&	getNode() const = 0;
	virtual void	setNode(const INode& node) = 0;

	virtual INode&	getNodeIn(const uint idx) const = 0;
	virtual INode&	getNodeIn(const INode& node) const = 0;
	virtual void	setNodeIn(const INode& nodeIn) = 0;

	virtual INode&	getNodeOut(const uint idx) const = 0;
	virtual INode&	getNodeOut(const INode& node) const = 0;
	virtual void	setNodeOut(const INode& nodeIn) = 0;

	virtual bool	ConnectFrom(const INode& nodeIn) = 0;
	virtual bool	ConnectTo(const INode& nodeOut) = 0;
	//Q: Interface needs destructor???
	//virtual ~INode() = 0;
};

class Node : public INode
{
public:
	Node()
		: m_Content( *(IContent *)nullptr ) //??? which constructor is it using?
	{
	}
	//Node(const Node& node) 
	//{
	//	this = node;
	//}
	virtual ~Node(){}

	//Q: have to define these here?
	virtual bool	createNode(INode** node)
	{
		return true;
	}
	virtual bool	deleteNode(INode& node)
	{
		return true;
	}

	virtual INode&	getNode() const
	{
		return *(INode *)nullptr;
	}
	virtual void	setNode(const INode& node)
	{
		return;
	}

	virtual INode&	getNodeIn(const uint idx) const
	{
		return *(INode *)nullptr;
	}
	virtual INode&	getNodeIn(const INode& node) const
	{
		return *(INode *)nullptr;
	}
	virtual void	setNodeIn(const INode& nodeIn)
	{
		return;
	}

	virtual INode&	getNodeOut(const uint idx) const
	{
		return *(INode *)nullptr;
	}
	virtual INode&	getNodeOut(const INode& node) const
	{
		return *(INode *)nullptr;
	}
	virtual void	setNodeOut(const INode& nodeIn)
	{
		return;
	}

	virtual bool	ConnectFrom(const INode& nodeIn)
	{
		return true;
	}
	virtual bool	ConnectTo(const INode& nodeOut)
	{
		return true;
	}


private:
	INode*  m_NodeIn; //Q: Can use INode& here?
	uint	m_NumOfNodeIn;
	INode*	m_NodeOut;
	uint	m_NumOfNodeOut;

	IContent& m_Content;
};

