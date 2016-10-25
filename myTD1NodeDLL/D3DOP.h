#pragma once
#include "OP.h"

#include <D3DX10math.h>

typedef struct GeometryOP
{
public:
	GeometryOP() {}
	GeometryOP(const GeometryOP& c) {}
	~GeometryOP() {}
private:
	D3DMATRIX m_World; //To do: use a common Matrix
	D3DMATRIX m_View;
	D3DMATRIX m_Projection;
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
}*PMaterialOP;

typedef struct TextureOP
{
public:
	TextureOP() {}
	TextureOP(const TextureOP& c) {}
	~TextureOP() {}
private:
	//Texture m_Texture; //To do: add more material types OR material is actually a texture?
}*PTextureOP;
//To do: More 3D effect: cubemap, environment mapp, shadow, smoke, water...

class NodeOPD3D :public NodeOP
{
public:
private:
	PGeometryOP m_OP3DGeometry;
	PLightOP	m_OP3DLight;
	PMaterialOP m_OP3DMaterial;
	PTextureOP	m_OP3DTexture;
};