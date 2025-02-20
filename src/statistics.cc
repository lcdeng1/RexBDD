#include "statistics.h"

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                      statistics methods                        *
// *                                                                *
// *                                                                *
// ******************************************************************

Statistics::Statistics()
{
    //
    activeNodes = 0;
    peakNodes = 0;
    numOps = 0;
    numOpsTerms = 0;
    numHitsCT = 0;
}
Statistics::~Statistics()
{
    //
}