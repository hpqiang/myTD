////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	//HPQ
	bool Initialize(ID3D11Device*); // , ID3D11DeviceContext*);
	//eHPQ
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
private:
	bool LoadBitMap(ID3D11Device*);
	bool LoadWindowBitmap(ID3D11Device*);
	bool LoadDesktopBitmap(ID3D11Device*);// , ID3D11DeviceContext*);

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif