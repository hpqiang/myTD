#pragma once

#include "CommonDefs.h"
#include "NodeOP.h"

typedef struct ColorOP
{
public:
	ColorOP() {}
	ColorOP(ushort r, ushort g, ushort b, ushort a)
	{
		m_Red = r; m_Green = g; m_Blue = b; m_Alpha = a;
	}
	ColorOP xor (const ushort r, const ushort g, const ushort b, const ushort a)
	{
		return ColorOP(r^m_Red, g^m_Green, b^m_Blue, a^m_Alpha);
	}
	//To do: add more 'operator's

private:
	ushort m_Red;
	ushort m_Green;
	ushort m_Blue;
	ushort m_Alpha;
}*PColorOP;

typedef struct CoordinateOP
{
public:
	CoordinateOP() {}
	CoordinateOP(uint x, uint y)
	{
		m_X = x; m_Y = y;
	}
	CoordinateOP(const CoordinateOP& c) {}
	~CoordinateOP() {}

private:
	uint	m_X;
	uint	m_Y;
}*PCoordinateOP;

class NodeOP2D :public NodeOP
{
public:
private:
	PColorOP		m_OP2DColor;
	PCoordinateOP	m_OP2DCoordinate;
};