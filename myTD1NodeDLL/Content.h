#pragma once

#include "CommonDefs.h"

class IContent
{
	virtual bool	loadContent(IContent** content, const string& source) = 0;
};

class Content :public IContent
{
public:
	//Content(){}
	//Content(const Content& content){}
	virtual ~Content() {}

	bool loadContent(IContent** content, const string& source)
	{

	}

private:
	Content() {}
	Content(const Content& content) {}
	void operator=(const Content& content)
	{
		this->m_Source = content.m_Source;
	}
	string	m_Source;
};

//class TextContent :public Content  //Note: Move to 'Text' menu
//{
//public:
//private:
//
//};
//
//class TableContent :public Content  //Note: Move to 'Table' menu
//{
//public:
//private:
//
//};

//class PythonContent :public Content  //Note: Move to 'Python' menu
//{
//public:
//private:
//
//};

class _2DContent :public Content
{
public:
private:

};

class D3DContent : public Content
{
public:
private:
	//D3DClass* m_Content; //To do: D3DClass?
};

class OGLContent :public Content
{
public:
private:
	//OGLClass* m_Content;
};

class AudioContent :public Content
{
public:
private:

};
class VideoContent :public Content
{
public:
private:

};

class CameraContent : public VideoContent
{
public:
private:

};
