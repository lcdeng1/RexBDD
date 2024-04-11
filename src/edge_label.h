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
    } LabelType;

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

/**
    Unified edge value object.
*/
class REXBDD::EdgeLabel {

    public:
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
            return (label_type == LabelType::VOID);
        }
        inline bool isLabelRule() const {
            return (label_type == LabelType::RULE);
        }
        inline bool isLabelInt() const {
            return (label_type == LabelType::INT);
        }
        inline bool isLabelLong() const {
            return (label_type == LabelType::LONG);
        }
        inline bool isLabelFloat() const {
            return (label_type == LabelType::FLOAT);
        }
        inline bool isLabelDouble() const {
            return (label_type == LabelType::DOUBLE);
        }
        inline bool hasLabelType(LabelType t) const {
            return (label_type == t);
        }
        inline bool isFlagComp() const {
            return (flag_type == FlagType::COMP ||
                    flag_type == FlagType::CO_SWAP_ONE ||
                    flag_type == FlagType::CO_SWAP_ALL ||
                    flag_type == FlagType::CO_SWAP_MIX ||
                    flag_type == FlagType::REL_CO_SWAP_FROM ||
                    flag_type == FlagType::REL_CO_SWAP_TO ||
                    flag_type == FlagType::REL_CO_SWAP_ALL ||
                    flag_type == FlagType::REL_CO_SWAP_MIX);
        }
        inline bool isFlagSwap() const {
            return !(flag_type == FlagType::COMP ||
                    flag_type == FlagType::NAIVE);
        }
        inline bool isFlagSwapMix() const {
            return (flag_type == FlagType::SWAP_MIX ||
                    flag_type == FlagType::CO_SWAP_MIX ||
                    flag_type == FlagType::REL_SWAP_MIX ||
                    flag_type == FlagType::REL_CO_SWAP_MIX);
        }
        inline bool hasFlagType(FlagType t) const {
            return (flag_type == t);
        }

        //******************************************
        //  Getters for the label
        //******************************************
        inline int getLabelInt() const {
            REXBDD_DCASSERT(isLabelInt());
            return ev_int;
        }
        inline long getLabelLong() const {
            REXBDD_DCASSERT(isLabelLong());
            return ev_long;
        }
        inline float getLabelFloat() const {
            REXBDD_DCASSERT(isLabelFloat());
            return ev_float;
        }
        inline double getLabelDouble() const {
            REXBDD_DCASSERT(isLabelDouble());
            return ev_double;
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
            v = ev_int;
        }
        inline void get(long &v) const {
            REXBDD_DCASSERT(isLabelLong());
            v = ev_long;
        }
        inline void get(float &v) const {
            REXBDD_DCASSERT(isLabelFloat());
            v = ev_float;
        }
        inline void get(double &v) const {
            REXBDD_DCASSERT(isLabelDouble());
            v = ev_double;
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
        inline void get(LabelType et, void* p) const {
            switch (et) {
                case LabelType::VOID:
                    return;

                case LabelType::INT:
                    getLabelInt(p);
                    return;

                case LabelType::LONG:
                    getLabelLong(p);
                    return;

                case LabelType::FLOAT:
                    getLabelFloat(p);
                    return;

                case LabelType::DOUBLE:
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
            label_type = LabelType::VOID;
        }
        inline void set(EdgeRule r) {
            label_type = LabelType::RULE;
            rule = r;
        }
        inline void set(int v) {
            label_type = LabelType::INT;
            ev_int = v;
        }
        inline void set(long v) {
            label_type = LabelType::LONG;
            ev_long = v;
        }
        inline void set(float v) {
            label_type = LabelType::FLOAT;
            ev_float = v;
        }
        inline void set(double v) {
            label_type = LabelType::DOUBLE;
            ev_double = v;
        }
        inline void set(FlagType t) {
            flag_type = t;
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
            label_type = LabelType::VOID;
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
        inline void set(LabelType et, const void* p) {
            switch (et) {
                case LabelType::VOID:
                    setVoid(p);
                    return;
                case LabelType::RULE:
                    setRule(p);
                    return;

                case LabelType::INT:
                    setInt(p);
                    return;

                case LabelType::LONG:
                    setLong(p);
                    return;

                case LabelType::FLOAT:
                    setFloat(p);
                    return;

                case LabelType::DOUBLE:
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
            ev_int -= v;
        }
        inline void subtract(long v) {
            REXBDD_DCASSERT(isLabelLong());
            ev_long -= v;
        }
        inline void subtract(float v) {
            REXBDD_DCASSERT(isLabelFloat());
            ev_float -= v;
        }
        inline void subtract(double v) {
            REXBDD_DCASSERT(isLabelDouble());
            ev_double -= v;
        }

        //******************************************
        // Divide the edge value
        // Useful for normalization
        //******************************************

        inline void divide(int v) {
            REXBDD_DCASSERT(isLabelInt());
            REXBDD_DCASSERT(v);
            ev_int /= v;
        }
        inline void divide(long v) {
            REXBDD_DCASSERT(isLabelLong());
            REXBDD_DCASSERT(v);
            ev_long /= v;
        }
        inline void divide(float v) {
            REXBDD_DCASSERT(isLabelFloat());
            REXBDD_DCASSERT(v);
            ev_float /= v;
        }
        inline void divide(double v) {
            REXBDD_DCASSERT(isLabelDouble());
            REXBDD_DCASSERT(v);
            ev_double /= v;
        }

        //******************************************
        // Equality checks
        //******************************************
        inline bool equals(EdgeRule r) const {
            if (label_type != LabelType::RULE) return false;
            return r == rule;
        }
        inline bool equals(int v) const {
            if (label_type != LabelType::INT) return false;
            return v == ev_int;
        }
        inline bool equals(long v) const {
            if (label_type != LabelType::LONG) return false;
            return v == ev_long;
        }
        inline bool equals(float v) const {
            if (label_type != LabelType::FLOAT) return false;
            if (v) {
                double diff = v-ev_float;
                return ABS(diff/v) < 1e-6;
            } else {
                return ABS(ev_float) < 1e-10;
            }
        }
        inline bool equals(double v) const {
            if (label_type != LabelType::DOUBLE) return false;
            if (v) {
                double diff = v-ev_double;
                return ABS(diff/v) < 1e-6;
            } else {
                return ABS(ev_double) < 1e-10;
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
            switch (label_type) {
                case LabelType::VOID:
                    return v.isLabelVoid();

                case LabelType::RULE:
                    return v.equals(rule);

                case LabelType::INT:
                    return v.equals(ev_int);

                case LabelType::LONG:
                    return v.equals(ev_long);

                case LabelType::FLOAT:
                    return v.equals(ev_float);

                case LabelType::DOUBLE:
                    return v.equals(ev_double);

                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        // Equality check, for low-level storage objects
        //******************************************
        inline bool equals(const void* p) const {
            switch (label_type) {
                case LabelType::VOID:
                    return isLabelVoid();

                case LabelType::RULE:
                    return equals( *((const EdgeRule*)p) );

                case LabelType::INT:
                    return equals( *((const int*)p) );

                case LabelType::LONG:
                    return equals( *((const long*)p) );

                case LabelType::FLOAT:
                    return equals( *((const float*)p) );

                case LabelType::DOUBLE:
                    return equals( *((const double*)p) );

                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        // Hash the edge value.
        //******************************************
        inline void hash(hash_stream &h) const {
            switch (label_type) {
                case LabelType::INT:
                    h.push(&ev_int, sizeof(int));
                    return;

                case LabelType::LONG:
                    h.push(&ev_long, sizeof(long));
                    return;

                case LabelType::FLOAT:
                    h.push(&ev_float, sizeof(float));
                    return;

                case LabelType::DOUBLE:
                    h.push(&ev_double, sizeof(double));
                    return;

                case LabelType::VOID:
                default:
                    throw error(error::MISCELLANEOUS, __FILE__, __LINE__);
            }
        }

        //******************************************
        //  File I/O
        //******************************************
        // TBD


    private:
        /// the type of this edge label
        LabelType label_type;
        /// the type of this edge flags
        FlagType flag_type;


        /// the actual label of this edge
        union {
            int         ev_int;
            long        ev_long;
            float       ev_float;
            double      ev_double;
            EdgeRule    rule;
        };

        /// swap flags, always initialized 0
        union {
            /// mix swap flags
            //  Note: for nonrelation BDDs,
            //          00: no swap; 10: swap-one; 01: swap-all; 11: illegal
            //        for relation BDDs,
            //          00: no swap; 10: swap-from; 01: swap-to; 11: swap-all (from and to)
            bool swap_mix[2];
            /// single swap flag
            //      0: no swap; 1: swap
            bool swap;
        };

        /// complement flag, always initialized 0
        bool complement;
        
};


#endif