#pragma once

#include <Windows.h>

class __declspec(dllexport) IContent
{
public:
	//template<typename... Args>
	//virtual bool contentFirstOperation(Args... args) = 0;
	virtual bool contentOP1(HWND hwnd) = 0;
	//template<typename... Args>
	//virtual void contentSecondOperation(Args... args) = 0;
	virtual void contentOP2(HWND hwnd) = 0;
};

class __declspec(dllexport) Content :public IContent
{
public:
	Content() {}
	virtual ~Content() {}

	virtual bool contentOP1(HWND hwnd) override;
	virtual void contentOP2(HWND hwnd) override;
private:
};

//class ContentText :public Content  //Note: Move to 'Text' menu
//{
//public:
//private:
//
//};
//
//class ContentTable :public Content  //Note: Move to 'Table' menu
//{
//public:
//private:
//
//};

//class ContentPython :public Content  //Note: Move to 'Python' menu
//{
//public:
//private:
//
//};

class Content2D :public Content
{
public:
private:

};



class ContentOGL :public Content
{
public:
private:
	//OGLClass* m_Content;
};

class ContentAudio :public Content
{
public:
private:

};
class ContentVideo :public Content
{
public:
private:

};

class ContentCamera : public ContentVideo
{
public:
private:

};

//Q: AR/VR???
