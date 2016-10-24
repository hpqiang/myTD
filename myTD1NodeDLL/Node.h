#pragma once

#include <list>
using namespace std;

#if defined(_WIN32)
#include <windows.h>
//else

#endif

typedef unsigned int uint; //to do: move these defs to common def.h
typedef unsigned short ushort;

typedef struct ColorOP
{
public:
	ColorOP(){}
	ColorOP(ushort r, ushort g, ushort b, ushort a)
	{
		m_Red = r; m_Green = g; m_Blue = b; m_Alpha = a;
	}
	ColorOP xor(const ushort r, const ushort g, const ushort b, const ushort a)
	{
		return ColorOP(r^m_Red, g^m_Green, b^m_Blue, a^m_Alpha);
	}
	//To do: add more 'operator's

private:
	ushort m_Red;
	ushort m_Green;
	ushort m_Blue;
	ushort m_Alpha;
}ColorOP, *PColorOP;

typedef struct CoordinateOP
{
public:
	CoordinateOP(){}
	CoordinateOP(uint x, uint y)
	{
		m_X = x; m_Y = y;
	}
	CoordinateOP(const CoordinateOP& c){}
	~CoordinateOP(){}

private:
	uint	m_X;
	uint	m_Y;
}*PCoordinateOP;

typedef struct GeometryOP
{
public:
	GeometryOP() {}
	GeometryOP(const GeometryOP& c) {}
	~GeometryOP() {}
private:
	//MATRIX m_World; //To do: use a common Matrix
	//MATRIX m_View;
	//MATRIX m_Projection;
}*PGeometryOP;

typedef struct LightOP
{
public:
	LightOP() {}
	LightOP(const LightOP& c) {}
	~LightOP() {}
private:
	//SpotLight m_SpotLight; //To do: add more light types
}*PLightOP;

typedef struct MaterialOP
{
public:
	MaterialOP() {}
	MaterialOP(const MaterialOP& c) {}
	~MaterialOP() {}
private:
	//Material m_Material; //To do: add more material types OR material is actually a texture?
}*PMaterial;

typedef struct TextureOP
{
public:
	TextureOP() {}
	TextureOP(const TextureOP& c) {}
	~TextureOP() {}
private:
	//Texture m_Texture; //To do: add more material types OR material is actually a texture?
}*PTexture;

class INode
{
	virtual bool	createNode(INode** node) = 0;
	virtual bool	deleteNode(INode& node) = 0;

	virtual INode&	getNode() const = 0;
	virtual void	setNode(const INode& node) = 0;

	virtual INode&	getNodeIn(const uint idx) const = 0;
	virtual INode&	getNodeIn(const INode& node) const = 0;
	virtual void	setNodeIn(const INode& nodeIn) = 0;

	virtual INode&	getNodeOut(const uint idx) const = 0;
	virtual INode&	getNodeOut(const INode& node) const = 0;
	virtual void	setNodeOut(const INode& nodeIn) = 0;

	virtual bool	ConnectFrom(const INode& nodeIn) = 0;
	virtual bool	ConnectTo(const INode& nodeOut) = 0;
	//Q: Interface needs destructor???
	//virtual ~INode() = 0;
};

class IContent;

class Node : public INode
{
public:
	Node() 
	{
	}
	Node(const Node& node) {}
	virtual ~Node(){}

private:
	INode*  m_NodeIn; //Q: Can use INode& here?
	uint	m_NumOfNodeIn;
	INode*	m_NodeOut;
	uint	m_NumOfNodeOut;

	IContent& m_Content;
};

class NodeWin :public Node
{
public:
	//To do: How to deal with WndProc?

private:
	HWND hwnd;
};

class IOP
{

};

class OP :public IOP
{

};

class NodeOP :public Node, public OP
{
public:
private:

};

class NodeOP2D :public NodeOP
{
public:
private:
	PColorOP		m_OP2DColor;
	PCoordinateOP	m_OP2DCoordinate;
};

class NodeOP3D :public NodeOP
{
public:
private:
	GeometryOP* m_OP3DGeometry;
	LightOP*	m_OP3DLight;
	MaterialOP* m_OP3DMaterial;
	TextureOP*	m_OP3DTexture;
};

class IContent
{
	virtual bool	loadContent(IContent** content, const string& source) = 0;
};

class Content :public IContent
{
public:
	Content(){}
	Content(const Content& content){}
	virtual ~Content(){}

	bool loadContent(IContent** content, const string& source)
	{

	}

private:
	string	m_Source;
};

class TextContent:public Content
{
public:
private:

};

class TableContent :public Content
{
public:
private:

};

class PythonContent :public Content
{
public:
private:

};


class _2DContent:public Content
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
	OGLClass* m_Content;
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

class __declspec(dllexport) TDManager
{
public:
	// to do : add CRUD for Node handling
private:
	NodeManager*	m_NodeManager;
	ContentManager* m_ContentManager;
	HWND			m_RootWindow;
};

class NodeManager
{
public:
	// to do : add CRUD for Node handling
private:
	list<INode *>*	m_Nodes;
};

class ContentManager
{
public:
	// to do : add CRUD for Node handling
private:
	list<IContent *>*	m_Nodes;
};
