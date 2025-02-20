#include "node_manager.h"
#include "forest.h"

#include <cstdlib>
#include <random>

const unsigned SEED=123456;


std::mt19937 gen(SEED);     // Seed the Mersenne Twister engine with the fixed seed
std::uniform_int_distribution<> distrRule(0, 10);
std::uniform_int_distribution<> distrBool(0, 1);
std::uniform_int_distribution<uint16_t> distr16(0, UINT16_MAX);
std::uniform_int_distribution<uint32_t> distr32(0, UINT32_MAX);

using namespace REXBDD;

void fill_node(Node& node, bool isMxd)
{
    node.setEdgeRule(0, (ReductionRule)distrRule(gen), isMxd);
    node.setEdgeRule(1, (ReductionRule)distrRule(gen), isMxd);
    node.setChildNodeHandle(0, (NodeHandle)distr16(gen), isMxd);
    node.setChildNodeHandle(1, (NodeHandle)distr16(gen), isMxd);
    node.setEdgeComp(1,distrBool(gen), isMxd);
}

double uniform()
{
    // random returns between 0 and 2^31 - 1
    double x;
    do {
        x = distr32(gen);
    } while (0.0 == x);
    return x / UINT32_MAX;
}

unsigned equilikely(unsigned a, unsigned b)
{
    return a + (unsigned)( (b-a+1) * uniform() );
}

void random_mark(Forest* forest, uint32_t* marklist, unsigned size)
{
    uint16_t level = forest->getSetting().getNumVars();
    for (uint32_t i=0; i<size; i++) {
        marklist[i] = 0;
    }
    uint32_t mlen = 0;
    for (uint32_t i=1; i<=size; i++) {
        if (forest->getNode(level, i).isInUse()) {
            marklist[mlen] = i+1;
            mlen++;
        }
    }
    for (uint32_t i=0; i<mlen; i++) {
        uint32_t j = equilikely(i, mlen-1);
        if (i != j) {
            uint32_t t = marklist[i];
            marklist[i] = marklist[j];
            marklist[j] = t;
        }
    }
}

void alloc_mark_sweep(Forest* forest, unsigned num_a, unsigned num_m, uint32_t* marklist, unsigned size)
{
    int nodeSize = forest->getSetting().nodeSize();
    bool isRel = forest->getSetting().isRelation();
    Node node(nodeSize);
    node.setChildNodeHandle(0, num_a, isRel);
    node.setChildNodeHandle(1, num_m, isRel);
    node.setEdgeRule(0, RULE_AH0, isRel);
    node.setEdgeRule(1, RULE_AH1, isRel);
    /* Allocate nodes */
    // NodeHandle handle = 0;
    uint16_t level = forest->getSetting().getNumVars();
    std::cout << "\tAllocating " << num_a << " nodes at level " << level << std::endl;
    for (unsigned i=num_a; i; i--) {
        // if (i==10) node.setEdgeRule(0, RULE_EL1);   // make a little change
        fill_node(node, isRel);
        NodeHandle h = forest->obtainFreeNodeHandle(level,node);
        if (!forest->getNode(level, h).isEqual(node, nodeSize)) {
            std::cout<<"[REXBDD] Test Error!"<<std::endl;
            exit(0);
        }

    }
    std::cout << "\t\t" << forest->getNodeManUsed(level) << " nodes used" << std::endl;

    /* Randomly mark */
    std::cout << "\tMarking " << num_m << " nodes" << std::endl;
    random_mark(forest, marklist, size);
    for (uint32_t i=0; i<num_m; i++) {
        if (!marklist[i]) continue;
        forest->getNode(level, marklist[i]-1).mark();
    }
    forest->sweepNodeMan(level);
}

unsigned max_marked_plus1(unsigned slots, uint32_t* marklist)
{
    unsigned i, m=0;
    for (i=0; i<slots; i++) {
        if (marklist[i] > m)
            m = marklist[i];
    }
    return m;
}

void check_equal(const char* what, unsigned a, unsigned b)
{
    if (a==b) return;
    printf("%s mismatch: expected %u, was %u\n", what, a, b);
    exit(1);
}

int main()
{
    std::cout<< "Node manager test." << std::endl;

    const unsigned allox[] = {
        16, 32, 1, 64, 128, 256, 512, 1024, 2048, 4092, 8192, 16384,
        32768, 65536, 131072, 262144, 524288, 1048576, 2097152,
        4194394, 8388608, 16770000,       1,       1,       1,
              1,     1,     1,     1,    1,    1,   1,  1,  1, 1, 1, 1, 0
    };
    const unsigned marks[] = {
         8, 12, 12, 1,  80, 300,  42, 1000, 3000, 5000,   10,     1,
        32768, 98304, 100000, 200000, 400000,  100000,   25000,
          12000,    7216, 16777215, 8388607, 4194393, 2097151,
        1048575, 262143, 65535, 16383, 4091, 1023, 255, 63, 15, 3, 1, 1, 0
    };

    uint16_t level = 10;
    ForestSetting setting("RexBDD", level);
    Forest* forest = new Forest(setting);

    unsigned marklistSize = allox[0];
    for (uint32_t i=0; allox[i]; i++) {
        if (i>0) marklistSize = allox[i] + marks[i-1];
        uint32_t* marklist = (uint32_t*)malloc(marklistSize * sizeof(uint32_t));
        alloc_mark_sweep(forest, allox[i], marks[i], marklist, marklistSize);
        check_equal("used nodes", marks[i], forest->getNodeManUsed(level));
        check_equal("first unalloc", max_marked_plus1(marks[i], marklist), forest->getNodeManAlloc(level));
        free(marklist);
    }

    std::cout << "test passed!" << std::endl;
    delete forest;
    return 0;
}