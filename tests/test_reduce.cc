#include "RexBDD.h"

using namespace REXBDD;

Edge buildSetEdge(Forest* forest,
                uint16_t lvl,
                std::vector<bool>& fun,
                int start, int end)
{
    std::vector<Edge> child(2);
    EdgeLabel label = 0;
    packRule(label, RULE_X);
    if (lvl == 1) {
        child[0].setEdgeHandle(makeTerminal(INT, fun[start]?1:0));
        if (forest->getSetting().getValType() == FLOAT) {
            child[0].setEdgeHandle(makeTerminal(FLOAT, fun[start]?1.0f:0.0f));
        }
        child[1].setEdgeHandle(makeTerminal(INT, fun[end]?1:0));
        if (forest->getSetting().getValType() == FLOAT) {
            child[1].setEdgeHandle(makeTerminal(FLOAT, fun[end]?1.0f:0.0f));
        }
        child[0].setRule(RULE_X);
        child[1].setRule(RULE_X);
        return forest->reduceEdge(lvl, label, lvl, child);
    }
    child[0] = buildSetEdge(forest, lvl-1, fun, start, start+(1<<(lvl-1))-1);
    child[1] = buildSetEdge(forest, lvl-1, fun, start+(1<<(lvl-1)), end);
    return forest->reduceEdge(lvl, label, lvl, child);
}

bool buildSetForest(uint16_t num, PredefForest bdd)
{
    // test up to 5 variables
    if (num > 5) return 0;
    ForestSetting setting(bdd, num);
    Forest* forest = new Forest(setting);
    forest->getSetting().output(std::cout);
    bool ans = 0;
    // variables
    std::vector<std::vector<bool> > vars(1<<num, std::vector<bool>(num+1, false));
    for (int i=0; i<(1<<num); i++) {
        for (uint16_t k=1; k<=num; k++){
            vars[i][k] = i & (1<<(k-1));
        }
    }
    // function values
    Func function(forest);
    std::vector<std::vector<bool> > funs(1<<(1<<num), std::vector<bool>(1<<num, false));
    for (int i=0; i<(1<<(1<<num)); i++) {
        for (int k=0; k<(1<<num); k++) {
            funs[i][k] = i & (1<<k);
        }
        // build function
        Edge edge = buildSetEdge(forest, num, funs[i], 0, (1<<num)-1);
        function.setEdge(edge);
        // evaluate function
        for (int j=0; j<(1<<num); j++) {
            Value eval = function.evaluate(vars[j]);
            union
            {
                int valInt;
                float valFloat;
            };
            eval.getValueTo(&valInt, INT);
            if (valInt != funs[i][j]) {
                std::cout<<"Evaluation Failed for " << num << " variable(s), function " << i << std::endl;
                std::cout<<"\t assignment: ";
                for (uint16_t k=1; k<=num; k++){
                    std::cout << vars[j][k] << " ";
                }
                std::cout << "; value shoud be: " << funs[i][j] << "; was: " << valInt << std::endl;
                std::cout<<"\t full function: ";
                for (int k=0; k<(1<<num); k++){
                    std::cout << funs[i][k] << " ";
                }
                std::cout << std::endl;

                DotMaker dot(forest, "error_function");
                dot.buildGraph(function);
                dot.runDot("pdf");
                return 0;
            }
            
        }
    }
    ans = 1;
    for (uint16_t i=1; i<=num; i++) {
        std::cout << "Number of nodes at level " << i << ": " << forest->getNodeManUsed(i) << std::endl;
    }
    delete forest;
    return ans;
}

int main(int argc, char** argv)
{
    uint16_t num;
    PredefForest bdd;
    if (argc == 3) {
        num = atoi(argv[2]);
        bdd = (PredefForest)atoi(argv[1]);
    } else {
        num = 4;
        bdd = PredefForest::REXBDD;
    }

    std::cout<< "ReduceEdge test." << std::endl;

    bool pass = buildSetForest(num, bdd);

    if (!pass) {
        std::cout << "Test Failed!" << std::endl;
    } else {
        std::cout << "Test Pass!" << std::endl;
    }
    return pass;
}