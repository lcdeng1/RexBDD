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
    /// Value type
    //      Note: used only for EVBDDs different value range and precision
    typedef enum {
        VOID,               //  Nothing, for BDDs without values on edges
        INT,                //  Integer
        LONG,               //  Long
        FLOAT,              //  Float
        DOUBLE              //  Double
    } ValueType;
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
        FROM,               // Swap flags are applied only for unprimed variables
        TO,                 // Swap flags are applied only for primed variables
        FROM_TO             // Swap flags are applied for both unprimed and primed
    } SwapSet;
    /// Complement flag type
    typedef enum {
        NO_COMP,            // No complement flags are used
        COMP                // Complement flags are used
    } CompSet;
    /// Merge type
    typedef enum {
        PUSH_UP,
        PUSH_DOWN
    } MergeType;

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
        I1  = 10,
        INVALID_RULE = -1   // the end of the reduction rules set
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
        VarDomain(const unsigned* order, unsigned size );
        VarDomain(const VarDomain& dom);
        /**
         * Getters
         */
        /// Get the max level (number of variables)
        inline unsigned getNumVars() const {return maxLevel;}
        /// Get the variable index for a given level
        inline unsigned getVar(unsigned lvl) const {return level2Var[lvl];}
        /// Get the level for a given variable index
        inline unsigned getLevel(unsigned var) const {return var2Level[var];}
        /**
         * Setters
         */


    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        unsigned                maxLevel;   // number of variables
        std::vector<unsigned>   level2Var;  // Variable order: indexed by levels
        std::vector<unsigned>   var2Level;  // Variable order: indexed by variables
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
        //******************************************
        //  Getters
        //******************************************
        inline RangeType getRangeType() const {return rangeType;}
        inline ValueType getValType() const {return valueType;}
        inline unsigned getMaxRange() const {return maxRange;}
        inline unsigned getMinRange() const {return minRange;}
        inline bool hasNegInf() const {return negInf;}
        inline bool hasPosInf() const {return posInf;}
        inline bool hasUnDef() const {return unDef;}
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        RangeType       rangeType;
        ValueType       valueType;
        unsigned        maxRange;       // the max range limit if type is FINITE*
        unsigned        minRange;       // the min range limit
        bool            negInf;         // 1 if -∞ is allowed
        bool            posInf;         // 1 if +∞ is allowed
        bool            unDef;          // 1 if UnDef is allowed
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
        //******************************************
        //  Getters
        //******************************************
        /// Get the type of reduction
        inline ReductionType getType() const {return type;}
        /// Get the size of reduction rules set
        inline int getSize() const {return size;}
        /// Check if the given ReductionRule has been set
        inline bool hasRule(ReductionRule rule) const {
            bool res = 0;
            for (int i=0; i<size; i++) {
                if (rules[i] == rule) res = 1;
            }
            return res;
        };
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        int             size;       // number of allowed reduction rules
        ReductionType   type;       // type of reductions
        ReductionRule*  rules;      // set of allowed reduction rules, end with -1
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
        /* Dimension */
        /// Get the dimension
        inline int getDim() const {return dimension;}
        /* Domain */
        /// Get the number of variables
        inline unsigned getNumVars() const {return domain.getNumVars();}
        /// Get the variable index for a given level
        inline unsigned getVar(unsigned lvl) const {return domain.getVar(lvl);}
        /// Get the level for a given variable index
        inline unsigned getLevel(unsigned var) const {return domain.getLevel(var);}
        /* Range */
        /// Get the RangeType 
        inline RangeType getRangeType() const {return range.getRangeType();}
        /// Get the ValueType 
        inline ValueType getValType() const {return range.getValType();}
        /// Get the max range limit that has been set
        inline unsigned getMaxRange() const {return range.getMaxRange();}
        /// Get the min range limit that has been set
        inline unsigned getMinRange() const {return range.getMinRange();}
        /// Check if the negative infinity has been set
        inline bool hasNegInf() const {return range.hasNegInf();}
        /// Check if the positive infinity has been set
        inline bool hasPosInf() const {return range.hasPosInf();}
        /// Check if the undefine value has been set
        inline bool hasUnDef() const {return range.hasUnDef();}
        /* Encoding */
        /// Get the encoding mechanism
        inline EncodeMechanism getEncodeMechanism() const {return encodingType;}
        /* Reduction */
        /// Get the reduction type
        inline ReductionType getReductionType() const {return reductions.getType();}
        /// Get the size of reduction rules applied
        inline int getReductionSize() const {return reductions.getSize();}
        /// Check if the given ReductionRule is applied 
        inline bool hasReductionRule(ReductionRule rule) const {return reductions.hasRule(rule);}
        /* Swap */
        /// Get the type of swap flag applied
        inline SwapSet getSwapType() const {return swapType;}
        /* Complement */
        /// Get the type of complement flag applied
        inline CompSet getCompType() const {return compType;}
        /* Merge */
        /// Get the type of merge mechanism applied
        inline MergeType getMergeType() const {return mergeType;}

        //******************************************
        //  Setters for the type
        //******************************************
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        int             dimension;      // 1: set/vector function; 2: relation/matrix function
        VarDomain       domain;         // Variable domain: number of variables, variable order
        Range           range;          // Range type and special values
        EncodeMechanism encodingType;   // Encoding mechanism: terminal, edge-valued
        Reductions      reductions;     // Reduction: number of rules, set of rules
        SwapSet         swapType;       // Swap flag type
        CompSet         compType;       // Complement flag type
        MergeType       mergeType;      // Merge type
};

#endif