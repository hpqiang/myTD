#include <iostream>
//#include "myDLL.h"
#include "inputclass.h"
//#include "Node.h"
#include "TDManager.h"
#include "TDSingleton.h"
#include "TDFactory.h"

#include "main.h"

//#include "NodeManager.h" //Q: Why need to include this here??

#include <thread>

using namespace std;

int f3(int a, float b)
{
	return 1;
}
int f4(int a, float b)
{
	return 1;
}

struct X
{
	int a;
	float b;
	double c;
};

class A
{
public:
	static int i;

	//const /*static*/ X x[] =
	//{
	//	{ 1, 3.3f, 4.4f },
	//	{ 2, 6.6f, 6.6f },
	//};

	//void printx()
	//{
	//	cout << sizeof(x) / sizeof(x[0]) << endl;
	//}
	//template<class T>
	//class Z
	//{
	//	int a;
	//	T* t;
	//};


	//Z<A> z[] =
	//{
	//	{1},
	//	{2},
	//};


	//template<typename T, typename... Args>
	//class YY
	//{
	//	int a;
	//	string b;
	//	T(*func)(Args...args);
	//};

	//A::YY<int, void *, int, string> yy[] =  //Q: YY is not a template here???
	//{
	//	{1, "Mike", f1},
	//	{2, "Qiang", f2},
	//};

	//static int f1(int a, string b)
	//{
	//	return 1;
	//}

	//static int f2(int a, string b)
	//{
	//	return 1;
	//}
};


void test()
{
	D<int, int, float> d[] =
	{
		{ 1, 1.1f, f1},
		{ 2, 2.2f, f2 },
		{ 3, 3.3f, f3 },
		{ 4, 4.4f, f4 },
	};



	//double a = 7.4;
	//int b = 99;

	//cout << "a + b = " <<
	//	MathLibrary::Functions::Add(a, b) << endl;
	//cout << "a * b = " <<
	//	MathLibrary::Functions::Multiply(a, b) << endl;
	//cout << "a + (a * b) = " <<
	//	MathLibrary::Functions::AddMultiply(a, b) << endl;
}

//Q: To do: Better use of variadic template???
//int  test1(string s, uint x = 0, long y = 0L)
//{
//	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
//	return 1;
//}
//int test2(string s, uint x = 0, long y = 0L)
//{
//	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
//	return 2;
//}
//int test3(string s, uint x = 0, long y = 0L)
//{
//	cout << __FUNCTION__ << "s = " << s.c_str() << endl;
//	return 3;
//}
//
//template<typename R, typename... Args>
//struct testcallback
//{
//	string c;
//	R (*func)(Args... args);
//};
//
//testcallback<int, string, uint, long> testCallBack[] =
//{
//	{ "1", test1 },
//	{ "2", test2 },
//	{ "3", test3 }
//};
//


void testFactory()
{
	//TDFactory<Node> *f = new TDFactory<Node>();
	//Node* n = f->getInstance();

	//delete n;
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


int main()
{
	//test();
	////testFactory();

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
