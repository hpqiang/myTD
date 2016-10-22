////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
//#ifndef _INPUTCLASS_H_
//#define _INPUTCLASS_H_
#define TDD3DLIBRARY_EXPORTS

#ifdef TDD3DLIBRARY_EXPORTS
#define TDD3DLIBRARY_API __declspec(dllexport) 
#else
#define TDD3DLIBRARY_API __declspec(dllimport) 
#endif

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class __declspec(dllexport) InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

//#endif