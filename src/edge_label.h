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
        //  Checkers for the type
        //******************************************
        inline bool isValueVoid() const {
            return (valueType == ValueType::VOID);
        }
        inline bool isValueRule() const {
            return (valueType == ValueType::RULE);
        }
        inline bool isValueInt() const {
            return (valueType == ValueType::INT);
        }
        inline bool isValueLong() const {
            return (valueType == ValueType::LONG);
        }
        inline bool isValueFloat() const {
            return (valueType == ValueType::FLOAT);
        }
        inline bool isValueDouble() const {
            return (valueType == ValueType::DOUBLE);
        }
        inline bool hasValueType(ValueType t) const {
            return (valueType == t);
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
        // maybe the check methods can be removed?

        //******************************************
        //  Getters for the type
        //******************************************
        inline ValueType getValueType() const {
            return valueType;
        }
        inline FlagType getFlagType() const {
            return flagType;
        }

        //******************************************
        //  Getters for the label
        //******************************************
        inline int getIntValue() const {
            REXBDD_DCASSERT(isValueInt());
            return intValue;
        }
        inline long getLongValue() const {
            REXBDD_DCASSERT(isValueLong());
            return longValue;
        }
        inline float getFloatValue() const {
            REXBDD_DCASSERT(isValueFloat());
            return floatValue;
        }
        inline double getDoubleValue() const {
            REXBDD_DCASSERT(isValueDouble());
            return doubleValue;
        }
        inline EdgeRule getRule() const {
            REXBDD_DCASSERT(isValueRule());
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
        inline void getValueTo(int &v) const {
            REXBDD_DCASSERT(isValueInt());
            v = intValue;
        }
        inline void getValueTo(long &v) const {
            REXBDD_DCASSERT(isValueLong());
            v = longValue;
        }
        inline void getValueTo(float &v) const {
            REXBDD_DCASSERT(isValueFloat());
            v = floatValue;
        }
        inline void getValueTo(double &v) const {
            REXBDD_DCASSERT(isValueDouble());
            v = doubleValue;
        }
        inline void getValueTo(EdgeRule &v) const {
            REXBDD_DCASSERT(isValueRule());
            v = rule;
        }

        //******************************************
        // Getters, for low-level storage objects
        //******************************************
        inline void getIntValueTo(void *p) const {
            REXBDD_DCASSERT(p);
            getValueTo( *((int*) p) );
        }
        inline void getLongValueTo(void *p) const {
            REXBDD_DCASSERT(p);
            getValueTo( *((long*) p) );
        }
        inline void getFloatValueTo(void *p) const {
            REXBDD_DCASSERT(p);
            getValueTo( *((float*) p) );
        }
        inline void getDoubleValueTo(void *p) const {
            REXBDD_DCASSERT(p);
            getValueTo( *((double*) p) );
        }
        inline void getRuleTo(void *p) const {
            REXBDD_DCASSERT(p);
            getValueTo( *((EdgeRule*) p) );
        }
        inline void getValueTo(ValueType et, void* p) const {
            switch (et) {
                case ValueType::VOID:
                    return;
                case ValueType::RULE:
                    getRuleTo(p);
                    return;

                case ValueType::INT:
                    getIntValueTo(p);
                    return;

                case ValueType::LONG:
                    getLongValueTo(p);
                    return;

                case ValueType::FLOAT:
                    getFloatValueTo(p);
                    return;

                case ValueType::DOUBLE:
                    getDoubleValueTo(p);
                    return;

                default:
                    throw error(REXBDD::error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        // Setters
        //******************************************
        inline void setValue() {
            valueType = ValueType::VOID;
        }
        inline void setValue(EdgeRule r) {
            valueType = ValueType::RULE;
            rule = r;
        }
        inline void setValue(int v) {
            valueType = ValueType::INT;
            intValue = v;
        }
        inline void setValue(long v) {
            valueType = ValueType::LONG;
            longValue = v;
        }
        inline void setValue(float v) {
            valueType = ValueType::FLOAT;
            floatValue = v;
        }
        inline void setValue(double v) {
            valueType = ValueType::DOUBLE;
            doubleValue = v;
        }
        inline void setFlagType(FlagType t) {
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
            valueType = ValueType::VOID;
        }
        inline void setRule(const void *p) {
            REXBDD_DCASSERT(p);
            setValue( *((const EdgeRule*) p) );
        }
        inline void setInt(const void *p) {
            REXBDD_DCASSERT(p);
            setValue( *((const int*) p) );
        }
        inline void setLong(const void *p) {
            REXBDD_DCASSERT(p);
            setValue( *((const long*) p) );
        }
        inline void setFloat(const void *p) {
            REXBDD_DCASSERT(p);
            setValue( *((const float*) p) );
        }
        inline void setDouble(const void *p) {
            REXBDD_DCASSERT(p);
            setValue( *((const double*) p) );
        }
        inline void setValue(ValueType et, const void* p) {
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
            REXBDD_DCASSERT(isValueInt());
            intValue -= v;
        }
        inline void subtract(long v) {
            REXBDD_DCASSERT(isValueLong());
            longValue -= v;
        }
        inline void subtract(float v) {
            REXBDD_DCASSERT(isValueFloat());
            floatValue -= v;
        }
        inline void subtract(double v) {
            REXBDD_DCASSERT(isValueDouble());
            doubleValue -= v;
        }

        //******************************************
        // Divide the edge value
        // Useful for normalization
        //******************************************

        inline void divide(int v) {
            REXBDD_DCASSERT(isValueInt());
            REXBDD_DCASSERT(v);
            intValue /= v;
        }
        inline void divide(long v) {
            REXBDD_DCASSERT(isValueLong());
            REXBDD_DCASSERT(v);
            longValue /= v;
        }
        inline void divide(float v) {
            REXBDD_DCASSERT(isValueFloat());
            REXBDD_DCASSERT(v);
            floatValue /= v;
        }
        inline void divide(double v) {
            REXBDD_DCASSERT(isValueDouble());
            REXBDD_DCASSERT(v);
            doubleValue /= v;
        }

        //******************************************
        // Equality checks
        //******************************************
        inline bool equals(EdgeRule r) const {
            if (valueType != ValueType::RULE) return false;
            return r == rule;
        }
        inline bool equals(int v) const {
            if (valueType != ValueType::INT) return false;
            return v == intValue;
        }
        inline bool equals(long v) const {
            if (valueType != ValueType::LONG) return false;
            return v == longValue;
        }
        inline bool equals(float v) const {
            if (valueType != ValueType::FLOAT) return false;
            if (v) {
                double diff = v-floatValue;
                return ABS(diff/v) < 1e-6;
            } else {
                return ABS(floatValue) < 1e-10;
            }
        }
        inline bool equals(double v) const {
            if (valueType != ValueType::DOUBLE) return false;
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
            switch (valueType) {
                case ValueType::VOID:
                    return v.isValueVoid();

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
            switch (valueType) {
                case ValueType::VOID:
                    return isValueVoid();

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
            switch (valueType) {
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
        ValueType valueType;
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