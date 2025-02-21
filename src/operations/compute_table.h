#ifndef REXBDD_COMPUTE_TABLE_H
#define REXBDD_COMPUTE_TABLE_H

#include "../defines.h"
#include "../forest.h"
#include "../hash_stream.h"

namespace REXBDD {
    class CacheEntry;
    class ComputeTable;
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                      CacheEntry class                          *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::CacheEntry {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    CacheEntry() {
        lvl = 0;
        keySize = 2;
        key = std::vector<Edge>(keySize);
        isInUse = 0;
    }
    CacheEntry(const uint16_t level, const Edge& a) {
        lvl = level;
        keySize = 1;
        key = std::vector<Edge>(keySize);
        key[0] = a;
        isInUse = 0;
    }
    CacheEntry(const uint16_t level, const Edge& a, const Edge& b) {
        lvl = level;
        keySize = 2;
        key = std::vector<Edge>(keySize);
        key[0] = a;
        key[1] = b;
        isInUse = 0;
    }

    inline void setResult(const Edge& r) {
        res = r;
        // only be in use when the result is set
        isInUse = 1;
    }

    inline uint64_t hash() const {
        hash_stream hs;
        hs.start(0);
        // push info
        hs.push(lvl);
        for (char i=0; i<keySize; i++) {
            hs.push(key[i].getEdgeHandle());
        }
        // for edge valued, TBD
        return (uint64_t)hs.finish64();
    }

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    friend class ComputeTable;

    inline bool equals(const CacheEntry& e) const {
        if (lvl != e.lvl) return 0;
        for (char i=0; i<keySize; i++) {
            if (key[i] != e.key[i]) return 0;
        }
        return 1;
    }


    std::vector<Edge>   key;
    Edge                res;
    uint16_t            lvl;
    char                keySize;
    bool                isInUse;
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                     ComputeTable class                         *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::ComputeTable {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    ComputeTable();
    ~ComputeTable();

    bool check(const uint16_t lvl, const Edge& a, Edge& ans);
    bool check(const uint16_t lvl, const Edge& a, const Edge& b, Edge& ans);

    void add(const uint16_t lvl, const Edge& a, const Edge& ans);
    void add(const uint16_t lvl, const Edge& a, const Edge& b, const Edge& ans);

    void sweep();

    void reportStat(std::ostream& out, int format=0) const;

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /**
     * @brief This will enlarge the table and rehash the old data with the new size
     * 
     */
    void enlarge(uint64_t newSize);

    std::vector<CacheEntry>     table;
    uint64_t                    numEnries;
    int                         sizeIndex;

    uint64_t                    countHits;
    uint64_t                    countOverwrite;

};

#endif