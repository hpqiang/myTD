////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
//#include "systemclass.h"

#include <Windows.h>

#include <iostream>
using namespace std;

#include "rootWindow.h"

int getOSEnv()
{
	LPTSTR lpszVariable;
	LPTCH lpvEnv;

	// Get a pointer to the environment block. 

	lpvEnv = GetEnvironmentStrings();

	// If the returned pointer is NULL, exit.
	if (lpvEnv == NULL)
	{
		printf("GetEnvironmentStrings failed (%d)\n", GetLastError());
		return 0;
	}

	// Variable strings are separated by NULL byte, and the block is 
	// terminated by a NULL byte. 

	lpszVariable = (LPTSTR)lpvEnv;

	while (*lpszVariable)
	{
		cout << "var: " << lpszVariable << endl;
		lpszVariable += lstrlen(lpszVariable) + 1;
	}
	FreeEnvironmentStrings(lpvEnv);

	return 0;
}

void getProjectMacro()
{
	cout << TARGET_DIR << endl;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main()
{

	//getOSEnv();
	//getProjectMacro();
	//cin.get();

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