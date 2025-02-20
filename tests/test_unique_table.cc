#include "RexBDD.h"

#include <random>
#include <iostream>
#include <cstdint>

const unsigned TESTS=5000000;
const uint16_t MAX_LVL = 100;
const unsigned SEED=12345678;


std::mt19937 gen(SEED);     // Seed the Mersenne Twister engine with the fixed seed
std::uniform_int_distribution<> distrLvl(1, MAX_LVL);
std::uniform_int_distribution<> distrRule(0, 10);
std::uniform_int_distribution<> distrBool(0, 1);
std::uniform_int_distribution<> distrMarks(0, 3);
std::uniform_int_distribution<uint16_t> distr16(0, UINT16_MAX);
std::uniform_int_distribution<uint32_t> distr32(0, UINT32_MAX);
std::uniform_int_distribution<uint64_t> distr64(0, UINT64_MAX);

using namespace REXBDD;

void fill_node(Node& node, bool isMxd)
{
    node.setEdgeRule(0, (ReductionRule)distrRule(gen), isMxd);
    node.setEdgeRule(1, (ReductionRule)distrRule(gen), isMxd);
    node.setChildNodeHandle(0, (NodeHandle)distr16(gen), isMxd);
    node.setChildNodeHandle(1, (NodeHandle)distr16(gen), isMxd);
    node.setEdgeComp(1,distrBool(gen), isMxd);
}

int main()
{
    std::cout<< "Unique table test." << std::endl;

    ForestSetting setting("RexBDD", MAX_LVL);
    Forest* forest = new Forest(setting);
    Node node(forest->getSetting());

    unsigned count = 0, maxHandle = 0;
    for (unsigned i=0; i<TESTS; i++) {
        fill_node(node, forest->getSetting().isRelation());
        NodeHandle h = forest->insertNode(10, node);
        // TBD
        if (h > maxHandle) {
            maxHandle = h;
        } else {
            count++;
        }
        if (forest->getUTEntriesNum(10) + count != i+1) {
            std::cout << "[REXBDD] Test Error! Some nodes are missing!" << std::endl;
            return 1;
        }
    }
    std::cout<<"UT entries number: \t" << forest->getUTEntriesNum(10) 
    << " / " << TESTS << std::endl;
    std::cout<<"Duplicates detected: \t" << count << std::endl;

    // TBD

    
    // std::cout<<"UT entries: "<<node.edgeRule(0)<<std::endl;
    std::cout << "test passed!" << std::endl;
    delete forest;
    return 0;
}