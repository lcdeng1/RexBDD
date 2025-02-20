#ifndef REXBDD_UNIQUE_TABLE_H
#define REXBDD_UNIQUE_TABLE_H

#include "defines.h"
#include "node_manager.h"

namespace REXBDD {
    class Forest;
    class UniqueTable;
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                     UniqueTable class                          *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::UniqueTable {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        UniqueTable();
        UniqueTable(Forest *f);
        ~UniqueTable();

        /// Get the unique table size for a given level
        inline uint32_t getSize(int varLvl) const {return tables[varLvl-1].getSize();}
        /// Get the total size (sum over all levels)
        uint64_t getSize() const;

        /// Get the number of unique nodes at a given level
        inline uint32_t getNumEntries(int varLvl) const {return tables[varLvl-1].getNumEntries();}
        /// Get the total number of unique nodes (sum over all levels)
        uint64_t getNumEntries() const;

        /// Get the memory used for a given level
        inline uint64_t getMemUsed(int varLvl) const {return tables[varLvl-1].getMemUsed();}
        /// Get the total memory used (sum over all variables)
        uint64_t getMemUsed() const;

        /**
         * Add a Node to the unique table.
         * If unique, returns a new handle; otherwise, returns the handle of the duplicate.
         * 
         * In either case, the returned node handle becomes the front entry of the hash chain.
         * 
         * @param lvl               The level of the node
         * @param node              The given node to be inserted
         * @return NodeHandle 
         */
        inline NodeHandle insert(uint16_t lvl, const Node& node) {
            return tables[lvl-1].insert(node);
        };

        /** If the table of the given variable level contains key node, return the item 
         * and move it to the front of the list. Otherwise, return 0 and do nothing.
         */
        // inline NodeHandle find(uint16_t level, Node &node) const {return tables[level].find(node);}

        /** Add the NodeHandle item to the front of the list of the corresponding variable.
         *  Used when we KNOW that the item is not in the unique table already.
         */
        // inline void add(unsigned long hash, NodeHandle item) {
        //     // get node level, it may be negative if for relation
        //     // get var level
        //     // TBD
        // }

        /** If the unique table of corresponding variable contains key node, remove it and return it.
         *  Otherwise, return 0.
         */
        // inline NodeHandle remove(unsigned long hash, NodeHandle item) {
        //     // get node level, it may be negative if for relation
        //     // get var level
        //     // TBD
        //     return 0;
        // }

        /// Remove all unmarked nodes from the unique table
        inline void sweep(uint16_t level) {tables[level-1].sweep();}
        void sweep();

        /// Clear the nodeHanlde items in the table of the given variable level and reset the state.
        inline void clear(int varLvl) {return tables[varLvl-1].clear();}


    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        class SubTable {
            public:
                SubTable(uint16_t lvl, Forest* f);
                ~SubTable();

                inline uint32_t getSize() const {
                    return PRIMES[sizeIndex]>=UINT32_MAX?UINT32_MAX:PRIMES[sizeIndex];
                }
                inline uint32_t getNumEntries() const {
                    return numEntries;
                }
                inline uint64_t getMemUsed() const {
                    return PRIMES[sizeIndex] * sizeof(NodeHandle);
                }

                // Stats for future TBD

                // /// For debugging
                // void show(output& s) const;

                /** If table contains key, move it to the front of the list.
                    Otherwise, do nothing.
                    Returns the item if found, 0 otherwise.
                */
                NodeHandle insert(const Node& node);
                /**
                 * Sweep a subtable.
                 *  For each nodehandle in it, check if its represented node is marked or not.
                 *  If marked, skip (mark bit(s) will be cleared in NodeManager sweep).
                 *  If unmarked, the nodehandle will be removed.
                 * 
                 */
                void sweep();

                /** If table contains key, remove it and return it.
                    I.e., the exact key.
                    Otherwise, return 0.
                */
                int remove(unsigned hash, NodeHandle item);

                /**
                    Remove all the items in the table and reset the state.
                */
                void clear();

                /**
                    Retrieve the items in the table of the given variable.
                        @param  var     Variable of interest
                        @param  NH      Array of node handles
                        @param  sz      Dimension of NH array, and max items
                                        to retrieve.

                        @return         Number of items retrieved; will be
                                        at most sz.
                */
                unsigned getItems(NodeHandle* items, unsigned sz) const;

            private:
            // ======================Helper Methods====================
                /// Expand the hash table (if possible)
                void expand();

                /// Shrink the hash table
                void shrink();
            // ========================================================
                friend class UniqueTable;
                Forest*         parent;
                NodeHandle*     table;
                uint16_t        level;              // The level of stored nodes
                int             sizeIndex;          // Table size at this level, index of PRIMES
                uint64_t        numEntries;         // The number of nodes at this level
        }; // class SubTable

        // ========================================================
        Forest*         parent;     // Parent forest
        SubTable*       tables;     // Subtables divided by levels
};


#endif