#ifndef REXBDD_NODE_MANAGER_H
#define REXBDD_NODE_MANAGER_H

#include "defines.h"
#include "node.h"
#include "packed_node.h"

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
    NodeManager();
    NodeManager(Forest *f);
    ~NodeManager();

    /**
     *  Get an unused node handle.
     *  This is either a recycled one or
     *  the next one in the available pool
     *  (which will be expanded if necessary).
     */
    inline NodeHandle getFreeNodeHandle(int varLvl) {
        // construct with level
        return constructHandle(varLvl, chunks[varLvl].getFreeSlot());
    }

    /**
     *  Find the packed node (pointer) corresponding to a node handle
     */
    PackedNode getNodeFromHandle(NodeHandle h);

    /**
     *  Set the given node handle with a unpacked node.
     *  This is usually used with method getFreeNodeHandle.
     */
    inline void setNodeForHandle(NodeHandle h, Node p) {
        // 
    }


    /**
     *  Recycle a used node handle.
     *  The recycled handle can eventually be
     *  reused when returned by a call to
     *  getFreeNodeHandle().
     */
    void recycleNodeHandle(NodeHandle h);

    /**
     *  Sweep a manager.
     *  For each node in it, check if it is marked or not.
     *  If marked, the mark bit(s) is cleared.
     *  If unmarked, the node is recycled.
     */
    void sweep(int varLvl);
    void sweep();


    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    class submanager {
        public:
            submanager(Forest *f);
            ~submanager();

            

            uint64_t getFreeSlot();

            void sweep();
            
        private:
            /// Expand the hash table (if possible)
            void expand();

            /// Shrink the hash table
            void shrink();

        private:
            Forest* parent;             // Parent forest
            PackedNode* nodes;          // Actual pack node storage
            int sizeIndex;              // Index of prime number for size
            uint64_t firstUnalloc;      // Index of first unallocated slot
            uint64_t freeList;          // Header of the list of unused slots
            uint64_t numFrees;          // Number of unused slots
    }; // class submanager

    // ======================Helper Methods====================
    inline NodeHandle constructHandle(int lvl, uint64_t index) {
        // check range
        #ifdef DCASSERTS_ON
            REXBDD_DCASSERT(lvl <= ((0x01<<numLevelBits)-1) 
                        && index <= ((0x01<<(HANDLE_LENGTH-numLevelBits))-1));
        #endif
        return (lvl<<(HANDLE_LENGTH-numLevelBits)) | index;
    }


    // ========================================================
    Forest* parent;                 // Parent Forest
    submanager* chunks;             // Chunks by levels
    int numLevelBits;               // number of level bits in node handle

};

#endif