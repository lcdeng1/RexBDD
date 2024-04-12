#ifndef REXBDD_Edge_LABEL_H
#define REXBDD_Edge_LABEL_H

#include "defines.h"
#include "hash_stream.h"
#include "error.h"

//
namespace REXBDD {
    // Type of label on an edge
    typedef enum {
        /// Nothing; for BDDs without rules or values:
        //      QBDD, MTBDD
        VOID,
        /// edge value would be EdgeRule, for BDDs with reduction rules on edges:
        //      FBDD, ZBDD, ESRBDD, CESRBDD, RexBDD, BMxDD
        RULE,
        /// int, for EVBDDs
        INT,
        /// long, for EVBDDs
        LONG,
        /// float, for EVBDDs
        FLOAT,
        /// double, for EVBDDs
        DOUBLE
    } ValueType;

    // Type of complement and swap flags
    typedef enum {
        /// nothing
        NAIVE,
        /// only complement flag
        COMP,
        /// only swap flag
        //      swap-one, swap-all, or mix for nonrelation BDDs
        //          mix is using swap_mix[2]
        SWAP_ONE, SWAP_ALL, SWAP_MIX,
        /// relation only swap flag
        //      swap-from, swap-to, swap-all, or mix for relation BDDs
        //          mix is using swap_mix[2]
        REL_SWAP_FROM, REL_SWAP_TO, REL_SWAP_ALL, REL_SWAP_MIX,
        /// both complement and swap (one, all, or mix) flags
        CO_SWAP_ONE, CO_SWAP_ALL, CO_SWAP_MIX,
        /// relation only for complement and swap (from, to, all, or mix) flags
        REL_CO_SWAP_FROM, REL_CO_SWAP_TO, REL_CO_SWAP_ALL, REL_CO_SWAP_MIX
    } FlagType;

    // Reducation rules on edges
    typedef enum {
        EL0 = 0,
        EL1 = 2,
        EH0 = 4,
        EH1 = 6,
        AL0 = 1,
        AL1 = 3,
        AH0 = 5,
        AH1 = 7,
        X   = 10,
        I0  = 8,
        I1  = 9,
    } EdgeRule;

    class EdgeLabel;

    // input and output
    // TBD

}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                          EdgeLabel class                       *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::EdgeLabel {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        /// Initializes to VOID
        EdgeLabel();
        /// Initializes to RULE
        EdgeLabel(EdgeRule v);
        /// Initializes to INT
        EdgeLabel(int v);
        /// Initializes to LONG
        EdgeLabel(long v);
        /// Initializes to FLOAT
        EdgeLabel(float v);
        /// Initializes to DOUBLE
        EdgeLabel(double v);

        //******************************************
        //  Getters for the type
        //******************************************
        inline bool isLabelVoid() const {
            return (labelType == ValueType::VOID);
        }
        inline bool isLabelRule() const {
            return (labelType == ValueType::RULE);
        }
        inline bool isLabelInt() const {
            return (labelType == ValueType::INT);
        }
        inline bool isLabelLong() const {
            return (labelType == ValueType::LONG);
        }
        inline bool isLabelFloat() const {
            return (labelType == ValueType::FLOAT);
        }
        inline bool isLabelDouble() const {
            return (labelType == ValueType::DOUBLE);
        }
        inline bool hasLabelType(ValueType t) const {
            return (labelType == t);
        }
        inline bool isFlagComp() const {
            return (flagType == FlagType::COMP ||
                    flagType == FlagType::CO_SWAP_ONE ||
                    flagType == FlagType::CO_SWAP_ALL ||
                    flagType == FlagType::CO_SWAP_MIX ||
                    flagType == FlagType::REL_CO_SWAP_FROM ||
                    flagType == FlagType::REL_CO_SWAP_TO ||
                    flagType == FlagType::REL_CO_SWAP_ALL ||
                    flagType == FlagType::REL_CO_SWAP_MIX);
        }
        inline bool isFlagSwap() const {
            return !(flagType == FlagType::COMP ||
                    flagType == FlagType::NAIVE);
        }
        inline bool isFlagSwapMix() const {
            return (flagType == FlagType::SWAP_MIX ||
                    flagType == FlagType::CO_SWAP_MIX ||
                    flagType == FlagType::REL_SWAP_MIX ||
                    flagType == FlagType::REL_CO_SWAP_MIX);
        }
        inline bool hasFlagType(FlagType t) const {
            return (flagType == t);
        }

        //******************************************
        //  Getters for the label
        //******************************************
        inline int getLabelInt() const {
            REXBDD_DCASSERT(isLabelInt());
            return intValue;
        }
        inline long getLabelLong() const {
            REXBDD_DCASSERT(isLabelLong());
            return longValue;
        }
        inline float getLabelFloat() const {
            REXBDD_DCASSERT(isLabelFloat());
            return floatValue;
        }
        inline double getLabelDouble() const {
            REXBDD_DCASSERT(isLabelDouble());
            return doubleValue;
        }
        inline EdgeRule getRule() const {
            REXBDD_DCASSERT(isLabelRule());
            return rule;
        }
        inline bool getFlagComp() const {
            return complement;
        }
        inline bool getSwap() const {
            if (!isFlagSwapMix()) return swap;
            throw error(REXBDD::error::MISCELLANEOUS, __FILE__, __LINE__);
        }

        //******************************************
        // Alternate getters (better for templates)
        //******************************************
        inline void get(int &v) const {
            REXBDD_DCASSERT(isLabelInt());
            v = intValue;
        }
        inline void get(long &v) const {
            REXBDD_DCASSERT(isLabelLong());
            v = longValue;
        }
        inline void get(float &v) const {
            REXBDD_DCASSERT(isLabelFloat());
            v = floatValue;
        }
        inline void get(double &v) const {
            REXBDD_DCASSERT(isLabelDouble());
            v = doubleValue;
        }
        inline void get(EdgeRule &v) const {
            REXBDD_DCASSERT(isLabelRule());
            v = rule;
        }

        //******************************************
        // Getters, for low-level storage objects
        //******************************************
        inline void getLabelInt(void *p) const {
            REXBDD_DCASSERT(p);
            get( *((int*) p) );
        }
        inline void getLabelLong(void *p) const {
            REXBDD_DCASSERT(p);
            get( *((long*) p) );
        }
        inline void getLabelFloat(void *p) const {
            REXBDD_DCASSERT(p);
            get( *((float*) p) );
        }
        inline void getLabelDouble(void *p) const {
            REXBDD_DCASSERT(p);
            get( *((double*) p) );
        }
        inline void getRule(void *p) const {
            REXBDD_DCASSERT(p);
            get( *((EdgeRule*) p) );
        }
        inline void get(ValueType et, void* p) const {
            switch (et) {
                case ValueType::VOID:
                    return;

                case ValueType::INT:
                    getLabelInt(p);
                    return;

                case ValueType::LONG:
                    getLabelLong(p);
                    return;

                case ValueType::FLOAT:
                    getLabelFloat(p);
                    return;

                case ValueType::DOUBLE:
                    getLabelDouble(p);
                    return;

                default:
                    throw error(REXBDD::error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        // Setters
        //******************************************
        inline void set() {
            labelType = ValueType::VOID;
        }
        inline void set(EdgeRule r) {
            labelType = ValueType::RULE;
            rule = r;
        }
        inline void set(int v) {
            labelType = ValueType::INT;
            intValue = v;
        }
        inline void set(long v) {
            labelType = ValueType::LONG;
            longValue = v;
        }
        inline void set(float v) {
            labelType = ValueType::FLOAT;
            floatValue = v;
        }
        inline void set(double v) {
            labelType = ValueType::DOUBLE;
            doubleValue = v;
        }
        inline void set(FlagType t) {
            flagType = t;
        }
        inline void setComp(bool v) {
            complement = v;
        }
        inline void setSwap(bool v) {
            swap = v;
        }

        //******************************************
        // Setters, for low-level storage objects
        //******************************************
        inline void setVoid(const void *p) {
            labelType = ValueType::VOID;
        }
        inline void setRule(const void *p) {
            REXBDD_DCASSERT(p);
            set( *((const EdgeRule*) p) );
        }
        inline void setInt(const void *p) {
            REXBDD_DCASSERT(p);
            set( *((const int*) p) );
        }
        inline void setLong(const void *p) {
            REXBDD_DCASSERT(p);
            set( *((const long*) p) );
        }
        inline void setFloat(const void *p) {
            REXBDD_DCASSERT(p);
            set( *((const float*) p) );
        }
        inline void setDouble(const void *p) {
            REXBDD_DCASSERT(p);
            set( *((const double*) p) );
        }
        inline void set(ValueType et, const void* p) {
            switch (et) {
                case ValueType::VOID:
                    setVoid(p);
                    return;
                case ValueType::RULE:
                    setRule(p);
                    return;

                case ValueType::INT:
                    setInt(p);
                    return;

                case ValueType::LONG:
                    setLong(p);
                    return;

                case ValueType::FLOAT:
                    setFloat(p);
                    return;

                case ValueType::DOUBLE:
                    setDouble(p);
                    return;

                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //  ____________________
        //  | Operations below |
        //  v                  v
        //******************************************
        // Subtract from the edge value
        // Useful for normalization
        //******************************************
        inline void subtract(int v) {
            REXBDD_DCASSERT(isLabelInt());
            intValue -= v;
        }
        inline void subtract(long v) {
            REXBDD_DCASSERT(isLabelLong());
            longValue -= v;
        }
        inline void subtract(float v) {
            REXBDD_DCASSERT(isLabelFloat());
            floatValue -= v;
        }
        inline void subtract(double v) {
            REXBDD_DCASSERT(isLabelDouble());
            doubleValue -= v;
        }

        //******************************************
        // Divide the edge value
        // Useful for normalization
        //******************************************

        inline void divide(int v) {
            REXBDD_DCASSERT(isLabelInt());
            REXBDD_DCASSERT(v);
            intValue /= v;
        }
        inline void divide(long v) {
            REXBDD_DCASSERT(isLabelLong());
            REXBDD_DCASSERT(v);
            longValue /= v;
        }
        inline void divide(float v) {
            REXBDD_DCASSERT(isLabelFloat());
            REXBDD_DCASSERT(v);
            floatValue /= v;
        }
        inline void divide(double v) {
            REXBDD_DCASSERT(isLabelDouble());
            REXBDD_DCASSERT(v);
            doubleValue /= v;
        }

        //******************************************
        // Equality checks
        //******************************************
        inline bool equals(EdgeRule r) const {
            if (labelType != ValueType::RULE) return false;
            return r == rule;
        }
        inline bool equals(int v) const {
            if (labelType != ValueType::INT) return false;
            return v == intValue;
        }
        inline bool equals(long v) const {
            if (labelType != ValueType::LONG) return false;
            return v == longValue;
        }
        inline bool equals(float v) const {
            if (labelType != ValueType::FLOAT) return false;
            if (v) {
                double diff = v-floatValue;
                return ABS(diff/v) < 1e-6;
            } else {
                return ABS(floatValue) < 1e-10;
            }
        }
        inline bool equals(double v) const {
            if (labelType != ValueType::DOUBLE) return false;
            if (v) {
                double diff = v-doubleValue;
                return ABS(diff/v) < 1e-6;
            } else {
                return ABS(doubleValue) < 1e-10;
            }
        }
        inline bool equalsComp(bool c) const {
            return c == complement;
        }
        inline bool equalsSwap(bool s) const {
            return s == swap;
        }
        inline bool equalsEdge(const EdgeLabel &v) const {
            if (v.getFlagComp() != complement || v.getSwap() != swap) return false;
            switch (labelType) {
                case ValueType::VOID:
                    return v.isLabelVoid();

                case ValueType::RULE:
                    return v.equals(rule);

                case ValueType::INT:
                    return v.equals(intValue);

                case ValueType::LONG:
                    return v.equals(longValue);

                case ValueType::FLOAT:
                    return v.equals(floatValue);

                case ValueType::DOUBLE:
                    return v.equals(doubleValue);

                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        // Equality check, for low-level storage objects
        //******************************************
        inline bool equals(const void* p) const {
            switch (labelType) {
                case ValueType::VOID:
                    return isLabelVoid();

                case ValueType::RULE:
                    return equals( *((const EdgeRule*)p) );

                case ValueType::INT:
                    return equals( *((const int*)p) );

                case ValueType::LONG:
                    return equals( *((const long*)p) );

                case ValueType::FLOAT:
                    return equals( *((const float*)p) );

                case ValueType::DOUBLE:
                    return equals( *((const double*)p) );

                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        // Hash the edge value.
        //******************************************
        inline void hash(hash_stream &h) const {
            switch (labelType) {
                case ValueType::INT:
                    h.push(&intValue, sizeof(int));
                    return;

                case ValueType::LONG:
                    h.push(&longValue, sizeof(long));
                    return;

                case ValueType::FLOAT:
                    h.push(&floatValue, sizeof(float));
                    return;

                case ValueType::DOUBLE:
                    h.push(&doubleValue, sizeof(double));
                    return;

                case ValueType::VOID:
                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        //  File I/O
        //******************************************
        // TBD

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        /// the type of this edge label
        ValueType labelType;
        /// the type of this edge flags
        FlagType flagType;

        /// the actual label of this edge
        union {
            int         intValue;
            long        longValue;
            float       floatValue;
            double      doubleValue;
            EdgeRule    rule;
        };

        /// swap flags, always initialized 0
        union {
            /// mix swap flags
            //  Note: for nonrelation BDDs,
            //          00: no swap; 10: swap-one; 01: swap-all; 11: illegal
            //        for relation BDDs,
            //          00: no swap; 10: swap-from; 01: swap-to; 11: swap-all (from and to)
            bool swapMix[2];
            /// single swap flag
            //      0: no swap; 1: swap
            bool swap;
        };

        /// complement flag, always initialized 0
        bool complement;
        
};


#endif