#ifndef REXBDD_UNIQUE_TABLE_H
#define REXBDD_UNIQUE_TABLE_H

#include "defines.h"
#include "node_manager.h"
#include "forest.h"

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
        UniqueTable(Forest *f);
        ~UniqueTable();

        /// Get the unique table size for a given level
        inline uint64_t getSize(int varLvl) const {return tables[varLvl].getSize();}
        /// Get the total size (sum over all levels)
        uint64_t getSize() const;

        /// Get the number of unique nodes at a given level
        inline uint64_t getNumEntries(int varLvl) const {return tables[varLvl].getNumEntries();}
        /// Get the total number of unique nodes (sum over all levels)
        uint64_t getNumEntries() const;

        /// Get the memory used for a given level
        inline uint64_t getMemUsed(int varLvl) const {return tables[varLvl].getMemUsed();}
        /// Get the total memory used (sum over all variables)
        uint64_t getMemUsed() const;

        /** If the table of the given variable level contains key node, return the item 
         * and move it to the front of the list. Otherwise, return 0 and do nothing.
         */
        inline nodeHandle find(Node &key, int level) const {return tables[level].find(key);}

        /** Add the nodeHandle item to the front of the list of the corresponding variable.
         *  Used when we KNOW that the item is not in the unique table already.
         */
        inline void add(unsigned long hash, nodeHandle item) {
            // get node level, it may be negative if for relation
            // get var level
            // TBD
        }

        /** If the unique table of corresponding variable contains key node, remove it and return it.
         *  Otherwise, return 0.
         */
        inline nodeHandle remove(unsigned long hash, nodeHandle item) {
            // get node level, it may be negative if for relation
            // get var level
            // TBD
        }

        /// Remove all unmarked nodes from the unique table
        void sweep();

        /// Clear the nodeHanlde items in the table of the given variable level and reset the state.
        inline void clear(int varLvl) {return tables[varLvl].clear();}


    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        class subtable {
            public:
                subtable();
                ~subtable();

                inline uint64_t getSize() const {
                    return PRIMES[sizeIndex];
                }
                inline uint64_t getNumEntries() const {
                    return numEntries;
                }
                inline uint64_t getMemUsed() const {
                    return PRIMES[sizeIndex] * sizeof(nodeHandle);
                }

                // Stats for future

                // void reportStats(output &s, const char* pad,
                //         reporting_flags flags) const;

                // /// For debugging
                // void show(output& s) const;

                /**
                    Initialize the sub table. Must be called before use.
                */
                void init(Forest *ef);

                /** If table contains key, move it to the front of the list.
                    Otherwise, do nothing.
                    Returns the item if found, 0 otherwise.

                    Class T must have the following methods:
                        unsigned hash():    return the hash value for this item.
                        bool equals(int p): return true iff this item equals node p.
                */
                template <typename T> nodeHandle find(const T &key) const
                {
                    unsigned h = key.hash() % size;
                    MEDDLY::CHECK_RANGE(__FILE__, __LINE__, 0u, h, size);
                    nodeHandle prev = 0;
                    for (nodeHandle ptr = table[h];
                            ptr != 0;
                            ptr = parent->getNext(ptr))
                    {
                        if (parent->areDuplicates(ptr, key)) {
                            // MATCH
                            if (ptr != table[h]) {
                                // Move to front
                                MEDDLY_DCASSERT(prev);
                                parent->setNext(prev, parent->getNext(ptr));
                                parent->setNext(ptr, table[h]);
                                table[h] = ptr;
                            }
                            MEDDLY_DCASSERT(table[h] == ptr);
                            return ptr;
                        }
                        prev = ptr;
                    }

                    return 0;
                }

                /** Add the item to the front of the list.
                    Used when we KNOW that the item is not
                    in the unique table already.
                */
                void add(unsigned hash, nodeHandle item);

                /** If table contains key, remove it and return it.
                    I.e., the exact key.
                    Otherwise, return 0.
                */
                int remove(unsigned hash, nodeHandle item);

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
                unsigned getItems(nodeHandle* items, unsigned sz) const;

            private:  // helper methods
                /// Empty the hash table into a list; returns the list.
                nodeHandle convertToList();

                /// A series of inserts; doesn't check for duplicates or expand.
                void buildFromList(nodeHandle front);

                /// Expand the hash table (if possible)
                void expand();

                /// Shrink the hash table
                void shrink();

            private:
                Forest* parent;
                nodeHandle* table;
                int sizeIndex;              // Table size at this level, index of PRIMES
                uint64_t numEntries;        // The number of nodes at this level
                unsigned nextExpandSize;    // The size for next expand
                unsigned nextShrinkSize;    // The size for next shrink
        }; // class subtable

        // ========================================================
        Forest*             parent;     // Parent forest
        subtable*           tables;     // Subtables divided by levels
        // int                 maxVar;    // The number of variables
        // int                 minVar;    // 0 for regular BDDs, -max_var for relation BDDs
};


#endif