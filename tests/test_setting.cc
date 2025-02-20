#include "RexBDD.h"

using namespace REXBDD;

int main(int argc, char** argv) {
    printInfo();
    std::string bdd;
    if (argc == 1) bdd = "rexbdd";
    else bdd = argv[1];

    // basic test case
    unsigned numVals = 10;
    // RangeType rangeType = FINITE;
    // ValueType valueType = INT;
    // int maxRange = 20;
    // bool hasNegInf = 0;
    // bool hasPosInf = 0;
    // bool hasUnDef = 0;
    // EncodeMechanism encode = TERMINAL;
    // int dim = 1;
    // ReductionType reductionType = FULLY;
    // SwapSet swapType = ONE;
    // CompSet CompSet = NO_COMP;

    // default setting
    ForestSetting setting1(bdd, numVals);

    // build forest
    Forest forest1(setting1);
    
    // change setting1 totally different from forest1
    setting1.setNumVars(5);                    // Number of variables
    setting1.setRangeType(FINITE);             // Range type
    setting1.setValType(INT);                  // Value type
    setting1.setMaxRange(6);                   // Max range, N
    setting1.setNegInf(1);                     // Negative infinity
    setting1.setPosInf(0);                     // Positive infinity
    setting1.setUnDef(0);                      // Undefined
    setting1.setEncodeMechanism(EDGE_PLUS);    // Encoding mechanism
    setting1.setDim(2);                        // Dimension
    setting1.setReductionType(FULLY);          // Reudction type
    setting1.addReductionRule(RULE_I0);
    setting1.addReductionRule(RULE_I1);
    // ^ may add or delete rules here          // Add/delete reduction rules
    setting1.setSwapType(NO_SWAP);             // Swap flag
    setting1.setCompType(NO_COMP);             // Complement flag
    setting1.setMergeType(PUSH_DOWN);          // Merge type (this will be removed)
    setting1.setName("v5f6-EVBDD");            // Name
    setting1.output(std::cout, 0);
    forest1.getSetting().output(std::cout, 0);

    if (forest1.getSetting().getNumVars() == setting1.getNumVars()) {
        std::cout<< "Error!" << std::endl;
        exit(1);
    }


    return 0;
}