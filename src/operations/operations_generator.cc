#include "operations_generator.h"

namespace REXBDD {
    UnaryList UOPs;
    BinaryList BOPs;
}

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                            Front End                           *
// *                                                                *
// ******************************************************************
// Unary operations
UnaryOperation* REXBDD::COPY(Forest* arg, Forest* res)
{
    if (!arg) return nullptr;
    UnaryOperation* uop = UOPs.find(UnaryOperationType::UOP_COPY, arg, res);
    if (uop) return uop;
    return UOPs.add(new UnaryOperation(UnaryOperationType::UOP_COPY, arg, res));
}
UnaryOperation* REXBDD::CARDINALITY(Forest* arg, OpndType res)
{
    if (!arg) return nullptr;
    UnaryOperation* uop = UOPs.find(UnaryOperationType::UOP_CARDINALITY, arg, res);
    if (uop) return uop;
    return UOPs.add(new UnaryOperation(UnaryOperationType::UOP_CARDINALITY, arg, res));
}
UnaryOperation* REXBDD::COMPLEMENT(Forest* arg, Forest* res)
{
    if (!arg) return nullptr;
    UnaryOperation* uop = UOPs.find(UnaryOperationType::UOP_COMPLEMENT, arg, res);
    if (uop) return uop;
    return UOPs.add(new UnaryOperation(UnaryOperationType::UOP_COMPLEMENT, arg, res));
}


// ... TBD

// Binary operations
BinaryOperation* REXBDD::UNION(Forest* arg1, Forest* arg2, Forest* res)
{
    if (!arg1 || !arg2) return nullptr;
    // commute
    if (arg1 > arg2) SWAP(arg1, arg2);
    BinaryOperation* bop = BOPs.find(BinaryOperationType::BOP_UNION, arg1, arg2, res);
    if (bop) return bop;
    bop = new BinaryOperation(BinaryOperationType::BOP_UNION, arg1, arg2, res);
    return bop;
}
BinaryOperation* REXBDD::INTERSECTION(Forest* arg1, Forest* arg2, Forest* res)
{
    if (!arg1 || !arg2) return nullptr;
    // commute
    if (arg1 > arg2) SWAP(arg1, arg2);
    BinaryOperation* bop = BOPs.find(BinaryOperationType::BOP_INTERSECTION, arg1, arg2, res);
    if (bop) return bop;
    bop = new BinaryOperation(BinaryOperationType::BOP_INTERSECTION, arg1, arg2, res);
    return bop;
}