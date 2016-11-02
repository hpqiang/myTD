////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//////////////
// INCLUDES //
//////////////
//#include <windows.h>
//#include "nodeWindow.h"  //Don't include this

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;// true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
//extern struct myD3DConnectionOP; //forward declaration
//#include "../myTD1NodeDLL/NodeWin.h"
// /*extern*/ struct __declspec(dllimport) myD3DConnectionOP;

#include "../myMain/DefineCommon.h"

class __declspec(dllexport) GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int,int);

	bool Render(myD3DConnectionOP *op);
	bool Render(float rotX, float rotY, float rotZ,
		float tX, float tY, float tZ,
		float sX, float sY, float xZ);
	bool Render(int rot);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
};

#endif