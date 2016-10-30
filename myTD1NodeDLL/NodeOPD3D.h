#pragma once
#include "NodeOP.h"
//#include "PropertyWin.h"
//#include <D3DX10math.h>

struct myRotation
{
	float m_Rx;  //In radian
	float m_Ry;
	float m_Rz;
};
struct myTranslation
{
	float m_Tx;
	float m_Ty;
	float m_Tz;
};
struct myScalar
{
	float m_Sx;
	float m_Sy;
	float m_Sz;
};

typedef struct GeometryOP
{
public:
	GeometryOP() {}
	GeometryOP(const GeometryOP& c) {}
	~GeometryOP() {}
private:
	myRotation		m_Rotation;
	myTranslation	m_Translation;
	myScalar		m_Scalar;

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
	//PGeometryOP m_OP3DGeometry;
	//PLightOP	m_OP3DLight;
	//PMaterialOP m_OP3DMaterial;
	//PTextureOP	m_OP3DTexture;
};

class NodeOPD3DGeometry : public NodeOPD3D
{
public:
private:
	PGeometryOP m_OP3DGeometry;
	//PropertyWinD3DGeometry* m_PropertyWin;
};

class NodeOPD3DLight : public NodeOPD3D
{
public:
private:
	PLightOP	m_OP3DLight;
};

class NodeOPD3DMaterial : public NodeOPD3D
{
public:
private:
	PMaterialOP m_OP3DMaterial;
};

class NodeOPD3DTexture : public NodeOPD3D
{
public:
private:
	PTextureOP	m_OP3DTexture;
};
