// MathClient.cpp : Defines the entry point for the console application.
// Compile by using: cl /EHsc /link MathLibrary.lib MathClient.cpp

#include <iostream>
//#include "myDLL.h"
#include "inputclass.h"
#include "Node.h"
#include "TDManager.h"
#include "TDSingleton.h"
#include "TDFactory.h"

#include "tabWindow.h"

#include <thread>

using namespace std;

void test()
{
	//double a = 7.4;
	//int b = 99;

	//cout << "a + b = " <<
	//	MathLibrary::Functions::Add(a, b) << endl;
	//cout << "a * b = " <<
	//	MathLibrary::Functions::Multiply(a, b) << endl;
	//cout << "a + (a * b) = " <<
	//	MathLibrary::Functions::AddMultiply(a, b) << endl;
}

void testFactory()
{
	TDFactory<Node> *f = new TDFactory<Node>();
	Node* n = f->getInstance();

	delete n;
}

bool testInputClass(HINSTANCE hinst, HWND hwnd)
{
	//InputClass* iC = TDSingleton<InputClass>::getInstance();
	//Node* iC = TDSingleton<Node>::getInstance();
	TDFactory<InputClass> *f = new TDFactory<InputClass>();
	InputClass *iC = f->getInstance();

	cout << typeid(iC).raw_name() << endl;
	cout << typeid(iC).name() << endl;
	cout << typeid(iC).hash_code() << endl;

	RECT rect;
	GetWindowRect(hwnd, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	iC->Initialize(hinst, hwnd, w, h);
	cout << "w: " << w << " " << "h: " << h << endl;
	//iC->Initialize(hinst, hwnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	bool result;
	int x, y;


	// Check if the user pressed escape and wants to exit the application.
	do
	//{
	//	return false;
	//}
	//else
	{
		result = iC->Frame();
		if (!result)
		{
			cout << "Frame error" << endl;
			return false;
		}
		iC->GetMouseLocation(x, y);  //Issue: x, y are not correct
		cout << "x: " << x << "\t" << "y: " << y << endl;
	} while (iC->IsEscapePressed() != true);
	//Input has to be with hinstance and hwnd??? yes.
}

int testTabWindow()
{
	TabWindow* tabWindow = new TabWindow();

	tabWindow->displayWindow();
	tabWindow->run();

	return 0;
}


int main()
{
	////test();
	////testFactory();
	//testTabWindow();

	//std::thread t = std::thread(createNode, 3);
	TDManager* td_Manager = TDSingleton<TDManager>::getInstance();

	td_Manager->createRootWindow();

	//HINSTANCE hinst = td_Manager->getRootWindow()->getInstance();
	//HWND	  hwnd = td_Manager->getRootWindow()->getHwnd();

	td_Manager->getRootWindow()->displayWindow();
	//testInputClass(hinst, hwnd);  //Not a good place to test this here
	td_Manager->getRootWindow()->run();

	delete td_Manager;
	
	
	cin.get();
	return 0;
}
//
//void createNode(int i)
//{
//
//}
