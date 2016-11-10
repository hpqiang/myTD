#include "Node.h"
#include "rootWindow.h"

Node::Node()
{
	m_NodeWin = nullptr;
	m_Content = nullptr;
	m_NodesIn = {};
	m_NodesOut = {};
	m_NodesInDeleteCandidates = {};
	m_NodesOutDeleteCandidates = {};
}

Node::~Node()
{
	if (m_NodeWin != nullptr)
	{
		delete m_NodeWin;
		m_NodeWin = nullptr;
	}
	if (m_Content != nullptr)
	{
		delete m_Content;
		m_Content = nullptr;
	}

	m_NodesIn.clear();
	m_NodesOut.clear();

	m_NodesInDeleteCandidates.clear();
	m_NodesOutDeleteCandidates.clear();

}

NodeWin* Node::getNodeWin()
{
	return m_NodeWin;
}
void Node::setNodeWin(NodeWin* nW)
{
	m_NodeWin = nW;
}

Content* Node::getContent()
{
	return m_Content;
}
void Node::setContent(Content *content)
{
	m_Content = content;
}

bool Node::addNodeIn(Node *node)
{
	list<Node *>::iterator it;
	for (it = m_NodesIn.begin(); it != m_NodesIn.end(); it++)
	{
		if ((*it) == node)
		{
			return true;
		}
	}
	m_NodesIn.push_back(node);
	return true;
}

bool Node::addNodeOut(Node *node)
{
	list<Node *>::iterator it;
	for (it = m_NodesOut.begin(); it != m_NodesOut.end(); it++)
	{
		if ((*it) == node)
		{
			return true;
		}
	}
	m_NodesOut.push_back(node);
	return true;
}

uint Node::getNodesInSize()
{
	return m_NodesIn.size();
}
uint Node::getNodesOutSize()
{
	return m_NodesOut.size();
}

int Node::drawConnection(bool isBiDir, bool isErase, bool isSelected)
{
	list<Node *>::iterator it;

	for (it = m_NodesIn.begin(); it != m_NodesIn.end(); it++)
	{
		drawLine((*it)->getNodeWin()->getNodeWinHandle(), m_NodeWin->getNodeWinHandle(), isErase, isSelected);
	}
	if (isBiDir)
	{
		for (it = m_NodesOut.begin(); it != m_NodesOut.end(); it++)
		{
			drawLine(m_NodeWin->getNodeWinHandle(), (*it)->getNodeWin()->getNodeWinHandle(), isErase, isSelected);
		}
	}
	return 1;
}

bool Node::isHittingConnLine(long x, long y)
{
	list<Node *>::iterator it;
	RECT startRECT;
	RECT endRECT;

	//Only consider one direction now
	for (it = m_NodesIn.begin(); it != m_NodesIn.end(); it++)
	{
		POINT start;
		POINT end;
		HWND to = m_NodeWin->getNodeWinHandle();
		////HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
		HWND from = (*it)->getNodeWin()->getNodeWinHandle();

		RootWindow::getWindowMiddle(from, &start.x, &start.y, true);
		RootWindow::getWindowMiddle(to, &end.x, &end.y, false);

		if (RootWindow::isHittingConnLine(x, y, start.x, start.y,
				end.x, end.y) == true)
		{
			//cout << "Hitting..." << endl;
			drawLine(from, to, false, true);

			list<Node *>::iterator dit;
			for (dit = (*it)->m_NodesOutDeleteCandidates.begin();
				dit != (*it)->m_NodesOutDeleteCandidates.end(); dit++)
			{
				if ((*dit) == (*it))
					return true;
			}
			(*it)->m_NodesOutDeleteCandidates.push_back(this);

			for (dit = m_NodesInDeleteCandidates.begin();
				dit != m_NodesInDeleteCandidates.end(); dit++)
			{
				if ((*dit) == (*it))
					return true;
			}
			m_NodesInDeleteCandidates.push_back(*it);

			return true;
		}
	}
	return false;
}

int Node::drawLine(HWND from, HWND to, bool isErase, bool isSelected)
{
	RECT startRECT;
	RECT endRECT;
	HDC hdc;
	HGDIOBJ original = NULL;

	if (from == nullptr || to == nullptr)
	{
		cout << " either from or to is nullptr!!!!" << endl;
		return -1;
	}

	if (to == from)
	{
		cout << "Waring: same from and to!!!!!" << endl;
		return -1;
	}

	//if (to != nullptr)
	{
		HWND parentHwnd = GetParent(to);

		POINT start;
		POINT end;
		RootWindow::getWindowMiddle(from, &start.x, &start.y, true);
		RootWindow::getWindowMiddle(to, &end.x, &end.y, false);

		////HPQ: refer to http://winapi.freetechsecrets.com/win32/WIN32Drawing_Lines_with_the_Mouse.htm
		hdc = GetDC(parentHwnd);
		if (isErase)
		{
			SetROP2(hdc, R2_NOTXORPEN);
			MoveToEx(hdc, start.x, start.y, NULL);
			LineTo(hdc, end.x, end.y);
		}
		else if (isSelected)
		{
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 0, 0));  //red color To do: only black color works now
			MoveToEx(hdc, start.x, start.y, NULL);
			LineTo(hdc, end.x, end.y);
		}
		else
		{
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(0, 0, 0));  //black color To do: only black color works now
			MoveToEx(hdc, start.x, start.y, NULL);
			LineTo(hdc, end.x, end.y);
		}
	}
	return 1;
}

bool Node::removeNodeInOut()
{
	list<Node *>::iterator it;
	for (it = m_NodesIn.begin(); it != m_NodesIn.end();)
	{
		it = m_NodesIn.erase(it);
	}
	for (it = m_NodesOut.begin(); it != m_NodesOut.end();)
	{
		it = m_NodesOut.erase(it);
	}

	for (it = m_NodesInDeleteCandidates.begin(); it != m_NodesInDeleteCandidates.end();)
	{
		it = m_NodesInDeleteCandidates.erase(it);
	}
	for (it = m_NodesOutDeleteCandidates.begin(); it != m_NodesOutDeleteCandidates.end();)
	{
		it = m_NodesOutDeleteCandidates.erase(it);
	}

	return true;
}

bool Node::deleteDeleteCandidateNodes()
{
	list<Node *>::iterator it;
	for (it = m_NodesInDeleteCandidates.begin(); 
		it != m_NodesInDeleteCandidates.end();)
	{
		list<Node *>::iterator nit;
		for (nit = m_NodesIn.begin(); nit != m_NodesIn.end();)
		{
			if ( *nit == *it)
			{
				nit = m_NodesIn.erase(nit);
			}
			else
			{
				nit++;
			}
		}
		it = m_NodesInDeleteCandidates.erase(it);
	}
	for (it = m_NodesOutDeleteCandidates.begin(); 
		it != m_NodesOutDeleteCandidates.end();)
	{
		list<Node *>::iterator nit;
		for (nit = m_NodesOut.begin(); nit != m_NodesOut.end();)
		{
			if ( *nit == *it)
			{
				nit = m_NodesOut.erase(nit);
			}
			else
			{
				nit++;
			}
		}
		it = m_NodesOutDeleteCandidates.erase(it);
	}
	return true;
}
