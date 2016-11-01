#pragma once

typedef unsigned int uint; //to do: move these defs to common def.h
typedef unsigned short ushort;


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
	//GeometryOP(const GeometryOP& c) {}
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
