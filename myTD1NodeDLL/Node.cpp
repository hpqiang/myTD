#include "Node.h"

Node::Node()
{
	m_NodeWin = nullptr;
	m_Content = nullptr;
	m_NodesIn = {};
	m_NodesOut = {};
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
	m_NodesIn.push_back(node);
	return true;
}

bool Node::addNodeOut(Node *node)
{
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

int Node::drawConnection(bool isBiDir, bool isErase)
{
	list<Node *>::iterator it;

	for (it = m_NodesIn.begin(); it != m_NodesIn.end(); it++)
	{
		drawLine((*it)->getNodeWin()->getNodeWinHandle(), m_NodeWin->getNodeWinHandle(), isErase);
	}
	if (isBiDir)
	{
		for (it = m_NodesOut.begin(); it != m_NodesOut.end(); it++)
		{
			drawLine(m_NodeWin->getNodeWinHandle(), (*it)->getNodeWin()->getNodeWinHandle(), isErase);

		}
	}
	return 1;
}

int Node::drawLine(HWND from, HWND to, bool isErase)
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
		RECT parentClientRect;
		GetClientRect(parentHwnd, &parentClientRect);

		//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
		GetWindowRect(from, &startRECT);
		MapWindowPoints(HWND_DESKTOP, GetParent(from), (LPPOINT)&startRECT, 2);
		GetWindowRect(to, &endRECT);
		MapWindowPoints(HWND_DESKTOP, GetParent(to), (LPPOINT)&endRECT, 2);

		POINT start;
		POINT end;
		start.x = startRECT.right;
		start.y = startRECT.top + (startRECT.bottom - startRECT.top) / 2;
		end.x = endRECT.left;
		end.y = endRECT.bottom - (endRECT.bottom - endRECT.top) / 2;

		////HPQ: refer to http://winapi.freetechsecrets.com/win32/WIN32Drawing_Lines_with_the_Mouse.htm
		hdc = GetDC(parentHwnd);
		if (isErase)
		{
			SetROP2(hdc, R2_NOTXORPEN);
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
