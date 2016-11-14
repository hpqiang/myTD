#include "textureclass.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, LPCWSTR filename)
{
	HRESULT result;

	// Load the texture in.
	//result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	result = D3DX11CreateShaderResourceViewFromFileW(device, filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}