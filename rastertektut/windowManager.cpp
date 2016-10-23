#include "windowManager.h"

static WindowManager* theWindowManager = nullptr;

WindowManager *
WindowManager::getWindowManager()
{
	if (theWindowManager == nullptr)
		theWindowManager = new WindowManager();
	return theWindowManager;
}
