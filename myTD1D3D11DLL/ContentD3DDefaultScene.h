#pragma once

#include "D3DBase.h"

class __declspec(dllexport) ContentD3DDefaultScene
	: public D3DBase
{
public:
	ContentD3DDefaultScene();
	~ContentD3DDefaultScene();

	virtual bool initializeCamera() override;
	virtual bool initializeModel() override;
	virtual bool initializeTextureShader(HWND hwnd) override;

	virtual void D3DRender(int rot) override;
};
