#include "TDSingleton.h"
#include "TDFactory.h"
#include "TDManager.h"
#include "rootWindow.h"

#include "NodeWin.h"
#include "NodeOPD3D.h"
#include "PropertyWin.h"

TDManager::TDManager()
	:m_NodeManager(nullptr), m_ContentManager(nullptr)
	, m_RootWindow(nullptr)
{
	m_Node_Contents.clear();
	m_NodeManager = TDSingleton<NodeManager>::getInstance();
	m_ContentManager = TDSingleton<ContentManager>::getInstance();
}

TDManager::~TDManager()
{
	cout << "calling " << __FUNCTION__ << endl;
	if (m_NodeManager)
	{
		delete m_NodeManager;
		m_NodeManager = nullptr;
	}
	if (m_ContentManager)
	{
		delete m_ContentManager;
		m_ContentManager = nullptr;
	}

	for (map<INode*, IContent*>::iterator it = m_Node_Contents.begin();
		it != m_Node_Contents.end(); ++it)
	{  //HPQ: To do later
		////if ((*it).second.size() > 0)
		//{
		//	for (IContent* l : it->second)
		//		if (l)
		//		{
		//			delete l;
		//		}
		//	(*it).second.clear();
		//	it++; //Q: Necessary? 
		//}
		//else
		//	it++;
	}
	m_Node_Contents.clear();

	if (m_RootWindow)
	{
		delete m_RootWindow;
		m_RootWindow = nullptr;
	}
}

bool TDManager::createRootWindow()
{
	m_RootWindow = new RootWindow(); //to do: use factory pattern

	return TRUE;
}

RootWindow* TDManager::getRootWindow()
{
	return m_RootWindow;
}

//Q: By luck, this function is not called yet, otherwise, needs to put into .h?
template<class T>
int TDManager::createNodeWin(HWND hwnd, const string& title)
{
	//step1: create and display the node window
	TDFactory<T> *f = new TDFactory<T>();
	T* nW = f->getInstance();

	nW->createWindow(hwnd, title);
	nW->createInputObject();
	nW->createGraphicsObject(); //Q: Temp: for NodeWinD3D it's valid
	nW->displayWindow();  //Q: Should nW display it OR let NodeManager to display?OR let TDManager to display????

						  //step2: add this node window to NodeManager
	m_NodeManager = TDSingleton<NodeManager>::getInstance();
	m_NodeManager->addNode(nW);

	return 1;
}

//Q: Temp: Take this NodeWinD3D as NodeWinD3DGeometry for now 
/*static */int TDManager::createNodeWinD3D(void *this_Ptr, string s, myEvent e)
{
	//Q: Jesus, took me so long to find this! 
	//refer to first answer at : http://stackoverflow.com/questions/400257/how-can-i-pass-a-class-member-function-as-a-callback
	TDManager* self = (TDManager *)this_Ptr;

	self->createNodeWin<NodeWinD3D>(e.hwnd, "NodeWinD3DGeometry");
	return 1;
}

/*static */int TDManager::createNodeOPD3D(void * this_Ptr, string s, myEvent e)//, myEvent *e, uint numOfEvents)
{
	TDManager* self = (TDManager *)this_Ptr;

	self->createNodeWin<NodeOPD3D>(e.hwnd, "NodeOPD3D");

	return 1;
}

bool TDManager::findFrom(HWND hwnd, HWND* foundStart, int *num)
{
	list<myConnLine *>::iterator it;
	it = m_Connections.begin();

	for (; it != m_Connections.end(); it++)
	{
		if ((*it)->myLine.second.hwnd == hwnd)
		{
			foundStart[*num] = (*it)->myLine.first.hwnd;
			*num += 1;
		}
	}
	if (*num >= 1)
		return true;
	return false;
}

bool TDManager::findTo(HWND hwnd, HWND* foundStart, int *num)
{
	list<myConnLine *>::iterator it;

	it = m_Connections.begin();

	for (; it != m_Connections.end(); it++)
	{
		if ((*it)->myLine.first.hwnd == hwnd)
		{
			foundStart[*num] = (*it)->myLine.second.hwnd;
			*num += 1;
		}
	}
	if (*num >= 1)
		return true;
	return false;
}

void TDManager::Render()
{
	m_NodeManager->Render();
}

NodeManager* TDManager::getNodeManager()
{
	m_NodeManager = TDSingleton<NodeManager>::getInstance();
	return m_NodeManager;
}

void TDManager::sendEvent(myEvent event)
{
	//Q: this lock has side effect when WM_SIZE in NodeWin is happening?
	//std::lock_guard<mutex> lck(m_QueueMutex); // supposing RAII 

	//cout << "received event: " << event.command.c_str() << endl;

	m_Events.push(event);

}
void TDManager::processEvent()
{
	std::lock_guard<mutex> lck(m_QueueMutex); //Q: What's wrong if using unique_lock?
											  //lck.lock();
	myEvent e = m_Events.front(); //Q: if usiing myEvent&, then side effect????
	cout << "event:  " << e.event << " is poped" << endl;
	cout << "e.command is : " << e.command.c_str() << endl;
	m_Events.pop();
	//lck.unlock();

	processEachEvent(e);

	//static int i = 0;
	//std::thread t = std::thread(&TDManager::createNode<Node>, e, i++);

	////static int j = 0;
	////std::thread t2 = std::thread(&TDManager::func, this, ++j);

	//t.join();
	////t2.join();
}

uint TDManager::getEventQueueSize()
{
	return m_Events.size();
}

bool TDManager::isHittingConnLine(long x, long y, int *num)
{
	list<myConnLine *>::iterator it;

	for (it = m_Connections.begin(); it != m_Connections.end(); it++)
	{
		long startX = *((*it)->myLine.first.x);
		long startY = *((*it)->myLine.first.y);
		long endX = *((*it)->myLine.second.x);
		long endY = *((*it)->myLine.second.y);

		if (x < min(startX, endX) || x > max(startX, endX)
			|| y < min(startY, endY) || y > max(startY, endY))
		{
			continue;
		}

		float k1 = (float)(y - startY) / (float)(x - startX);
		float k2 = (float)(endY - startY) / (float)(endX - startX);
		if (((k1 - k2) < 0.1f) && ((k1 - k2) > -0.1f))
		{
			(*it)->isSelected = true;

			(*num)++;
		}
	}

	if (*num > 0)
		return true;
	return false;
}

int TDManager::DrawConnections()
{
	list<myConnLine *>::iterator it;

	cout << "Connection number: " << m_Connections.size() << endl;
	if (m_Connections.size() == 0)
	{
		return 0;
	}

	for (it = m_Connections.begin(); it != m_Connections.end(); it++)
	{
		DrawLine((*it)->myLine.first, (*it)->myLine.second, (*it)->isSelected, false);
	}

	return 1;
}

int TDManager::deleteSelectedLines()
{
	list<myConnLine *>::iterator it;

	for (it = m_Connections.begin(); it != m_Connections.end(); )//it++)
	{
		cout << "(*it)->isSelected" << (*it)->isSelected << endl;

		if ((*it)->isSelected == true)
		{
			delete (*it)->myLine.first.x;
			delete (*it)->myLine.first.y;
			delete (*it)->myLine.second.x;
			delete (*it)->myLine.second.y;
			m_Connections.erase(it++);

			cout <<  "*************changing size : " << m_Connections.size() << endl;

			//it = m_Connections.erase(it); //to test out. 
		}
		else
		{
			it++;
		}
	}

	cout << __FUNCTION__ << "size : " << m_Connections.size() << endl;

	return 1;

}

int TDManager::removeFromTo(HWND hwnd)
{
	list<myConnLine *>::iterator it;
	HWND *foundStart = new HWND[5]; // Temp: 5
	HWND *foundEnd = new HWND[5];
	int numFrom = 0;
	int numTo = 0;

	for (it = m_Connections.begin(); it != m_Connections.end(); it++)
	{
		numFrom = 0;
		if (findFrom(hwnd, foundStart, &numFrom) == true)
		{
			for (int i = 0; i < numFrom; i++)
			{
				//if (hwnd == foundStart[i])
					(*it)->isSelected = true; //To do: Logic error here
			}
		}
		numTo = 0;
		if (findTo(hwnd, foundEnd, &numTo) == true)
		{
			for (int i = 0; i < numTo; i++)
			{
				//if (hwnd == foundEnd[i])
					(*it)->isSelected = true; //To do: Logic error here
			}
		}
	}

	cout << "In loop with numFrom= " << numFrom << "\t numTo= " << numTo << endl;
	deleteSelectedLines();

	return 1;
}

//Q: To do: registration pattern? refer to : http://stackoverflow.com/questions/1096700/instantiate-class-from-name
void TDManager::processEachEvent(const myEvent& e)
{
	//cout << "---------->Poping up e.command is : " << e.command.c_str() << endl;
	for (int i = 0; i < sizeof(Command_Callback) / sizeof(Command_Callback[0]); i++)
	{
		if (e.command == Command_Callback[i].command)
		{
			int x = (*(Command_Callback[i].func))(this, "This is a test string", e);
		}
	}
}

/*static*/ int TDManager::createPropertyWinD3DGeometry(void * this_Ptr, string s, myEvent e)//, myEvent *e, uint numOfEvents)
{
	TDManager* self = (TDManager *)this_Ptr;
	//Q: Why rW is nullptr below????
	//RootWindow* rW = self->getRootWindow();
	//if (rW == nullptr)
	//{
	//	cout << "What? RootWindow pointer null????" << endl;
	//	return -1;
	//}
	//HWND parentHwnd = rW->getHwnd();
	//Q: Temp walkaround....
	HWND parentHwnd = GetParent(e.hwnd);

	self->createPropertyWin<PropertyWinD3DOPGeometry>(parentHwnd, e.hwnd, "D3DGeometry");

	return 1;
}

/*static*/ int TDManager::nodeWinMove(void * this_Ptr, string s, myEvent e)
{
	TDManager* self = (TDManager *)this_Ptr;

	self->moveNodeWin<NodeWin>(e);

	return 1;
}

/*static*/ int TDManager::prepareDrawLine(void * this_Ptr, string s, myEvent e)
{
	TDManager* self = (TDManager *)this_Ptr;
	self->m_PrevEvent = e;

	return 1;
}

/*static*/ int TDManager::DrawLineFromTo(void * this_Ptr, string s, myEvent e)
{
	TDManager* self = (TDManager *)this_Ptr;

	//Q: Temp
	if (self->m_PrevEvent.hwnd == e.hwnd)
	{
		cout << "Why same hwnd goes here???" << endl;
		//return -1;
	}

	if (!self->existFromTo(self->m_PrevEvent, e))
	{
		if (self->m_PrevEvent.hwnd != nullptr)
		{
			if (self->m_PrevEvent.hwnd != e.hwnd)  //REally temp here???
			{
				cout << "Adding to pair for: " << self->m_PrevEvent.hwnd <<
					"and : " << e.hwnd;
				//Save necessary information to m_Connections
				//std::pair<myEvent, myEvent> myPair;
				std::pair<myConnPoint, myConnPoint> myPair;
				myConnPoint prev;
				myConnPoint cur;
				prev.hwnd = self->m_PrevEvent.hwnd;
				prev.x = new long();
				prev.y = new long();
				cur.hwnd = e.hwnd;
				cur.x = new long();
				cur.y = new long();
				myConnLine *oneLine = new myConnLine(); //To do: use factory pattern
				oneLine->myLine.first = prev;
				oneLine->myLine.second = cur;
				oneLine->isSelected = false;
				self->m_Connections.push_back(oneLine);
			}
		}
		else
		{
			cout << "m_PrevEvent.hwnd is nullptr" << endl;
		}
	}
	self->DrawConnections();

	//self->m_PrevEvent = *(myEvent *)nullptr;
	self->m_PrevEvent.hwnd = nullptr;  //Temp


	return 1;
}

bool TDManager::existFromTo(myEvent prevEvent, myEvent e)
{
	list<myConnLine *>::iterator it;

	for (it = m_Connections.begin(); it != m_Connections.end(); it++)
	{
		if ((*it)->myLine.first.hwnd == prevEvent.hwnd && (*it)->myLine.second.hwnd == e.hwnd)
			return true;
	}
	return false;
}

int TDManager::DrawLine(myConnPoint prev, myConnPoint cur, bool isSelected, bool movedFlag)
{
	RECT startRECT;
	RECT endRECT;
	HDC hdc;
	HGDIOBJ original = NULL;

	HWND From = prev.hwnd;
	HWND To = cur.hwnd;

	if (To == From)
	{
		cout << "Waring: same from and to!!!!!" << endl;
		return -1;
	}

	if (To != nullptr)
	{
		HWND parentHwnd = GetParent(To);
		RECT parentClientRect;
		GetClientRect(parentHwnd, &parentClientRect);
		//InvalidateRect(parentHwnd, &parentClientRect, false);
		//ShowWindow(parentHwnd, SW_SHOW);
		//RedrawWindow(parentHwnd, &parentClientRect, NULL, RDW_UPDATENOW); // RDW_INVALIDATE); // | RDW_ERASE);

		hdc = GetDC(GetParent(To));

		SelectObject(hdc, GetStockObject(DC_PEN));
		if (!isSelected)
			SetDCPenColor(hdc, RGB(0, 0, 0));  //black color
		else
			SetDCPenColor(hdc, RGB(255, 0, 0));  //red color


		GetWindowRect(From, &startRECT);
		//cout << " startRECT: " << startRECT.right <<
		//	": " << startRECT.top << endl;
		//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
		MapWindowPoints(HWND_DESKTOP, GetParent(From), (LPPOINT)&startRECT, 2);
		GetWindowRect(To, &endRECT);
		//cout << "                end RECT: " << endRECT.left <<
		//	": " << endRECT.bottom << endl;
		MapWindowPoints(HWND_DESKTOP, GetParent(To), (LPPOINT)&endRECT, 2);

		////HPQ: refer to http://winapi.freetechsecrets.com/win32/WIN32Drawing_Lines_with_the_Mouse.htm
		//SetROP2(hdc, R2_NOTXORPEN);

		POINT start;
		POINT end;
		start.x = startRECT.right;
		start.y = startRECT.top + (startRECT.bottom - startRECT.top) / 2;
		end.x = endRECT.left;
		end.y = endRECT.bottom - (endRECT.bottom - endRECT.top) / 2;

		//cout << "Start: " << start.x << ": " << start.y << endl;
		//cout << "End: " << end.x << ": " << end.y << endl;

		//MoveToEx(hdc, prevFrom.x, prevFrom.y,
		//	(LPPOINT)NULL);
		//LineTo(hdc, prevTo.x, prevTo.y);

		//SelectObject(hdc, GetStockObject(DC_PEN));
		//SetDCPenColor(hdc, RGB(255, 0, 0));

		MoveToEx(hdc, start.x, start.y, NULL);
		LineTo(hdc, end.x, end.y);

		*prev.x = start.x;
		*prev.y = start.y;
		*cur.x = end.x;
		*cur.y = end.y;
	}
	return 1;
}

int TDManager::DrawLine(myConnPoint prev, myConnPoint cur, bool moveFlag)
{
	RECT startRECT;
	RECT endRECT;
	HDC hdc;
	HGDIOBJ original = NULL;

	//cout << "From HWND: " << From << endl;
	//cout << "To HWND: " << To << endl;
	HWND From = prev.hwnd;
	HWND To = cur.hwnd;

	if (To == From)
	{
		cout << "Waring: same from and to!!!!!" << endl;
		return -1;
	}

	if (To != nullptr)
	{
		//static POINT prevFrom, prevTo;

		HWND parentHwnd = GetParent(To);
		RECT parentClientRect;
		GetClientRect(parentHwnd, &parentClientRect);
		//InvalidateRect(parentHwnd, &parentClientRect, false);
		//ShowWindow(parentHwnd, SW_SHOW);
		//RedrawWindow(parentHwnd, &parentClientRect, NULL, RDW_UPDATENOW); // RDW_INVALIDATE); // | RDW_ERASE);

		hdc = GetDC(GetParent(To));

		//SelectObject(hdc, GetStockObject(DC_PEN));
		//SetDCPenColor(hdc, RGB(255, 0, 0));

		GetWindowRect(From, &startRECT);
		//cout << " startRECT: " << startRECT.right <<
		//	": " << startRECT.top << endl;
		//HPQ: refer to http://stackoverflow.com/questions/18034975/how-do-i-find-position-of-a-win32-control-window-relative-to-its-parent-window
		MapWindowPoints(HWND_DESKTOP, GetParent(From), (LPPOINT)&startRECT, 2);
		GetWindowRect(To, &endRECT);
		//cout << "                end RECT: " << endRECT.left <<
		//	": " << endRECT.bottom << endl;
		MapWindowPoints(HWND_DESKTOP, GetParent(To), (LPPOINT)&endRECT, 2);

		////HPQ: refer to http://winapi.freetechsecrets.com/win32/WIN32Drawing_Lines_with_the_Mouse.htm
		SetROP2(hdc, R2_NOTXORPEN);

		POINT start;
		POINT end;
		start.x = startRECT.right;
		start.y = startRECT.top + (startRECT.bottom - startRECT.top) / 2;
		end.x = endRECT.left;
		end.y = endRECT.bottom - (endRECT.bottom - endRECT.top) / 2;

		//cout << "Start: " << start.x << ": " << start.y << endl;
		//cout << "End: " << end.x << ": " << end.y << endl;


		MoveToEx(hdc, *prev.x, *prev.y,
			(LPPOINT)NULL);
		LineTo(hdc, *cur.x, *cur.y);

		//SelectObject(hdc, GetStockObject(DC_PEN));
		//SetDCPenColor(hdc, RGB(255, 0, 0));

		MoveToEx(hdc, start.x, start.y, NULL);
		LineTo(hdc, end.x, end.y);

		*prev.x = start.x;
		*prev.y = start.y;
		*cur.x = end.x;
		*cur.y = end.y;
	}
	return 1;
}