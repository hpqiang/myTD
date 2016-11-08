//#pragma once
//
//class __declspec(dllexport) InputClass
//{
//public:
//	InputClass();
//	InputClass(const InputClass&);
//	~InputClass();
//
//	void Initialize();
//
//	void KeyDown(unsigned int);
//	void KeyUp(unsigned int);
//
//	bool IsKeyDown(unsigned int);
//
//private:
//	bool m_keys[256];
//};

#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class __declspec(dllexport) InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);
	bool IsEscapePressed();
	bool IsLeftMouseButtonDown();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};