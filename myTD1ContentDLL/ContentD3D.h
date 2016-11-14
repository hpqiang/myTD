#pragma once

#include "Content.h"
#include "inputclass.h"
#include "graphicsclass.h"
#include "D3DBase.h"

class __declspec(dllexport) ContentD3D : public Content
{
public:
	ContentD3D();
	ContentD3D(void *t, HWND hwnd);
	/*virtual*/ ~ContentD3D(); // {}

	virtual bool contentOP1(HWND hwnd) override;
	virtual void contentOP2(HWND hwnd) override;

	bool createInputObject(HWND hwnd);

	//bool createGraphicsObject(HWND hwnd);
	bool createD3DBaseObject(HWND hwnd);

	void Update(HWND hwnd);
	void Render(int a, HWND hwnd);

private:
	InputClass		*m_Input;
	//GraphicsClass	*m_Graphics;
	D3DBase			*m_D3DBase;
};