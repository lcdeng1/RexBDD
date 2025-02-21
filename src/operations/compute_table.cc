#include "compute_table.h"

// #define REXBDD_CACHE_TRACE

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                     ComputeTable methods                       *
// *                                                                *
// *                                                                *
// ******************************************************************

ComputeTable::ComputeTable()
{
    sizeIndex = 0;
    numEnries = 0;
    table = std::vector<CacheEntry>(PRIMES[sizeIndex], CacheEntry());
    countHits = 0;
    countOverwrite = 0;
}
ComputeTable::~ComputeTable()
{
    table.clear();
}

bool ComputeTable::check(const uint16_t lvl, const Edge& a, Edge& ans)
{
    CacheEntry entry(lvl, a);
    uint64_t size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
    uint64_t id = entry.hash() % size;
    if (table[id].isInUse) {
        /* Valid entry, then check if match */
        if ((table[id].lvl == lvl) && (table[id].keySize == 1)
            && (table[id].key[0] == a)) {
            countHits++;
            ans = table[id].res;
            return 1;
        }
    }
    /* Not cached */
    return 0;
}

bool ComputeTable::check(const uint16_t lvl, const Edge& a, const Edge& b, Edge& ans)
{
    CacheEntry entry(lvl, a, b);
    uint64_t size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
    uint64_t id = entry.hash() % size;
#ifdef REXBDD_CACHE_TRACE
    std::cout << "checking in cache, id = " << id << "; size = " << size << std::endl;
#endif
    if (table[id].isInUse) {
        /* Valid entry, then check if match */
        if ((table[id].lvl == lvl) && (table[id].keySize == 2)
            && (table[id].key[0] == a) && (table[id].key[1] == b)) {
            countHits++;
            ans = table[id].res;
            return 1;
        }
    }
    /* Not cached */
    return 0;
}

void ComputeTable::add(const uint16_t lvl, const Edge& a, const Edge& ans)
{
    /* Check if we should enlage the table when  */
    uint64_t size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
    if ((numEnries > (size / 1.5)) && (numEnries < (uint64_t)0x01 << 60)) {
        sizeIndex++;
        size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
        enlarge(size);
    }
    CacheEntry entry(lvl, a);
    uint64_t id = entry.hash() % size;
    /* new entry */
    if (!table[id].isInUse) {
        numEnries++;
    } else {
        countOverwrite++;
    }
    /* overwrite */
    entry.setResult(ans);
    table[id] = entry;
#ifdef REXBDD_CACHE_TRACE
    std::cout << "entry ID = " << id << ": a: ";
    entry.key[0].print(std::cout);
    std::cout << " ans: ";
    entry.res.print(std::cout);
    std::cout << std::endl;
    std::cout << "table entry: a: ";
    table[id].key[0].print(std::cout);
    std::cout << " ans: ";
    table[id].res.print(std::cout);
    std::cout << std::endl;
#endif
}
void ComputeTable::add(const uint16_t lvl, const Edge& a, const Edge& b, const Edge& ans)
{
#ifdef REXBDD_CACHE_TRACE
    std::cout << "add entry lvl = " << lvl << ": a: ";
    a.print(std::cout);
    std::cout << " b: ";
    b.print(std::cout);
    std::cout << " ans: ";
    ans.print(std::cout);
    std::cout << std::endl;
#endif
    /* Check if we should enlage the table when  */
    uint64_t size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
    if ((numEnries > (size / 1.5)) && (numEnries < (uint64_t)0x01 << 60)) {
#ifdef REXBDD_CACHE_TRACE
    std::cout << "enlarge table: entries = " << numEnries << ", size = " << size << std::endl;
#endif
        sizeIndex++;
        size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
        enlarge(size);
    }
    CacheEntry entry(lvl, a, b);
#ifdef REXBDD_CACHE_TRACE
    std::cout << "compute hash\n";
#endif
    uint64_t id = entry.hash() % size;
#ifdef REXBDD_CACHE_TRACE
    std::cout << "compute hash done\n";
#endif
    /* new entry */
    if (!table[id].isInUse) {
        numEnries++;
    } else {
        countOverwrite++;
    }
    /* overwrite */
    entry.setResult(ans);
    table[id] = entry;
#ifdef REXBDD_CACHE_TRACE
    std::cout << "entry ID = " << id << ": a: ";
    entry.key[0].print(std::cout);
    std::cout << " b: ";
    entry.key[1].print(std::cout);
    std::cout << " ans: ";
    entry.res.print(std::cout);
    std::cout << std::endl;
    std::cout << "table entry: a: ";
    table[id].key[0].print(std::cout);
    std::cout << " b: ";
    table[id].key[1].print(std::cout);
    std::cout << " ans: ";
    table[id].res.print(std::cout);
    std::cout << std::endl;
#endif
}

void ComputeTable::sweep()
{
    //
}

void ComputeTable::reportStat(std::ostream& out, int format) const
{
    if (format == 0) {
        uint64_t size = PRIMES[sizeIndex] ? PRIMES[sizeIndex] : ((uint64_t)0x01 << 60);
        out << "Computing Table Statistics: \n";
        out << "Size: \t\t" << size << "\n";
        out << "Ents: \t\t" << numEnries << "\n";
        out << "Hits: \t\t" << countHits << "\n";
        out << "OWs:  \t\t" << countOverwrite << "\n";
    }
}

void ComputeTable::enlarge(uint64_t newSize)
{
    // copy the old table
    std::vector<CacheEntry> oldTable = table;
    // resize the table
    table.resize(newSize);
    // rehash
    for (size_t i=0; i<oldTable.size(); i++) {
        if (oldTable[i].isInUse) {
            table[oldTable[i].hash() % newSize] = oldTable[i];
        }
    }
    oldTable.clear();
}
