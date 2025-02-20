#ifndef REXBDD_NODE_MANAGER_H
#define REXBDD_NODE_MANAGER_H

#include "defines.h"
#include "node.h"

namespace REXBDD {
    class Forest;
    class NodeManager;

    // I/O TBD
    // stats for performance measurement TBD
}

// ******************************************************************
// *                                                                *
// *                   Node Manager  class                          *
// *                                                                *
// ******************************************************************
class REXBDD::NodeManager {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    NodeManager(Forest *f);
    ~NodeManager();

    /**
     *  Get an unused node handle.
     *  This is either a recycled one or
     *  the next one in the available pool
     *  (which will be expanded if necessary).
     *  Then fill it with the given unpacked node.
     */
    inline NodeHandle getFreeNodeHandle(const uint16_t lvl, const Node& node) {
        return chunks[lvl-1].getFreeNodeHandle(node);
    }

    /**
     *  Find the node corresponding to a node handle
     */
    inline Node& getNodeFromHandle(const uint16_t lvl, const NodeHandle h) {
        return chunks[lvl-1].getNodeFromHandle(h);
    }

    /**
     *  Recycle a used node handle.
     *  The recycled handle can eventually be
     *  reused when returned by a call to
     *  getFreeNodeHandle().
     */
    void recycleNodeHandle(uint16_t lvl, NodeHandle h);

    /**
     *  Sweep a manager.
     *  For each node in it, check if it is marked or not.
     *  If marked, the mark bit(s) is cleared.
     *  If unmarked, the node is recycled.
     */
    void sweep(uint16_t lvl);
    void sweep();

    void unmark(uint16_t lvl);
    void unmark();

    inline uint32_t numUsed(uint16_t lvl) const { return PRIMES[chunks[lvl-1].sizeIndex] - chunks[lvl-1].numFrees; }
    inline uint32_t numAlloc(uint16_t lvl) const { return chunks[lvl-1].firstUnalloc; }

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    class SubManager {
        public:
            SubManager(Forest *f);
            ~SubManager();

            void sweep();
        private:
        // ======================Helper Methods====================
            /// Get a free NodeHandle and fill it with a given node
            NodeHandle getFreeNodeHandle(const Node& node);
            /// Find the node corresponding to a node handle
            Node& getNodeFromHandle(const NodeHandle h);

            /// Expand the nodes to next size (if possible)
            void expand();
            /// Shrink the nodes to previous size
            void shrink();

        // ========================================================
            friend class NodeManager;
            Forest*     parent;         // Parent forest
            Node*       nodes;          // Actual node storage; the 1st slot (nodes[0]) will not be used
            int         sizeIndex;      // Index of prime number for size
            uint32_t    firstUnalloc;   // Index of first unallocated slot
            uint32_t    freeList;       // Header of the list of unused slots
            uint32_t    numFrees;       // Number of free/unused slots
            uint32_t    recycled;       // Last recycled node index

    }; // class SubManager

    // ======================Helper Methods====================


    // ========================================================
    Forest* parent;        // Parent Forest
    SubManager* chunks;    // Chunks by levels

};

#endif