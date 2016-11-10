#pragma once

#include "CommonDefs.h"
#include "NodeWin.h"
#include "../myTD1ContentDLL/Content.h"

class Node
{
public:
	Node();
	~Node();

	NodeWin* getNodeWin();
	void setNodeWin(NodeWin* nW);

	Content* getContent();
	void setContent(Content *content);

	bool addNodeIn(Node *node);

	bool addNodeOut(Node *node);

	uint getNodesInSize();
	uint getNodesOutSize();

	int drawConnection(bool isBiDir, bool isErase, bool isSelected);
	int drawLine(HWND from, HWND to, bool isErase, bool isSelected);
	bool isHittingConnLine(long x, long y);

	bool removeNodeInOut();
	bool deleteDeleteCandidateNodes();

private:
	NodeWin			*m_NodeWin;
	Content			*m_Content;

	list<Node *>	m_NodeComposite;  //should support composite pattern?

	list<Node *>	m_NodesIn;
	list<Node *>	m_NodesOut;

	list<Node *>	m_NodesInDeleteCandidates;
	list<Node *>	m_NodesOutDeleteCandidates;
};