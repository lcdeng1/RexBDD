#ifndef REXBDD_EDGE_FLAGS_H
#define REXBDD_EDGE_FLAGS_H

#include "../defines.h"

namespace REXBDD {
    /// Swap flag type
    typedef enum {
        NO_SWAP,            // No swap flags are used
        ONE,                // Swap-one flags are used
        ALL,                // Swap-all flags are used (placeholder)
        FROM,               // Swap flags are applied only for unprimed variables
        TO,                 // Swap flags are applied only for primed variables
        FROM_TO             // Swap flags are applied for both unprimed and primed
    } SwapSet;
    static inline std::string swapSet2String(SwapSet st) {
        std::string swapType;
        if (st == NO_SWAP) swapType = "No";
        else if (st == ONE) swapType = "Yes: Swap-one";
        else if (st == ALL) swapType = "Yes: Swap-all";
        else if (st == FROM) swapType = "Yes: Swap-from";
        else if (st == TO) swapType = "Yes: Swap-to";
        else if (st == FROM_TO) swapType = "Yes: Swap-from_to";
        else swapType = "Unknown";
        return swapType;
    }
    /// Complement flag type
    typedef enum {
        NO_COMP,            // No complement flags are used
        COMP                // Complement flags are used
        // ^ For FINITE range, N - value; for INTEGER and REAL, -value; for NNINTEGER/NNREAL, TBD.
    } CompSet;
    static inline std::string compSet2String(CompSet ct) {
        std::string compType;
        if (ct==NO_COMP) compType = "No";
        else if (ct==COMP) compType = "Yes";
        else compType = "Unknown";
        return compType;
    }
    /// Edge flags type
    class Flags;
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         Edge Flags class                       *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Flags {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Flags(SwapSet swap, CompSet comp);
    ~Flags();
    //******************************************
    //  Getters
    //******************************************
    /* Swap */
    /// Get the type of swap flag applied
    inline SwapSet getSwapType() const {return swapType;}
    /* Complement */
    /// Get the type of complement flag applied
    inline CompSet getCompType() const {return compType;}
    //******************************************
    //  Setters
    //******************************************
    /// Set the type of swap flag applied
    inline void setSwapType(const SwapSet swap) {swapType = swap;}
    /// Set the type of complement flag applied 
    inline void setCompType(const CompSet comp) {compType = comp;}

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    SwapSet         swapType;       // Swap flag type
    CompSet         compType;       // Complement flag type
};


#endif