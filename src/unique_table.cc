#include "unique_table.h"
#include "forest.h"

// ******************************************************************
// *                                                                *
// *                                                                *
// *                    UniqueTable methods                         *
// *                                                                *
// *                                                                *
// ******************************************************************

REXBDD::UniqueTable::UniqueTable()
{
    //
}
REXBDD::UniqueTable::UniqueTable(Forest* f)
{
    //
}
REXBDD::UniqueTable::~UniqueTable()
{
    //
}

/** If table contains key, move it to the front of the list.
    Otherwise, do nothing.
    Returns the item if found, 0 otherwise.

    Class T must have the following methods:
        unsigned hash():    return the hash value for this item.
        bool equals(int p): return true iff this item equals node p.
*/
template <typename T> 
REXBDD::NodeHandle REXBDD::UniqueTable::subtable::find(const T &key) const
{
    unsigned h = key.hash() % PRIMES[sizeIndex];
    // REXBDD::CHECK_RANGE(__FILE__, __LINE__, 0u, h, PRIMES[sizeIndex]);
    NodeHandle prev = 0;
    for (NodeHandle ptr = table[h];
            ptr != 0;
            ptr = parent->getNodeNext(ptr))
    {
        if (parent->areDuplicates(ptr, key)) {
            // MATCH
            if (ptr != table[h]) {
                // Move to front
                REXBDD_DCASSERT(prev);
                parent->setNodeNext(prev, parent->getNodeNext(ptr));
                parent->setNodeNext(ptr, table[h]);
                table[h] = ptr;
            }
            REXBDD_DCASSERT(table[h] == ptr);
            return ptr;
        }
        prev = ptr;
    }

    return 0;
}