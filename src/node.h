#ifndef REXBDD_NODE_H
#define REXBDD_NODE_H

#include "defines.h"
#include "edge_label.h"
#include "edge.h"

namespace REXBDD {
    class Forest;
    class Node;


}

// ******************************************************************
// *                                                                *
// *                           Node  class                          *
// *                                                                *
// ******************************************************************

class REXBDD::Node {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Node();

    ~Node();

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Level of the node
    //  Note:
    //      >=0: for regular node, or for unprimed node of relation
    //      <0: illegal for regular node, or for primed node of relation.
    int level;

    /// Down pointers
    nodeHandle childNode[2];

    /// Edge labels
    EdgeLabel childEdge[2];

    /// Hash of the node
    long long hashValue;
    
    
    /// ID of the parent forest
    unsigned parentFID;
    
    /// Parent forest where the node belongs
    const Forest* parentForest;

};

#endif