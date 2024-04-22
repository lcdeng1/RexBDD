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
        FINITE,             // {0, 1, 2, ..., N-1}, N is determined in Range
        FINITE_POSINF,      // {0, 1, 2, ..., N-1, N} U {+∞}, N is determined in Range
        FINITE_NEGPOSINF,   // {0, 1, 2, ..., N-1, N} U {+∞, -∞}, N is determined in Range
        NATURAL,            // {0, 1, 2, ...}
        NATURAL_POSINF,     // {0, 1, 2, ...} U {+∞}
        INTEGER,            // {..., -2, -1, 0, 1, 2, ...}
        INTEGER_POSINF,     // {..., -2, -1, 0, 1, 2, ...} U {+∞}
        INTEGER_NEGPOSINF,  // {..., -2, -1, 0, 1, 2, ...} U {-∞, +∞}
        NNREAL,             // [0, +∞)
        NNREAL_POSINF,      // [0, +∞]
        REAL,               // (-∞, +∞)
        REAL_NEGINF,        // [-∞, +∞)
        REAL_POSINF,        // (-∞, +∞]
        REAL_NEGPOSINF      // [-∞, +∞]
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
        NAIVE,              // No swap flags are used
        ONE,                // Swap-one flags are used
        ALL,                // Swap-all flags are used (placeholder)
        FROM,               // Swap flags are used only for unprimed variables
        TO,                 // Swap flags are used only for primed variables
        FROM_TO             // Swap flags are used for both unprimed and primed
    } SwapSet;
    /// Complement flag type
    typedef enum {
        NAIVE,              // No complement flags are used
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
        unsigned            maxLevel;
        std::vector<int>    level2Var;
        std::vector<int>    var2Level;
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
        int             size;
        ReductionType   type;
        ReductionRule*  rules;
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
        int             dimension;
        VarDomain       domain;
        Range           rangeType;
        EncodeMechanism encodingType;
        Reductions      reductions;
        SwapSet         swapType;
        CompSet         compType;
};


#endif