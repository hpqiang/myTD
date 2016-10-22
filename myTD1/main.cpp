////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
//#include "systemclass.h"

#include <Windows.h>

//#include <iostream>
//using namespace std;

#include "rootWindow.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main()
{
	RootWindow* rootWindow = new RootWindow();

	rootWindow->displayWindow();
	rootWindow->run();


	//bool result;
	//SystemClass* System;
	//
	//
	//// Create the system object.
	//System = new SystemClass;
	//if(!System)
	//{
	//	return 0;
	//}

	//// Initialize and run the system object.
	//result = System->Initialize();

	//if(result)
	//{
	//	System->Run();
	//}

	//// Shutdown and release the system object.
	//System->Shutdown();
	//delete System;
	//System = 0;

	//cout << "End...\n";

	//getchar();

	return 0;
}