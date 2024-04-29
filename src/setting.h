#ifndef REXBDD_SETTING_H
#define REXBDD_SETTING_H

#include "defines.h"
#include <vector>

namespace REXBDD {
    /*
     *  User-cared specifications of forest
    */
    ///  Range type
    typedef enum {
        BOOLEAN,            // {0, 1}
        FINITE1,            // {0, 1, 2, ..., N-1}, N is determined in Range
        FINITE,             // {0, 1, 2, ..., N-1, N}, N is determined in Range
        NATURAL,            // {0, 1, 2, ...}
        INTEGER,            // {..., -2, -1, 0, 1, 2, ...}
        NNREAL,             // [0, +∞)
        REAL                // (-∞, +∞)
    } RangeType;
    /// Encoding mechanism
    typedef enum {
        TERMINAL,           // only if range type is Boolean:       BDDs, MTBDDs
        EDGE_PLUS,          // only if range type is not Boolean:   EV+BDDs
        EDGE_PLUSMOD,       // only if range type is FINITE*:       EV%BDDs
        EDGE_MULT           // only if range type is *REAL*:        EV*BDDs
    } EncodeMechanism;
    ///  Reduction type
    typedef enum {
        QUASI,              // -+
        FULLY,              //  +---- only applicable if variable dimension is 1
        REX,                // -+
        QUASI_QUASI,        // -+
        FULLY_FULLY,        //  +---- only applicable if variable dimension is 2
        FULLY_IDENTITY,     // -+
        USER_DEFINED        //        only if dimension is 1: user-defined combinations of reductions
    } ReductionType;
    /// Swap flag type
    typedef enum {
        NO_SWAP,            // No swap flags are used
        ONE,                // Swap-one flags are used
        ALL,                // Swap-all flags are used (placeholder)
        FROM,               // Swap flags are used only for unprimed variables
        TO,                 // Swap flags are used only for primed variables
        FROM_TO             // Swap flags are used for both unprimed and primed
    } SwapSet;
    /// Complement flag type
    typedef enum {
        NO_COMP,            // No complement flags are used
        COMP,               // Complement flags are used
    } CompSet;

    //--------------------------+---------------------------+-----------------|
    // EL0    AL0 |  EL1    AL1 |  EH0    AH0 |  EH1    AH1 |  I0    X    I1
    //------------+-------------+-------------+-------------+-----------------|
    // 0      0   |  0      0   |  0      0   |  0      0   |  1     1    1
    // 0      0   |  0      0   |  1      1   |  1      1   |  0     0    0
    // 0      0   |  1      1   |  0      0   |  1      1   |  0     0    1
    // 0      1   |  0      1   |  0      1   |  0      1   |  0     1    0
    //------------+-------------+-------------+-------------+-----------------|
    // 0      1   |  2      3   |  4      5   |  6      7   |  8     9    10
    //--------------------------+---------------------------+-----------------|
    // Note:
    //      check X: rule == 1001; 
    //  otherwise
    //      check relation: rule > 7;
    //      check (?)1: rule & (0010);
    //  Complement rule:
    //      rule == X ? X :
    //          (rule & (0010)) ? (rule & (1101)) : (rule | (0010));
    //  Swap rule:
    //      rule >7 ? rule :
    //          (rule & (0010)) ? (rule & (1011)) : (rule | (0100));

    /// Reduction rule
    typedef enum {
        EL0 = 1,
        EL1 = 2,
        EH0 = 4,
        EH1 = 6,
        AL0 = 1,
        AL1 = 3,
        AH0 = 5,
        AH1 = 7,
        X   = 9,
        I0  = 8,
        I1  = 10
    } ReductionRule;
    
    /// Domain and ordering of variables
    class VarDomain;
    /// Range type and range size if FINITE*
    class Range;
    /// Reduction type and set of rules
    class Reductions;

    /// 
    class ForestSetting;
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         VarDomain class                        *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::VarDomain {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        VarDomain(unsigned size);
        VarDomain(const int* order, int size );
        VarDomain(const VarDomain& dom);
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        unsigned            maxLevel;   // number of variables
        std::vector<int>    level2Var;  // Variable order: indexed by levels
        std::vector<int>    var2Level;  // Variable order: indexed by variables
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                           Range class                          *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Range {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        Range();
        Range(const RangeType type, int size );
        Range(const Range& dom);
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        RangeType       type;
        int             maxRange;       // used only if type is FINITE*
        bool            hasNegInf;      // 1 if -∞ is allowed
        bool            hasPosInf;      // 1 if +∞ is allowed
        bool            hasUnDef;       // 1 if UnDef is allowed
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         Reductions class                       *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Reductions {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        Reductions();
        Reductions(const ReductionType t);
        Reductions(const ReductionRule* rs);
        Reductions(const Reductions& reduction);
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        int             size;       // number of allowed reduction rules
        ReductionType   type;       // type of reductions
        ReductionRule*  rules;      // set of allowed reduction rules
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                     ForestSetting class                        *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::ForestSetting {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        ForestSetting();
        ~ForestSetting();
        //******************************************
        //  Getters for the type
        //******************************************
        

        //******************************************
        //  Setters for the type
        //******************************************
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        int             dimension;      // 1: set/vector function; 2: relation/matrix function
        VarDomain       domain;         // Variable domain: number of variables, variable order
        Range           rangeType;      // Range type and special values
        EncodeMechanism encodingType;   // Encoding mechanism: terminal, edge-valued
        Reductions      reductions;     // Reduction: number of rules, set of rules
        SwapSet         swapType;       // Swap flag type
        CompSet         compType;       // Complement flag type
};


#endif