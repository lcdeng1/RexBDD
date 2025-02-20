#include "reductions.h"

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                     Reductions methods                         *
// *                                                                *
// *                                                                *
// ******************************************************************

Reductions::Reductions()
{
    dimension = 1;
    type = REX;
    rules = std::vector<bool>(11,1);
    rules[RULE_I0] = 0;
    rules[RULE_I1] = 0;
}
Reductions::Reductions(const ReductionType reductionType)
{
    setType(reductionType);
}
Reductions::Reductions(const std::vector<bool> &ruleSet)
{
    dimension = 1;
}
Reductions::~Reductions()
{
    //
}