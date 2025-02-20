#ifndef REXBDD_SETTING_H
#define REXBDD_SETTING_H

#include "defines.h"
#include "settings/range.h"
#include "settings/reductions.h"
#include "settings/edge_flags.h"

namespace REXBDD {
    /// Predefined BDD type
    enum class PredefForest {
        REXBDD,
        QBDD,
        FBDD,
        ZBDD,
        ESRBDD,
        FBMXD,
        IBMXD,
        ESRBMXD
    };
    /// Encoding mechanism
    enum EncodeMechanism{
        TERMINAL,           // only if range type is Boolean:       BDDs, MTBDDs
        EDGE_PLUS,          // only if range type is not Boolean:   EV+BDDs
        EDGE_PLUSMOD,       // only if range type is FINITE*:       EV%BDDs
        EDGE_MULT           // only if range type is *REAL*:        EV*BDDs
    };
    static inline std::string encodeMechanism2String(EncodeMechanism em) {
        std::string emType;
        if (em == TERMINAL) {
            emType = "Terminal";
        } else if (em == EDGE_PLUS) {
            emType = "Edge plus (EV+)";
        } else if (em == EDGE_PLUSMOD) {
            emType = "Edge plus mod (EV%)";
        } else if (em == EDGE_MULT) {
            emType = "Edge multiply (EV*)";
        } else {
            emType = "Unknown";
        }
        return emType;
    }
    /// Merge type (this will be removed in the future.)
    enum MergeType{
        NO_MERGE,           // for quasi type bdd/bmxd
        PUSH_UP,            // both ShortenI and ShortenX only for MXDs with reduction rule I and X
        PUSH_DOWN,          // neither ShortenI nor ShortenX only for MXDs with reduction rule I and X
        SHORTEN_I,          // only ShortenI for MXDs with reduction rule I and X
        SHORTEN_X           // only ShortenX for MXDs with reduction rule I and X
    };
    static inline std::string mergeType2String(MergeType mt, bool isRel) {
        std::string mType;
        if (mt == PUSH_UP) {
            mType = (isRel)?"ShortenX: true; ShortenI: true":"Push up";
        } else if (mt == PUSH_DOWN) {
            mType = (isRel)?"ShortenX: false; ShortenI: false":"Push down";
        } else if (mt == SHORTEN_I) {
            mType = (isRel)?"ShortenX: false; ShortenI: true":"Unknown";
        } else if (mt == SHORTEN_X) {
            mType = (isRel)?"ShortenX: true; ShortenI: false":"Unknown";
        } else if (mt == NO_MERGE) {
            mType = "No needed";
        } else {
            mType = "Unknown";
        }
        return mType;
    }
    /// Domain and ordering of variables
    class VarDomain;
    /// Settings for forest
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
        VarDomain(const uint16_t size);
        VarDomain(const std::vector<uint16_t>& order, uint16_t size);
        ~VarDomain();
        /**
         * Getters
         */
        /// Get the max level (number of variables)
        inline uint16_t getNumVars() const {return maxLevel;}
        /// Get the variable index for a given level
        inline uint16_t getVar(uint16_t lvl) const {return level2Var[lvl];}
        /// Get the level for a given variable index
        inline uint16_t getLevel(uint16_t var) const {return var2Level[var];}
        /**
         * Setters
         */
        /// Set the number of variables
        inline void setNumVars(const uint16_t num) {maxLevel = num;}
        // ordering TBD


    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        uint16_t                maxLevel;   // number of variables
        std::vector<uint16_t>   level2Var;  // Variable order: indexed by levels
        std::vector<uint16_t>   var2Level;  // Variable order: indexed by variables
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                     ForestSetting class                        *
// *                                                                *
// *                                                                *
// ******************************************************************
/*
*  User-cared specifications of forest
*/
class REXBDD::ForestSetting {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        /**
         * @brief Construct a new ForestSetting object with the given number of variables.
         * 
         * @param numVals       The given number of variables.
         */
        ForestSetting(const unsigned numVals);
        ForestSetting(const PredefForest type, const unsigned numVals, const long maxRange=1);
        ForestSetting(const std::string& bdd, const unsigned numVals, const long maxRange=1);
        ~ForestSetting();
        //******************************************
        //  Getters for the type
        //******************************************
        /* Domain =======================================================================*/
        /// Get the number of variables
        inline uint16_t getNumVars() const {return domain.getNumVars();}
        /// Get the variable index for a given level
        inline uint16_t getVar(uint16_t lvl) const {return domain.getVar(lvl);}
        /// Get the level for a given variable index
        inline uint16_t getLevel(uint16_t var) const {return domain.getLevel(var);}
        /* Range ========================================================================*/
        /// Get the RangeType 
        inline RangeType getRangeType() const {return range.getRangeType();}
        /// Get the ValueType 
        inline ValueType getValType() const {return range.getValType();}
        /// Get the max range limit that has been set
        inline unsigned long getMaxRange() const {return range.getMaxRange();}
        /// Check if the negative infinity has been set
        inline bool hasNegInf() const {return range.hasNegInf();}
        /// Check if the positive infinity has been set
        inline bool hasPosInf() const {return range.hasPosInf();}
        /// Check if the undefine value has been set
        inline bool hasUnDef() const {return range.hasUnDef();}
        /* Encoding =====================================================================*/
        /// Get the encoding mechanism
        inline EncodeMechanism getEncodeMechanism() const {return encodingType;}
        /* Reduction ====================================================================*/
        inline bool isRelation() const {return reductions.getDim() > 1;}
        /// Get the reduction type
        inline ReductionType getReductionType() const {return reductions.getType();}
        /// Get the size of reduction rules applied
        inline int getReductionSize() const {return reductions.getNumRules();}
        /// Check if the given ReductionRule is applied 
        inline bool hasReductionRule(ReductionRule rule) const {return reductions.hasRule(rule);}
        /* Swap =========================================================================*/
        /// Get the type of swap flag applied
        inline SwapSet getSwapType() const {return flags.getSwapType();}
        /* Complement ===================================================================*/
        /// Get the type of complement flag applied
        inline CompSet getCompType() const {return flags.getCompType();}
        /* Merge ========================================================================*/
        /// Get the type of merge mechanism applied (will be removed later)
        inline MergeType getMergeType() const {return mergeType;}
        /* Name =========================================================================*/
        inline std::string getName() const {return name;}

        //******************************************
        //  Setters for the type
        //******************************************
        /* Domain */
        /// Set the number of variables
        inline void setNumVars(const uint16_t num) {domain.setNumVars(num);}
        // ordering TBD
        /* Range */
        /// Set the range type
        inline void setRangeType(const RangeType type) {range.setRangeType(type);}
        /// Set the value type
        inline void setValType(const ValueType type) {range.setValType(type);}
        /// Set the max range
        inline void setMaxRange(const unsigned long max) {range.setMaxRange(max);}
        /// Set the negative infinity
        inline void setNegInf(const bool nif) {range.setNedInf(nif);}
        /// Set the positive infinity 
        inline void setPosInf(const bool pif) {range.setPosInf(pif);}
        /// Set the undefined 
        inline void setUnDef(const bool udf) {range.setUnDef(udf);}
        /* Encoding */
        /// Set the encoding mechanism
        inline void setEncodeMechanism(const EncodeMechanism type) {encodingType = type;}
        /* Reduction */
        /// Set the dimension
        inline void setDim(const int dim) {reductions.setDim(dim);}
        /// Set the reduction type
        inline void setReductionType(const ReductionType type) {reductions.setType(type);}
        /// Add reduction rule
        inline void addReductionRule(const ReductionRule rule) {reductions.addRule(rule);}
        /// Delete reduction rule
        inline void delReductionRule(const ReductionRule rule) {reductions.delRule(rule);}
        /* Swap */
        /// Set the type of swap flag
        inline void setSwapType(const SwapSet type) {flags.setSwapType(type);}
        /// Set the type of complement flag
        inline void setCompType(const CompSet type) {flags.setCompType(type);}
        /* Merge */
        /// Set the merge type (will be removed later)
        inline void setMergeType(const MergeType type) {mergeType = type;}
        /* Name */
        /// Set the name of the BDD or BMxD
        inline void setName(const std::string& bdd) {name = bdd;}

        //******************************************
        //  Size of Node in NodeManager
        //******************************************
        inline int nodeSize() const {
            bool isRel = isRelation();
            int reductionSize = getReductionSize();
            int lvlSlots = 0;
            int infoSize = 0;
            /* slots for values.
                Note: for finite range, only logN bits needed: further compress TBD.
            */
            // check if this node needs level info
            if (reductionSize>0) lvlSlots = isRel ? 2 : 1;
            // check if this node needs value info; if so, what is the size
            ValueType valType = getValType();
            if (encodingType != TERMINAL) {
                if (valType==LONG || valType==DOUBLE) {
                    infoSize = isRel ? 6+3*2 : 4+2;
                } else if (valType==INT || valType==FLOAT) {
                    infoSize = isRel ? 6+3 : 4+1;
                }
            } else {
                infoSize = isRel ? 6 : 4;
            }
            // final size
            infoSize += lvlSlots;
            return infoSize;
        }
        //******************************************
        //  I/O
        //******************************************

        void output(std::ostream& out, int format = 0) const;
        void input(std::istream& in);

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Helper Methods ==============================================

    /// =============================================================
        VarDomain       domain;         // Variable domain: number of variables, variable order
        Range           range;          // Range type and special values
        Reductions      reductions;     // Reduction: number of rules, set of rules
        Flags           flags;          // Edge flags type
        EncodeMechanism encodingType;   // Encoding mechanism: terminal, edge-valued
        MergeType       mergeType;      // Merge type (will be removed in the future)
        std::string     name;           // The name of the forest
};

#endif