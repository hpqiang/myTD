#pragma once

#include "CommonDefs.h"
//#include "../myTD1NodeDLL/inputclass.h"
//#include "../myTD1D3D11DLL/graphicsclass.h"

//#include "NodeWin.h"

#include <Windows.h>

class IContent
{
public:
	virtual bool loadContent(IContent** content, const string& source) = 0;
	virtual bool createInputObject() = 0;
	virtual bool createGraphicsObject(HWND hwnd) = 0;
	virtual void Render(int a) = 0; //Q: Should be here or where?
};

class __declspec(dllexport) Content :public IContent
{
public:
	Content() {}
	//Content(const Content& content){}
	/*virtual*/ ~Content() {}

	virtual bool loadContent(IContent** content, const string& source) override;

	virtual bool createInputObject() override;

	virtual bool createGraphicsObject(HWND hwnd) override;

	virtual void Render(int a) override;

private:
	//InputClass		*m_Input;
	//GraphicsClass	*m_Graphics;
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
