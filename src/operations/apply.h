#ifndef REXBDD_APPLY_H
#define REXBDD_APPLY_H

#include "../defines.h"
#include "../setting.h"
#include "../forest.h"
#include "operation.h"
#include "operations_generator.h"

namespace REXBDD {
    /* Unary */
    typedef UnaryOperation* (*UnaryBuiltin1)(Forest* arg, Forest* res);
    typedef UnaryOperation* (*UnaryBuiltin2)(Forest* arg, OpndType res);
    /* Binary */
    typedef BinaryOperation* (*BinaryBuiltin1)(Forest* arg1, Forest* arg2, Forest* res);
    typedef BinaryOperation* (*BinaryBuiltin2)(Forest* arg1, OpndType arg2, Forest* res);

    // ******************************************************************
    // *                          Unary  apply                          *
    // ******************************************************************
    inline void apply(UnaryBuiltin1 ub, const Func& arg, Func& res)
    {
        UnaryOperation* uop = ub(arg.getForest(), res.getForest());
        uop->compute(arg, res);
    }
    inline void apply(UnaryBuiltin2 ub, const Func& arg, long& res)
    {
        UnaryOperation* uop = ub(arg.getForest(), OpndType::INTEGER);
        uop->compute(arg, res);
    }
    inline void apply(UnaryBuiltin2 ub, const Func& arg, double& res)
    {
        UnaryOperation* uop = ub(arg.getForest(), OpndType::REAL);
        uop->compute(arg, res);
    }
    // ******************************************************************
    // *                         Binary  apply                          *
    // ******************************************************************
    inline void apply(BinaryBuiltin1 bb, const Func& arg1, const Func& arg2, Func& res)
    {
        BinaryOperation* bop = bb(arg1.getForest(), arg2.getForest(), res.getForest());
        bop->compute(arg1, arg2, res);
    }
    inline void apply(BinaryBuiltin2 bb, const Func& arg1, const ExplictFunc& arg2, Func& res)
    {
        BinaryOperation* bop = bb(arg1.getForest(), OpndType::EXPLICIT_FUNC, res.getForest());
        bop->compute(arg1, arg2, res);
    }
};

#endif