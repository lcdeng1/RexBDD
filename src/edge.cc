#include "edge.h"

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         Edge  methods                          *
// *                                                                *
// *                                                                *
// ******************************************************************

REXBDD::Edge::Edge()
{
    parentFID = 0;
    targetNode = 0;
    prevEdge = nullptr;
    nextEdge = nullptr;
}

REXBDD::Edge::~Edge()
{

}

void REXBDD::Edge::setTarget(nodeHandle t)
{

}