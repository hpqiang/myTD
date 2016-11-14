#pragma once

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"

const bool isFULL_SCREEN = false;
const bool isVSYNC_ENABLED = false;// true;
const float fSCREEN_DEPTH = 1000.0f;
const float fSCREEN_NEAR = 0.1f;

class /*__declspec(dllexport)*/ ID3DBase
{
	virtual bool initializeCamera() = 0;
	virtual bool initializeModel() = 0;
	virtual bool initializeTextureShader(HWND hwnd) = 0;
	//To do: Op* for D3DRender....
	virtual void D3DRender(int rot) = 0;
};

class __declspec(dllexport) D3DBase : public ID3DBase
{
public:
	D3DBase();
	~D3DBase();

	bool initializeD3DEnv(HWND hwnd);

	virtual bool initializeCamera() override;
	virtual bool initializeModel() override;
	virtual bool initializeTextureShader(HWND hwnd) override;

	virtual void D3DRender(int rot) override;

protected:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
};