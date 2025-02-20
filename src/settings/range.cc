#include "range.h"

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                          Range methods                         *
// *                                                                *
// *                                                                *
// ******************************************************************

Range::Range(const RangeType range, const ValueType val)
{
    //
    rangeType = range;
    valueType = val;
    maxRange = 1;   // default for boolean; used to complement terminal
    negInf = 0;
    posInf = 0;
    unDef = 0;

}
Range::Range(unsigned long size)
{
    //
    rangeType = (size == 1) ? BOOLEAN : FINITE;
    valueType = (size == 1) ? INT : LONG;
    maxRange = size;
    negInf = 0;
    posInf = 0;
    unDef = 0;
}
Range::~Range()
{
    //
}