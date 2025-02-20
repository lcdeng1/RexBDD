#ifndef REXBDD_TERMINAL_H
#define REXBDD_TERMINAL_H

#include "defines.h"
#include "edge.h"

namespace REXBDD {
    // ******************************************************************
    // *                                                                *
    // *                        Terminal Handle                         *
    // *                                                                *
    // ******************************************************************
    /**
     * EdgeHandle pointing to terminal nodes
     * Assuming terminal value can only be:
     *          INT, FLOAT, -∞, +∞, UNDEF, OMEGA.
     * header (9 bits): bit 63: 1: if terminal is float value;
     *                  bit 62: 1: if terminal is int value;
     *                  bit 61: 1: if terminal is special value;
     * 
     * In Node storage: informatin in target NodeHandle, ForestSetting tells the meaning
     * 
     */
    
    /**
     * @brief Get the terminal value for the given EdgeHandle.
     * Note: it would check if the given handle points to terminal, and the value type header is valid.
     *       it will exit if check failed.
     * 
     * @param handle        The given EdgeHandle.
     * @return Value     – Output termianl value wrapper.
     */
    static inline Value getTerminalValue(const EdgeHandle& handle) {
        if(unpackLevel(handle)>0) {
            std::cout << "[REXBDD] ERROR!\t getTerminalValue(EdgeHandle): No value for nonterminal node!"<< std::endl;
            exit(0);
        }
        Value val(0);
        NodeHandle data = unpackTarget(handle);
        if (handle & FLOAT_VALUE_FLAG_MASK) {
            // float value
            float value = *reinterpret_cast<float*>(&data);
            val.setValue(value, FLOAT);
        } else if (handle & INT_VALUE_FLAG_MASK) {
            // int value
            int value = *reinterpret_cast<int*>(&data);
            val.setValue(value, INT);
        } else if (handle & SPECIAL_VALUE_FLAG_MASK) {
            // special value
            SpecialValue value = *reinterpret_cast<SpecialValue*>(&data);
            val.setValue(value, VOID);
        } else {
            // unknown value
            std::cout << "[REXBDD] ERROR!\t getTerminalValue(EdgeHandle): Unknown value for terminal node!"<< std::endl;
            exit(0);
        }
        return val;
    }
    
    static inline bool isTerminalOne(const EdgeHandle& handle) {
        Value val = getTerminalValue(handle);
        bool ans = 0;
        union {
            int valInt;
            float valFloat;
        };
        if (val.getType() == INT) {
            val.getValueTo(&valInt, INT);
            ans = (valInt == 1);
        } else {
            val.getValueTo(&valFloat, FLOAT);
            ans = (valFloat == 1.0f);
        }
        return ans;
    }

    static inline bool isTerminalZero(const EdgeHandle& handle) {
        Value val = getTerminalValue(handle);
        bool ans = 0;
        union {
            int valInt;
            float valFloat;
        };
        if (val.getType() == INT) {
            val.getValueTo(&valInt, INT);
            ans = (valInt == 0);
        } else {
            val.getValueTo(&valFloat, FLOAT);
            ans = (valFloat == 0.0f);
        }
        return ans;
    }
    /**
     * @brief Make a plain EdgeHandle target to terminal node, by giving the terminal value and type.
     * Note: Since the terminal value type (INT or FLOAT) is not distinguished in the actual stored nodes 
     *       in NodeManager, and the child edges of the node need to be queried during the evaluation
     *       process, please make sure that the "type" used is consistent with the ValueType supported 
     *       by the working forest, otherwise unpredictable results will occur.
     * 
     * @param type          The value type of terminal value: INT, FLOAT, or VOID for special value.
     * @param value         The terminal value.
     * @return EdgeHandle   - Output plain EdgeHandle without rule or flags initialied.
     */
    static inline EdgeHandle makeTerminal(const ValueType type, const void* value) {
        if (type != INT && type != FLOAT && type != VOID) {
            std::cout << "[REXBDD] ERROR!\t makeTerminal(ValueType, void*): Unsupported data type! It can only be INT, FLOAT, or VOID."<< std::endl;
            exit(0);
        }
        EdgeHandle handle = 0;
        NodeHandle node = 0;
        if (type == INT) {
            handle |= INT_VALUE_FLAG_MASK;
            int target = *((int*) value);
            node = *reinterpret_cast<NodeHandle*>(&target);
        } else if (type == FLOAT) {
            handle |= FLOAT_VALUE_FLAG_MASK;
            float target = *((float*) value);
            node = *reinterpret_cast<NodeHandle*>(&target);
        } else if (type == VOID){
            handle |= SPECIAL_VALUE_FLAG_MASK;
            SpecialValue target = *((SpecialValue*) value);
            node = *reinterpret_cast<NodeHandle*>(&target);
        }
        packTarget(handle, node);
        return handle;
    }
    /**
     * @brief Make a plain EdgeHandle target to terminal node, by giving the terminal value and type.
     * Note: Since the terminal value type (INT or FLOAT) is not distinguished in the actual stored nodes 
     *       in NodeManager, and the child edges of the node need to be queried during the evaluation
     *       process, please make sure that the "type" used is consistent with the ValueType supported 
     *       by the working forest, otherwise unpredictable results will occur.
     * 
     * @param type          The value type of terminal value: INT, FLOAT, or VOID for special value.
     * @param value         The terminal value.
     * @return EdgeHandle   - Output plain EdgeHandle without rule or flags initialied.
     */
    template <typename T>
    static inline EdgeHandle makeTerminal(const ValueType type, const T& value) {
        // return makeTerminal(type, reinterpret_cast<void*>(const_cast<T*>(&value)));
        return makeTerminal(type, static_cast<const void*>(&value));
    }

}; // end of namespace

#endif