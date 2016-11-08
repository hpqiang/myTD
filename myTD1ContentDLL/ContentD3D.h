#pragma once

#include "Content.h"
#include "inputclass.h"
#include "graphicsclass.h"

class __declspec(dllexport) ContentD3D : public Content
{
public:
	ContentD3D() {}
	/*virtual*/ ~ContentD3D() {}

	virtual bool loadContent(IContent** content, const string& source) override;

	virtual bool createInputObject() override;

	virtual bool createGraphicsObject(HWND hwnd) override;

	virtual void Render(int a) override;

private:
	InputClass		*m_Input;
	GraphicsClass	*m_Graphics;
};