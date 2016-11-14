#pragma once

#include "D3DBase.h"

class __declspec(dllexport) ContentD3DOPGeometry
	: public D3DBase
{
public:
	ContentD3DOPGeometry();
	~ContentD3DOPGeometry();

	virtual bool initializeCamera() override;
	virtual bool initializeModel() override;
	virtual bool initializeTextureShader(HWND hwnd) override;

	virtual void D3DRender(int rot) override;
};
