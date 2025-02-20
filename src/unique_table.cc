#include "unique_table.h"
#include "forest.h"

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                        SubTable methods                        *
// *                                                                *
// *                                                                *
// ******************************************************************
UniqueTable::SubTable::SubTable(uint16_t lvl, Forest* f):parent(f),level(lvl)
{
    sizeIndex = 0;
    table = (NodeHandle*)malloc(PRIMES[sizeIndex] * sizeof(NodeHandle));
    if (!table) {
        std::cout << "[BRAVE_DD] ERROR!\t Malloc fail for subtable: "<<lvl<< std::endl;
        exit(0);
    }
    memset(table, 0, PRIMES[sizeIndex]*sizeof(NodeHandle));
    numEntries = 0;
}
UniqueTable::SubTable::~SubTable()
{
    for (uint32_t i=0; i<PRIMES[sizeIndex]; i++) {
        table[i] = 0;
    }
    free(table);
    sizeIndex = 0;
    numEntries = 0;
}

NodeHandle UniqueTable::SubTable::insert(const Node& node)
{
    /* Check if we should enlarge */
    if (numEntries >= PRIMES[sizeIndex+1]) expand();
    /* Determine the hash index for the node */
    uint32_t index = node.hash(parent->nodeSize) % getSize();
    // Special, and hopefully common, case: empty chain. Which means the node is new.
    if (!table[index]) {
        numEntries++;
        table[index] = parent->obtainFreeNodeHandle(level, node);
        return table[index];
    }
    // Non-empty chain. Check the chain for duplicates.
    NodeHandle curr = table[index];
    while (curr) {
        if (!parent->getNode(level, curr).isEqual(node, parent->nodeSize)) {
            curr = parent->getNodeNext(level, curr);
            continue;
        }
        // we found a duplicate. Move it to the front.
        return curr;
    }
    // No duplicates in the chain. 
    // Get a new node handle, and add the new node to the front.
    numEntries++;
    NodeHandle handle = parent->obtainFreeNodeHandle(level, node);
    parent->setNodeNext(level, handle, table[index]);
    table[index] = handle;
    return handle;
}

void UniqueTable::SubTable::sweep()
{
    /* For each chain, traverse and keep only the marked items */
    numEntries = 0;
    NodeHandle curr, prev;
    for (uint32_t i=0; i<PRIMES[sizeIndex]; i++) {
        prev = 0;
        curr = table[i];
        while (curr) {
            if (parent->getNode(level, table[0]).isMarked()) {
                if (prev) {
                    parent->setNodeNext(level, prev, curr);
                } else {
                    table[i] = curr;
                }
                numEntries++;
                prev = curr;
            }
            curr = parent->getNodeNext(level, curr);
        }
        /* Done traversing; null terminate the new chain */
        if (prev) {
            parent->setNodeNext(level, prev, 0);
        } else {
            table[i] = 0;
        }
    }
    /* Check if we should shrink the table. TBD */
}

void UniqueTable::SubTable::expand()
{
    // Check if we can enlarge
    if (PRIMES[sizeIndex] >= UINT32_MAX) {  // MAX of uint32
        std::cout << "[BRAVE_DD] ERROR!\t Unable to enlarge SubUniqueTable!"
        << "\n\t\tToo many nodes at level: " << level << std::endl;
        exit(0);
    }
    /* Enlarge */
    // table to list, waiting for realloc
    NodeHandle front = 0, chain = 0;
    for (uint32_t i=0; i<PRIMES[sizeIndex]; i++) {
        while (table[i]) {
            chain = table[i];
            table[i] = parent->getNodeNext(level, chain);
            parent->setNodeNext(level, chain, front);
            front = chain;
        }
    }
    numEntries = 0;
    // new size
    sizeIndex++;
    uint32_t newSize = 0;
    if (PRIMES[sizeIndex] > UINT32_MAX) {
        newSize = UINT32_MAX;
    } else {
        newSize = PRIMES[sizeIndex];
    }
    // new table of larger size
    table = (NodeHandle*)realloc(table, newSize * sizeof(NodeHandle));
    if (!table) {
        std::cout << "[BRAVE_DD] ERROR!\t Realloc fail in expand subtable!"<< std::endl;
        exit(0);
    }
    for (uint32_t i=0; i<newSize; i++) {
        table[i] = 0;
    }
    // rehash
    NodeHandle next;
    uint32_t newIndex;
    while (front) {
        // save next, before we overwrite it
        next = parent->getNodeNext(level, front);
        // compute new hash and get new index
        newIndex = parent->getNodeHash(level, front) % newSize;
        // add to the front of the new list
        parent->setNodeNext(level, front, table[newIndex]);
        table[newIndex] = front;
        // advance
        front = next;
        numEntries++;
    }
}
// ******************************************************************
// *                                                                *
// *                                                                *
// *                      UniqueTable methods                       *
// *                                                                *
// *                                                                *
// ******************************************************************

UniqueTable::UniqueTable(Forest* f):parent(f)
{
    uint16_t lvls = f->getSetting().getNumVars();
    tables = (SubTable*)malloc(lvls * sizeof(SubTable));
    if (!tables) {
        std::cout << "[BRAVE_DD] ERROR!\t Unable to construct UniqueTable!"<< std::endl;
        exit(0);
    }
    for (uint16_t i=0; i<lvls; i++) {
        new (&tables[i]) SubTable(i+1, f);
    }
}
UniqueTable::~UniqueTable()
{
    for (uint16_t i=0; i<parent->getSetting().getNumVars(); i++) {
        tables[i].~SubTable();
    }
    free(tables);
    parent = 0;
}

