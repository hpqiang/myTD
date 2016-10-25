// MathClient.cpp : Defines the entry point for the console application.
// Compile by using: cl /EHsc /link MathLibrary.lib MathClient.cpp

#include <iostream>
//#include "myDLL.h"
#include "inputclass.h"
#include "Node.h"
#include "TDManager.h"
#include "TDSingleton.h"
#include "TDFactory.h"

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

void testInputClass()
{
	//InputClass* iC = TDSingleton<InputClass>::getInstance();
	//Node* iC = TDSingleton<Node>::getInstance();
	TDFactory<InputClass> *iC = new TDFactory<InputClass>();

	cout << typeid(iC).raw_name() << endl;
	cout << typeid(iC).name() << endl;
	cout << typeid(iC).hash_code() << endl;
	//Input has to be with hinstance and hwnd??? yes.
}

int main()
{

	//test();
	//testFactory();
	//testInputClass();

	TDManager* td_Manager = TDSingleton<TDManager>::getInstance();

	td_Manager->createRootWindow();

	//cin.get();
	return 0;
}