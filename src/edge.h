#ifndef REXBDD_EDGE_H
#define REXBDD_EDGE_H

#include "defines.h"
#include "setting.h"

namespace REXBDD {
    enum class SpecialValue {
        OMEGA,
        POS_INF,
        NEG_INF,
        UNDEF
    };
    static inline std::string speciaValue2String(SpecialValue value) {
        std::string sv;
        if (value == SpecialValue::OMEGA) {
            sv = "Omega";
        } else if (value == SpecialValue::POS_INF) {
            sv = "PosInf";
        } else if (value == SpecialValue::NEG_INF) {
            sv = "NegInf";
        } else if (value == SpecialValue::UNDEF) {
            sv = "UnDef";
        } else {
            sv = "Unknown";
        }
        return sv;
    }
    class Value;
    class Edge;
    class Forest;
    // file I/O
    // TBD

    // ******************************************************************
    // *                                                                *
    // *                        EdgeLabel type                          *
    // *                                                                *
    // ******************************************************************
    /**
     *  Labels for edge rule and flags storage
     *  Each label is constructed as:
     *            [ unused(1 bit) | rule(4 bits) | flags(3 bits)]
     * 
     *  flags(3 bits): swap(_from) | swap_to | complement
     *  
     */
    typedef uint8_t EdgeLabel;
    /* Masks */
    const uint8_t LABEL_RULE_MASK = (uint8_t)(0x0F) << 3;
    const uint8_t LABEL_COMP_MASK = (uint8_t)0x01;
    const uint8_t LABEL_SWAP_MASK = (uint8_t)(0x01 << 2);
    const uint8_t LABEL_SWAP_TO_MASK = (uint8_t)(0x01 << 1);

    /* Methods of EdgeLabel */

    /* Get the reduction rule from the given EdgeLabel */
    static inline ReductionRule unpackRule(const EdgeLabel& label)
    {
        return (ReductionRule)((label & LABEL_RULE_MASK) >> 3);
    }
    /* Get the complement flag from the given EdgeLabel */
    static inline bool unpackComp(const EdgeLabel& label)
    {
        return label & LABEL_COMP_MASK;
    }
    /* Get the swap flag from the given EdgeLabel */
    static inline bool unpackSwap(const EdgeLabel& label)
    {
        return label & LABEL_SWAP_MASK;
    }
    static inline bool unpackSwapTo(const EdgeLabel& label)
    {
        return label & LABEL_SWAP_TO_MASK;
    }
    /* Packing */
    static inline void packRule(EdgeLabel& label, ReductionRule rule)
    {
        label &= ~LABEL_RULE_MASK;
        label |= ((uint8_t)(rule) << 3);
    }
    static inline void packComp(EdgeLabel& label, bool comp)
    {
        label &= ~LABEL_COMP_MASK;
        label |= ((uint8_t)comp);
    }
    static inline void packSwap(EdgeLabel& label, bool swap)
    {
        label &= ~LABEL_SWAP_MASK;
        label |= ((uint8_t)swap << 2);
    }
    static inline void packSwapTo(EdgeLabel& label, bool swap)
    {
        label &= ~LABEL_SWAP_TO_MASK;
        label |= ((uint8_t)swap << 1);
    }

    // ******************************************************************
    // *                                                                *
    // *                        EdgeHandle type                         *
    // *                                                                *
    // ******************************************************************
    /**
     *  Handles for edges storage
     *  This effectively limits the number of possible nodes per forest.
     *  Each handle is constructed as:
     *      [ header (3 bits) | unused(9 bits) | rule(4 bits) | flags(3 bits) | level(16 bits) | nodeIdx(32 bits) ]
     *  "nodeIdx" limits the number of nodes per level in node manager.
     *  "flags": swap(_from) swap_to complement
     *  "header": type of terminal value
     * 
     *  [TBD] v
     *  The number of bits occupied by each part depends on the forest setting:
     *      Bits of "rule" = |log(numRules)|;
     *      Bits of "flags" = complement bit + swap bits;
     *      Bits of "level" = |log(maxLevel)|;
     *      Bits of "nodeIdx" = the remain bits;
     */
    typedef uint64_t EdgeHandle;
    /* Masks */
    const uint64_t FLOAT_VALUE_FLAG_MASK = ((uint64_t)0x01<<63);
    const uint64_t INT_VALUE_FLAG_MASK = ((uint64_t)0x01<<62);
    const uint64_t SPECIAL_VALUE_FLAG_MASK = ((uint64_t)0x01<<61);
    const uint64_t RULE_MASK = (uint64_t)(0x0F) << 51;
    const uint64_t LEVEL_MASK = (uint64_t)0xFFFF << 32;
    const uint64_t COMP_MASK = (uint64_t)(0x01) << 48;
    const uint64_t SWAP_MASK = (uint64_t)(0x01) << 50;
    const uint64_t SWAP_TO_MASK = (uint64_t)(0x01) << 49;
    const uint64_t NODE_MASK = 0xFFFFFFFF;
    const uint64_t LABEL_MASK = (uint64_t)(0xFF) << 48;

    
    /* Methods of EdgeHandle */

    /* Get the reduction rule from the given EdgeHandle */
    static inline ReductionRule unpackRule(const EdgeHandle& handle)
    {
        return (ReductionRule)((handle & RULE_MASK) >> 51);
    }
    /* Get the level of the target node from the given EdgeHandle */
    static inline uint16_t unpackLevel(const EdgeHandle& handle)
    {
        return (uint16_t)((handle & LEVEL_MASK) >> 32);
    }
    /* Get the complement flag from the given EdgeHandle */
    static inline bool unpackComp(const EdgeHandle& handle)
    {
        return handle & COMP_MASK;
    }
    /* Get the swap flag from the given EdgeHandle */
    static inline bool unpackSwap(const EdgeHandle& handle)
    {
        return handle & SWAP_MASK;
    }
    static inline bool unpackSwapTo(const EdgeHandle& handle)
    {
        return handle & SWAP_TO_MASK;
    }
    /* Get the target node index from the given EdgeHandle */
    static inline NodeHandle unpackTarget(const EdgeHandle& handle)
    {
        return (NodeHandle)(handle & NODE_MASK);
    }
    static inline EdgeLabel unpackLabel(const EdgeHandle& handle)
    {
        return (EdgeLabel)((handle & LABEL_MASK) >> 48);
    }
    /* Packing */
    static inline void packRule(EdgeHandle& handle, ReductionRule rule)
    {
        handle &= ~RULE_MASK;
        handle |= ((uint64_t)rule << 51);
    }
    static inline void packLevel(EdgeHandle& handle, uint16_t level)
    {
        if (((handle & FLOAT_VALUE_FLAG_MASK)
            || (handle & INT_VALUE_FLAG_MASK)
            || (handle & SPECIAL_VALUE_FLAG_MASK))
            && (level>0)) {
            // terminal node
            std::cout << "[REXBDD] ERROR!\t Unable to set target terminal node at level "<<level<< std::endl;
            exit(0);
        }
        handle &= ~LEVEL_MASK;
        handle |= ((uint64_t)level << 32);
    }
    static inline void packComp(EdgeHandle& handle, bool comp)
    {
        handle &= ~COMP_MASK;
        handle |= ((uint64_t)comp << 48);
    }
    static inline void packSwap(EdgeHandle& handle, bool swap)
    {
        handle &= ~SWAP_MASK;
        handle |= ((uint64_t)swap << 50);
    }
    static inline void packSwapTo(EdgeHandle& handle, bool swap)
    {
        handle &= ~SWAP_TO_MASK;
        handle |= ((uint64_t)swap << 49);
    }
    static inline void packTarget(EdgeHandle& handle, NodeHandle target)
    {
        handle &= ~NODE_MASK;
        handle |= (target);
    }
    static inline std::string unpackTermiValue(const EdgeHandle& handle) {
        std::string value = "";
        if (unpackLevel(handle) == 0) {
            NodeHandle target = unpackTarget(handle);
            if (handle & FLOAT_VALUE_FLAG_MASK) {
                value = std::to_string(*reinterpret_cast<float*>(&target));
            } else if (handle & INT_VALUE_FLAG_MASK) {
                value = std::to_string(target);
            } else if (handle & SPECIAL_VALUE_FLAG_MASK) {
                value = speciaValue2String((SpecialValue)target);
            }
        }
        return value;
    }
    static inline void printEdgeHandle(const EdgeHandle& handle, std::ostream& out, int format)
    {
        if (format == 0) {
            out << "<" << rule2String(unpackRule(handle));
            out << ", " << unpackComp(handle);
            out << ", " << unpackSwap(handle);
            out << ", ";
            if (unpackLevel(handle) == 0) {
                out << "T_";
                out << unpackTermiValue(handle);
            } else {
                out << unpackTarget(handle);
            }
            out << "> L: " << unpackLevel(handle);
        } else {
            // more format TBD
        }
    }

}; // end of namespace


// ******************************************************************
// *                                                                *
// *                                                                *
// *                          Value class                           *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Value {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Value();
    Value(int i);
    Value(long l);
    Value(double d);
    Value(float f);

    ValueType getType() const { return valueType; }
    inline void getValueTo(void* p, ValueType type) const {
        switch (type) {
            case VOID:
                *((SpecialValue*) p) = getSpecialValue();
                return;
            case INT:
                *((int*) p) = getIntValue();
                return;
            case LONG:
                *((long*) p) = getLongValue();
                return;
            case FLOAT:
                *((float*) p) = getFloatValue();
                return;
            case DOUBLE:
                *((double*) p) = getDoubleValue();
                return;
            default:
                throw error(REXBDD::ErrCode::MISCELLANEOUS, __FILE__, __LINE__);
        }
    }
    inline void setValue(const void* p, const ValueType type) {
        switch (type) {
            case VOID:
                setSpecial(p);
                break;
            case INT:
                setInt(p);
                break;
            case LONG:
                setLong(p);
                break;
            case FLOAT:
                setFloat(p);
                break;
            case DOUBLE:
                setDouble(p);
                break;
            default:
                throw error(REXBDD::ErrCode::MISCELLANEOUS, __FILE__, __LINE__);
            }
    }
    template <typename T>
    inline void setValue(const T& value, const ValueType type) {
        setValue(static_cast<const void*>(&value), type);
    }
    inline Value& operator=(const Value& val) {
        if (equals(val)) return *this;
        init(val);
        return *this;
    }
    inline bool operator==(const Value& val) const {
        return equals(val);
    }
    inline bool operator!=(const Value& val) const {
        return !equals(val);
    }
    void print(std::ostream& out, int format) const;
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    inline int getIntValue() const {
        if (valueType != INT) {
            return static_cast<int>(floatValue);
        }
        return intValue;
    }
    inline long getLongValue() const { return longValue;}
    inline float getFloatValue() const { 
        if (valueType != FLOAT) {
            return static_cast<float>(intValue);
        }
        return floatValue;
    }
    inline double getDoubleValue() const { return doubleValue;}
    inline SpecialValue getSpecialValue() const { return special;}
    // inline void setVoid() {valueType = VOID;}
    inline void setInt(const void *p) {
        REXBDD_DCASSERT(p);
        valueType = INT;
        intValue = *((const int*) p);
    }
    inline void setLong(const void *p) {
        REXBDD_DCASSERT(p);
        valueType = LONG;
        longValue = *((const long*) p);
    }
    inline void setFloat(const void *p) {
        REXBDD_DCASSERT(p);
        valueType = FLOAT;
        floatValue = *((const float*) p);
    }
    inline void setDouble(const void *p) {
        REXBDD_DCASSERT(p);
        valueType = DOUBLE;
        doubleValue = *((const double*) p);
    }
    inline void setSpecial(const void *p) {
        REXBDD_DCASSERT(p);
        valueType = VOID;
        longValue = 0;
        special = *((const SpecialValue*) p);
    }
    inline bool equals(const Value& val) const {
        bool isEqual = 1;
        if (valueType != val.valueType) return 0;
        switch (valueType) {
            case VOID:
                isEqual = (special == val.special);
                break;
            case INT:
                isEqual = (intValue == val.intValue);
                break;
            case LONG:
                isEqual = (longValue == val.longValue);
                break;
            case FLOAT:
                isEqual = (floatValue == val.floatValue);      // Precision? TBD
                break;
            case DOUBLE:
                isEqual = (doubleValue == val.doubleValue);    // Precision? TBD
                break;
            default:
                throw error(REXBDD::ErrCode::MISCELLANEOUS, __FILE__, __LINE__);
        }
        return isEqual;
    }
    inline void init(const Value& val) {
        valueType = val.valueType;
        switch (valueType) {
            case VOID:
                special = val.special;
                break;
            case INT:
                intValue = val.intValue;
                break;
            case LONG:
                longValue = val.longValue;
                break;
            case FLOAT:
                floatValue = val.floatValue;
                break;
            case DOUBLE:
                doubleValue = val.doubleValue;
                break;
            default:
                throw error(REXBDD::ErrCode::MISCELLANEOUS, __FILE__, __LINE__);
        }
    }

    /*-------------------------------------------------------------*/
    friend class Func;
    /// Values
    union {
        int             intValue;
        long            longValue;
        float           floatValue;
        double          doubleValue;
    };
    ValueType valueType;
    SpecialValue special;
};
// ******************************************************************
// *                                                                *
// *                                                                *
// *                          Edge class                            *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Edge {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        Edge();
        Edge(const Edge &e);
        Edge(const ReductionRule rule, Value val);
        Edge(const EdgeHandle h, Value val);
        ~Edge();

        /* Access to data */

        inline uint16_t getNodeLevel() const {return unpackLevel(handle);}
        inline NodeHandle getNodeHandle() const {return unpackTarget(handle);}
        inline EdgeHandle getEdgeHandle() const {return handle;}
        // get rule and flags TBD
        inline ReductionRule getRule() const {return unpackRule(handle);}
        inline bool getComp() const {return unpackComp(handle);}
        inline bool getSwap(bool isTo) const {return (isTo)?unpackSwapTo(handle):unpackSwap(handle);}
        inline Value getValue() const {return value;}

        inline void setNodeHandle(NodeHandle target) {packTarget(handle, target);}
        inline void setEdgeHandle(EdgeHandle edge) {handle = edge;}
        inline void setRule(ReductionRule rule) {packRule(handle, rule);}
        inline void setComp(bool comp) {packComp(handle, comp);}
        inline void setSwap(bool swap, bool isTo) {
            if (isTo) {
                packSwapTo(handle, swap);
            } else {
                packSwap(handle, swap);
            }
        }
        inline void setLevel(uint16_t lvl) {packLevel(handle, lvl);}

        // unpack the x/y part of a long edge, only be used for BDDs edge
        Edge part(bool xy) const;

        // this only be used by BDDs or BMXDs with range 1
        bool isComplementTo(const Edge& e) const;

        bool isConstantOne() const;

        bool isConstantZero() const;

        inline void complement() {
            // complement the reduction rule and flip the complement bit, when complement allowed
            packRule(handle, compRule(unpackRule(handle)));
            packComp(handle, !unpackComp(handle));
            // value? TBD
        }
        inline void swap() {
            // swap the reduction rule and flip the swap bit, when swap-all allowed
            packRule(handle, swapRule(unpackRule(handle)));
            packSwap(handle, !unpackSwap(handle));
        }

        inline Edge& operator=(const Edge& e) {
            if (equals(e)) return *this;
            init(e);
            return *this;
        }

        inline bool operator==(const Edge& e) const {
            return equals(e);
        }
        inline bool operator!=(const Edge& e) const {
            return !equals(e);
        }

        void print(std::ostream& out, int format=0) const;
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        inline void init(const Edge& e) {
            handle = e.handle;
            value = e.value;
        }
        inline bool equals(const Edge& e) const {
            return (handle == e.handle) && (value == e.value);
        }
        /* Getters and Setters under Forest */
        friend class Forest;
        friend class Func;
        friend class Node;

        /* Actual edge information */
        EdgeHandle      handle;     // Rule, flags, taget node and level.
        Value           value;      // Edge value.

        // std::string     display;    // for displaying if needed in the future
};

#endif