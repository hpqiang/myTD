#pragma once

#include "CommonDefs.h"
#include "Node.h"
#include "NodeWin.h"
//#include "NodeOP.h"
//#include "NodeOP3D.h"

//#include "NodeOPD3D.h"

class IOP
{

};

class OP :public IOP
{

};

class NodeOP :public NodeWin, public OP  //Q: NodeWin OR Node here???
{
public:
private:

};
//
//class NodeOPVR :public NodeOP3D
//{
//public:
//private:
//
//};
//
//class NodeOPAR : public NodeOP3D
//{
//public:
//private:
//
//};
