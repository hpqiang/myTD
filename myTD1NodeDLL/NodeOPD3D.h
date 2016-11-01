#pragma once
#include "NodeOP.h"
#include "CommonDefs.h"

class NodeOPD3D :public NodeOP  
{
public:
	bool createGraphicsObject()
	{
		//bool result;
		return true; //Q: Temp
	}

	//Temp: avoid a compile error since now this class is actually derived from NodeWin
	virtual void Render() override
	{
		return;
	}

	//Temp: avoid a compile error since now this class is actually derived from NodeWin
	//Temp: This should be done with NodeOPD3DGeometry
	virtual void getD3DConnectionOP(myD3DConnectionOP *op)
	{
		//cout << "Calling to this function: " << __FUNCTION__ << endl;
		myRotation mR;
		mR.m_Rx = 5.0f;
		mR.m_Ry = 5.5f;
		mR.m_Rz = 2.0f;
		op->myGeometryOP->setRotation(mR);

		return;
	}


private:
	//PGeometryOP m_OP3DGeometry;
	//PLightOP	m_OP3DLight;
	//PMaterialOP m_OP3DMaterial;
	//PTextureOP	m_OP3DTexture;
};

class NodeOPD3DGeometry : public NodeOPD3D
{
public:

	virtual void getD3DConnectionOP(myD3DConnectionOP *op)
	{
		cout << "Calling to this function: " << __FUNCTION__ << endl;
		return;
	}

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
