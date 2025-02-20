#include "function.h"
#include "forest.h"
#include "node.h"

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                         Func  methods                          *
// *                                                                *
// *                                                                *
// ******************************************************************
Func::Func()
{
    //
}
Func::Func(Forest* f)
{
    parent = f;
    name = "";
    prevFunc = 0;
    nextFunc = 0;
}
Func::Func(Forest* f, const Edge& e)
:edge(e)
{
    parent = f;
    name = "";
    prevFunc = 0;
    nextFunc = 0;
}
Func::~Func()
{
    //
}

/***************************** General **************************/

/**************************** Make edge *************************/
void Func::trueFunc()
{
    /* Don't care the value on edge */
    edge.handle = makeTerminal(INT, 1);
    if (parent->setting.getValType() == FLOAT) {
        edge.handle = makeTerminal(FLOAT, 1.0f);
    }
    packRule(edge.handle, RULE_X);
    edge = parent->normalizeEdge(parent->setting.getNumVars(), edge);
}
void Func::falseFunc()
{
    edge.handle = makeTerminal(INT, 0);
    if (parent->setting.getValType() == FLOAT) {
        edge.handle = makeTerminal(FLOAT, 0.0f);
    }
    packRule(edge.handle, RULE_X);
    edge = parent->normalizeEdge(parent->setting.getNumVars(), edge);
}
/* For dimention 1 and 2 */
void Func::constant(Value val)
{
    if (parent->setting.getEncodeMechanism() == TERMINAL) {
        //
    } else {
        //
    }
}
/* For dimention of 2 (Relation) */
void Func::identity(std::vector<bool> dependance)
{
    // TBD
}
/* For dimention of 1 (Set) */
void Func::variable(uint16_t lvl)
{
    // TBD
}
void Func::variable(uint16_t lvl, Value low, Value high)
{
    // TBD
}
/* For dimention of 2 (Relation) */
// Variable Func
void Func::variable(uint16_t lvl, bool isPrime)
{
    // TBD
}
void Func::variable(uint16_t lvl, bool isPrime, Value low, Value high)
{
    // TBD
}

Value Func::evaluate(const std::vector<bool>& assignment) const
{
    /* check the level */
    if (parent->getSetting().getNumVars() != (assignment.size()-1)) {
        std::cout << "[REXBDD] ERROR!\t Func::evaluate(): Variable number check failed in evaluation! It was "<<assignment.size()-1
        <<", it should be "<<parent->getSetting().getNumVars() << std::endl;
        exit(0);
    }
    /* final answer */
    Value ans(0);
    /* encode mechanism */
    EncodeMechanism encode = parent->getSetting().getEncodeMechanism();
    /* edge flags type */
    SwapSet st = parent->getSetting().getSwapType();
    CompSet ct = parent->getSetting().getCompType();

    /* tmp store edge info for "for" loop */
    Edge current = edge;
    /* get target node info */
    NodeHandle targetHandle = current.getNodeHandle();
    uint16_t targetLvl = current.getNodeLevel();
    /* info to determine next child edge */
    bool isComp = 0, isSwap = 0;
    /* flags for reduction rules */
    bool allOne = 1, existOne = 0;
    /* evaluation starting from the target node level */
    uint16_t k = assignment.size()-1;
    while (k >= 0) {
#ifdef BRAVE_DD_TRACE
        std::cout<<"evaluate k: " << k;
        std::cout<<"; currt: ";
        current.print(std::cout, 0);
        std::cout << std::endl;
#endif
        /* check the incoming edge's reduction rule for terminal cases */
        ReductionRule incoming = current.getRule();
        /* if incoming edge skips levels */
        if ((targetLvl < k) && (incoming != RULE_X)) {
            // determine flags of all-ones and exist-ones
            for (uint16_t i=k; i>targetLvl; i--) {
                allOne &= assignment[i];
                existOne |= assignment[i];
            }
            if (encode == TERMINAL) {
                // terminal value, don't care the Value on edge
                ValueType vt = (parent->getSetting().getValType() == INT
                                || parent->getSetting().getValType() == LONG) ? INT : FLOAT;
                if ((allOne && isRuleAH(incoming))
                    || ((!allOne) && isRuleEL(incoming))
                    || (existOne && isRuleEH(incoming))
                    || ((!existOne) && isRuleAL(incoming))) {
                    if (vt == INT) ans.setValue(hasRuleTerminalOne(incoming)?1:0, INT);
                    else ans.setValue(hasRuleTerminalOne(incoming)?1.0f:0.0f, FLOAT);
                    return ans;
                }
            } else if (encode == EDGE_PLUS) {
                // edge values plus
                // TBD
            } else if (encode == EDGE_PLUSMOD) {
                // edge values plus and modulo
                // TBD
            } else if (encode == EDGE_MULT) {
                // edge values multiply
                // TBD
            }
        }
        if (targetLvl > 0) {
            /* short incoming edge, or long incoming edge but skips */
            k = targetLvl-1;
            isSwap = (st==ONE || st==ALL) ? current.getSwap(0) : 0;
            // get swap/comp bit only when it's allowed, since user may insert illegal nodes into nodemanager (which is allowed)
            isComp = (ct==COMP) ? current.getComp() : 0;
            current = parent->getChildEdge(targetLvl, targetHandle, isSwap^assignment[targetLvl]);
            if (isComp) current.complement();
            if (isSwap && st==ALL) current.swap();  // for swap-all
            /* update varibles */
            allOne = 1;
            existOne = 0;
            targetHandle = current.getNodeHandle();
            targetLvl = current.getNodeLevel();
#ifdef BRAVE_DD_TRACE
            std::cout<<"next currt: k="<< k <<", targetlvl=" << targetLvl << "; ";
            current.print(std::cout, 0);
            std::cout << std::endl;
#endif
            continue;
        }
        // not reach the terminal cases of reduction rules, or got the next edge
        if (targetLvl == 0) {
            // value type INT, FLOAT, or VOID (special value)
            ans = getTerminalValue(current.handle);
            if (current.getComp() && ct != NO_COMP) {
                if (ans.valueType == INT) {
                    int terminalVal = *reinterpret_cast<int*>(&targetHandle);
                    terminalVal = parent->getSetting().getMaxRange() - terminalVal; // complement if needed
                    ans.setValue(terminalVal, INT);
                } else if (ans.valueType == FLOAT) {
                    float terminalVal = *reinterpret_cast<float*>(&targetHandle);
                    terminalVal = parent->getSetting().getMaxRange() - terminalVal; // complement if needed
                    ans.setValue(terminalVal, FLOAT);
                } else if (ans.valueType == VOID) {
                    // special value: NegInf => PosInf?
                    // TBD
                }
            }
            return ans;
        }
        k--;
    }
    return ans;
}

Value Func::evaluate(const std::vector<bool>& aFrom, const std::vector<bool>& aTo) const
{
    /* check the level */
    if ((parent->getSetting().getNumVars() != (aFrom.size()-1)) || (aFrom.size() != aTo.size())) {
        std::cout << "[REXBDD] ERROR!\t Func::evaluate(): Variable number check failed in evaluation! It was "<<aFrom.size()-1
        <<", it should be "<<parent->getSetting().getNumVars() << std::endl;
        exit(0);
    }
    /* final answer */
    Value ans(0);
    // TBD
    return ans;
}

void Func::unionAssignments(const ExplictFunc& assignments) {
    // check applicability based on setting TBD <== relation? levels?
        // throw error(ErrCode::INVALID_BOUND, __FILE__, __LINE__);
    std::cout<<"Not implemented..."<<std::endl;
    // TBD
}


/* Expert function for union assignments TBD */
Edge Func::unionAssignmentRecursive(uint16_t n, Edge& root, ExplictFunc assignments)
{
    /* declare the final answer */
    Edge ans;
    /* terminal case */
    if (n==0) {
        // return the edge pointing to terminal node
    }
    /*
     * here means the root edge can not capture the assignment,
     * then we do it recursively!
     *      we need to expand root edge if it's a long edge
     */
    uint16_t skipLvl;
    skipLvl = n - unpackLevel(root.handle);
    /* determine the down edges */
    //
    if (skipLvl == 0) {
        // it is a short edge, they are the child edges of root target node
    } else {
        // it is a long edge, expand based on its reduction rule if needed <==== helper function?
    }
    /*
     * build new node at level n, child edges are down edges or recursive calls based on assignment[n]
     */
    // temporary node information 
    /* reduce */
    // parent->reduceEdge
    return ans;
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         FuncArray  methods                     *
// *                                                                *
// *                                                                *
// ******************************************************************
FuncArray::FuncArray()
{
    //
}
FuncArray::FuncArray(Forest* f, int size)
{
    //
}
FuncArray::~FuncArray()
{
    //
}

void FuncArray::add(Func f)
{
    // TBD
}