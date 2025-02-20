#ifndef REXBDD_OPERATIONS_BUILTIN_H
#define REXBDD_OPERATIONS_BUILTIN_H

#include "../defines.h"
#include "../forest.h"
#include "../function.h"
#include "operation.h"

namespace REXBDD {
    // ******************************************************************
    // *                                                                *
    // *                        Unary operations                        *
    // *                                                                *
    // ******************************************************************
    class UnaryOperation;

    UnaryOperation* COPY(Forest* arg, Forest* res);
    UnaryOperation* CARDINALITY(Forest* arg, OpndType res);
    UnaryOperation* INDEX(Forest* arg, Forest* res);
    UnaryOperation* MASS(Forest* arg, OpndType res);
    UnaryOperation* HIGHEST(Forest* arg, OpndType res);
    UnaryOperation* LOWEST(Forest* arg, OpndType res);
    UnaryOperation* COMPLEMENT(Forest* arg, Forest* res);
    UnaryOperation* CONCRETIZE_RST(Forest* arg1, SpecialValue arg2, Forest* res);
    UnaryOperation* CONCRETIZE_OSM(Forest* arg1, SpecialValue arg2, Forest* res);
    UnaryOperation* CONCRETIZE_TSM(Forest* arg1, SpecialValue arg2, Forest* res);
    UnaryOperation* EQUANTIFY(Forest* arg, std::vector<uint16_t> val, Forest* res);  // 2-L?
    UnaryOperation* UQUANTIFY(Forest* arg, std::vector<uint16_t> val, Forest* res);

    UnaryOperation* REORDER(Forest* arg, Forest* res);

    // ******************************************************************
    // *                                                                *
    // *                        Binary operations                       *
    // *                                                                *
    // ******************************************************************
    class BinaryOperation;
    
    // elementwise
    BinaryOperation* UNION(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* INTERSECTION(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* DIFFERENCE(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* MINIMUM(Forest* arg1, OpndType arg2, Forest* res);
    BinaryOperation* MAXIMUM(Forest* arg1, OpndType arg2, Forest* res);
    BinaryOperation* PLUS(Forest* arg1, OpndType arg2, Forest* res);
    BinaryOperation* MINUS(Forest* arg1, OpndType arg2, Forest* res);
    BinaryOperation* MULTIPLY(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* DIVIDE(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* MODULO(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* EQUAL(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* NOT_EQUAL(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* LESS_THAN(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* LESS_THAN_EQUAL(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* GREATER_THAN(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* GREATER_THAN_EQUAL(Forest* arg1, Forest* arg2, Forest* res);

    BinaryOperation* CROSS(Forest* arg1, Forest* arg2, Forest* res);
    
    BinaryOperation* PRE_PLUS(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* POST_PLUS(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* PRE_IMAGE(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* POST_IMAGE(Forest* arg1, Forest* arg2, Forest* res);

    BinaryOperation* VM_MULTIPLY(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* MV_MULTIPLY(Forest* arg1, Forest* arg2, Forest* res);
    BinaryOperation* MM_MULTIPLY(Forest* arg1, Forest* arg2, Forest* res);



    // ******************************************************************
    // *                                                                *
    // *                       Numerical operations                     *
    // *                                                                *
    // ******************************************************************
    // TBD

    // ******************************************************************
    // *                                                                *
    // *                      Saturation operations                     *
    // *                                                                *
    // ******************************************************************
    //TBD
    inline Func saturate(Func set, FuncArray forwards) {
        // TBD
        return set;
    }
}

#endif