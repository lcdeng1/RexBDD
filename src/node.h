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
    Node(bool isRelation);
    ~Node();

    /// Methods =====================================================
    /**
     *  Get the down node handle for the given index of child
     *  0: low/left or 00 for relation nodes
     *  1: high/right or 01 for relation nodes
     *  2: 10 child for relation nodes
     *  3: 11 child for relation nodes
     */
    inline NodeHandle getChild(int i) const {
        return childNode[i];
    }
    
    
    /// Get this node's hash value
    inline unsigned long hash() const { return hashValue; }

    /// Compute the node's hash value
    void computeHash();

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Level of the node
    //  Note:
    //      >=0: for regular node, or for unprimed node of relation
    //      <0: illegal for regular node, or for primed node of relation.
    int level;

    NodeHandle* childNode;      // Down pointers
    EdgeLabel* childEdge;       // Edge labels
    unsigned long hashValue;    // Hash of the node
    unsigned parentFID;         // ID of the parent forest
    const Forest* parentForest; // Parent forest where the node belongs

};

#endif