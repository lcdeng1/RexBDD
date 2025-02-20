#include "node_manager.h"
#include "forest.h"

// #define REXBDD_NM_TRACE

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                       SubManager methods                       *
// *                                                                *
// *                                                                *
// ******************************************************************

NodeManager::SubManager::SubManager(Forest *f):parent(f)
{
    sizeIndex = 0;
    nodes = (Node*)malloc((PRIMES[sizeIndex] + 1) * sizeof(Node));
    if (!nodes) {
        std::cout << "[REXBDD] ERROR!\t Malloc fail for submanager!"<< std::endl;
        exit(0);
    }
    recycled = 0;
    firstUnalloc = 1;
    freeList = 0;
    numFrees = PRIMES[sizeIndex];
}
NodeManager::SubManager::~SubManager()
{
    for (uint32_t i=1; i<firstUnalloc; i++) {
        nodes[i].~Node();
    }
    free(nodes);
}

NodeHandle NodeManager::SubManager::getFreeNodeHandle(const Node& node)
{
    /* Re-use the recycled handle, if we have one */
    if (recycled) {
        const NodeHandle h = recycled;
        recycled = 0;
        nodes[h].assign(node, parent->nodeSize);
        return h;
    }
    /* Enlarge if there is no free/unused slots */
    if (!numFrees) expand();
    /* There are definitely free slots. Pull form there, if the free list is not empty */
    numFrees--;
    if (freeList) {
        // pull from the free list
        NodeHandle h = freeList;
        freeList = nodes[h].nextFree();
        nodes[h].assign(node, parent->nodeSize);
        return h;
    }
    /* Free list is empty, so pull from the unallocated end portion */
    new (&nodes[firstUnalloc]) Node(parent->nodeSize);
    nodes[firstUnalloc].assign(node, parent->nodeSize);
    return firstUnalloc++;
}

Node& NodeManager::SubManager::getNodeFromHandle(const NodeHandle h)
{
    if (h>=firstUnalloc) {
        std::cout << "[REXBDD] ERROR!\t Invalid handle in node submanager; " 
        << firstUnalloc-1 << "nodes are allocated" << std::endl;
        exit(0);
    }
    return nodes[h];
}

void NodeManager::SubManager::expand()
{
    // Check if we can enlarge
    if (PRIMES[sizeIndex] >= UINT32_MAX) {  // MAX of uint32
        std::cout << "[REXBDD] ERROR!\t Unable to enlarge node submanager!" << std::endl;
        exit(0);
    }
    // Enlarge
    sizeIndex++;
    uint32_t newSize = 0;
    if (PRIMES[sizeIndex] > UINT32_MAX) {
        newSize = UINT32_MAX + 1;
    } else {
        newSize = PRIMES[sizeIndex] + 1;
    }
    nodes = (Node*)realloc(nodes, newSize * sizeof(Node));
    numFrees += (newSize - PRIMES[sizeIndex-1] - 1);
}

void NodeManager::SubManager::shrink()
{
    sizeIndex--;
    uint32_t newSize = PRIMES[sizeIndex] + 1;
    nodes = (Node*)realloc(nodes, newSize * sizeof(Node));
    numFrees -= (PRIMES[sizeIndex+1] + 1 - newSize);
}

void NodeManager::SubManager::sweep()
{
    if (!nodes) return;
    /* Expand the unallocated portion as much as we  can */
    while (firstUnalloc) {
        if ((nodes+firstUnalloc-1)->isMarked()) {
            break;
        }
        (nodes+firstUnalloc-1)->~Node();
        firstUnalloc--;
    }
    numFrees = ((PRIMES[sizeIndex]>UINT32_MAX)? UINT32_MAX:PRIMES[sizeIndex]) + 1 - firstUnalloc;
    /* Check if we can shrink */
    if (firstUnalloc < PRIMES[sizeIndex-1]) {
        shrink();
    }
    /* Rebuild the free list, by scanning all nodes backwards.
       Unmarked nodes are added to the list. */
    freeList = 0;
    for (uint32_t i=firstUnalloc; i>1; --i) {
        if (nodes[i-1].isMarked()) {
            nodes[i-1].unmark();
        } else {
            nodes[i-1].recycle(freeList);
            freeList = i-1;
            numFrees++;
        }
    }
}
// ******************************************************************
// *                                                                *
// *                                                                *
// *                      NodeManager methods                       *
// *                                                                *
// *                                                                *
// ******************************************************************

NodeManager::NodeManager(Forest *f):parent(f)
{
    uint16_t lvls = f->getSetting().getNumVars();
    chunks = (SubManager*)malloc(lvls * sizeof(SubManager));
    for (uint32_t i=0; i<lvls; i++) {
        new (&chunks[i]) SubManager(f);
    }
}
NodeManager::~NodeManager()
{
    for (uint16_t i=0; i<parent->getSetting().getNumVars(); i++) {
        chunks[i].~SubManager();
    }
    free(chunks);
    parent = 0;
}

void NodeManager::sweep(uint16_t lvl)
{
    chunks[lvl-1].sweep();
}

void NodeManager::sweep()
{
    for (uint16_t k=0; k<parent->getSetting().getNumVars(); k++) {
        sweep(k);
    }
}

void NodeManager::unmark(uint16_t lvl)
{
#ifdef REXBDD_NM_TRACE
    std::cout << "unmark: unmark lvl = " << lvl << std::endl;
    std::cout << "\tfirstUnalloc = " << chunks[lvl-1].firstUnalloc << "; size = " << PRIMES[chunks[lvl-1].sizeIndex] << std::endl;
#endif
    for (uint32_t i=1; i<chunks[lvl-1].firstUnalloc; i++) {
        if (chunks[lvl-1].nodes[i].isMarked()) {
            chunks[lvl-1].nodes[i].unmark();
        }
    }
}

void NodeManager::unmark()
{
    for (uint16_t k=1; k<=parent->getSetting().getNumVars(); k++) {
        unmark(k);
    }
}