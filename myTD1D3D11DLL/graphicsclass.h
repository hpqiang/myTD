#pragma once

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"

//#include "Content.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;// true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//Temp for myD3DConnectionOP
//Q: Make this and other corresponding struct to be shared memory???
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

//To do: Need to distinguish local and world Geometry OPs
typedef struct GeometryOP
{
public:
	GeometryOP()
	{
	}
	~GeometryOP()
	{
	}

	myRotation getRotation()
	{
		return m_Rotation;
	}
	void setRotation(const myRotation& mR)
	{
		m_Rotation = mR;
	}

	myTranslation getTranslation()
	{
		return m_Translation;
	}
	void setTranslation(const myTranslation& mT)
	{
		m_Translation = mT;
	}

	myScalar getScalar()
	{
		return m_Scalar;
	}
	void setScalar(const myScalar mS)
	{
		m_Scalar = mS;
	}

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

//Temp: To avoid a dll recycling references. Using declspec(dllimport)/declspec(dllexport) causing problems....
// Former mistake: Did not put it here, ownership of this OP WAS misleading
struct __declspec(dllexport) myD3DConnectionOP
{
	PGeometryOP myGeometryOP;
	PLightOP	myLightOP;
	PMaterialOP myMaterialOP;
	PTextureOP	myTextureOP;
};

//To do: Make GraphicsClass a base class????
class __declspec(dllexport) GraphicsClass
{
public:
	GraphicsClass();
	/*virtual*/ ~GraphicsClass();

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