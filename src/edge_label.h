#ifndef REXBDD_Edge_LABEL_H
#define REXBDD_Edge_LABEL_H

#include "defines.h"
#include "setting.h"
#include "hash_stream.h"
#include "error.h"

//
namespace REXBDD {
    /// Value type
    //      Note: used only for EVBDDs different value range and precision
    typedef enum {
        VOID,               //  Nothing, for BDDs without values on edges
        INT,                //  Integer
        LONG,               //  Long
        FLOAT,              //  Float
        DOUBLE              //  Double
    } ValueType;

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
        EdgeLabel(ReductionRule v);
        /// Initializes to INT
        EdgeLabel(int v);
        /// Initializes to LONG
        EdgeLabel(long v);
        /// Initializes to FLOAT
        EdgeLabel(float v);
        /// Initializes to DOUBLE
        EdgeLabel(double v);

        //******************************************
        //  Checkers for the value type
        //******************************************
        inline bool isValueVoid() const {
            return (valueType == ValueType::VOID);
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
        //******************************************
        //  Getters for the type
        //******************************************
        inline ValueType getValueType() const {
            return valueType;
        }

        //******************************************
        //  Getters for the label
        //******************************************
        //
        // Values
        //
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
        //
        // Reduction rules
        //
        inline ReductionRule getRule() const {
            return rule;
        }
        //
        // Flags
        //
        inline bool getComp() const {
            return complement;
        }
        inline bool getSwap() const {
            return swap;
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
        inline void getValueTo(ReductionRule &v) const {
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
            getValueTo( *((ReductionRule*) p) );
        }
        inline void getValueTo(ValueType et, void* p) const {
            switch (et) {
                case ValueType::VOID:
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
        //
        // Values
        //
        inline void setValue() {
            valueType = ValueType::VOID;
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
        inline void setValueType(ValueType t) {
            valueType = t;
        }
        //
        // Reduction rules
        //
        inline void setRule(ReductionRule r) {
            valueType = ValueType::VOID;
            if (r < EL0 || r > I1) {
                throw error(REXBDD::error::VALUE_OVERFLOW, __FILE__, __LINE__);
            }
            rule = r;
        }
        //
        // Flags
        //
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
            if (*((const ReductionRule*) p) >= 0 
                && *((const ReductionRule*) p) <= 15) {
                setRule(*((const ReductionRule*) p));
            }
        }
        inline void setRule(const void *p) {
            REXBDD_DCASSERT(p);
            setValue( *((const ReductionRule*) p) );
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
        inline bool equalsRule(ReductionRule r) const {
            return r == rule;
        }
        inline bool equalsComp(bool c) const {
            return c == complement;
        }
        inline bool equalsSwap(bool s) const {
            return s == swap;
        }
        inline bool equalsEdge(const EdgeLabel &v) const {
            if (v.getComp() != complement || v.getSwap() != swap) return false;
            switch (valueType) {
                case ValueType::VOID:
                    if (v.isValueVoid()) {
                        return v.equalsRule(rule);
                    };
                    return false;

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
                    if (rule) {
                        h.push(&rule, sizeof(ReductionRule));
                        return;
                    }
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
        ValueType           valueType;      // Type of value, default is VOID
        /// Values
        union {
            int             intValue;
            long            longValue;
            float           floatValue;
            double          doubleValue;
        };

        ReductionRule       rule;           // Reduction rule, default is X

        union {
            bool            swap;           // single swap flag            
            bool            swapMix[2];     // mix swap flags
                                            //  Note: for nonrelation BDDs (placeholder),
                                            //          00: no swap; 10: swap-one;
                                            //          01: swap-all; 11: illegal
                                            //        for relation BDDs,
                                            //          00: no swap; 10: swap-from;
                                            //          01: swap-to; 11: swap-all (from and to)
        };
        bool                complement;     // complement flag

};


#endif