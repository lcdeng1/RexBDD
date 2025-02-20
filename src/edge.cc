#include "edge.h"
#include "terminal.h"

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                      Value  methods                        *
// *                                                                *
// *                                                                *
// ******************************************************************

Value::Value()
{
    valueType = INT;
    intValue = 0;
}
Value::Value(int i)
{
    valueType = INT;
    intValue = i;
}
Value::Value(long l)
{
    valueType = LONG;
    longValue = l;
}
Value::Value(double d)
{
    valueType = DOUBLE;
    doubleValue = d;
}
Value::Value(float f)
{
    valueType = FLOAT;
    floatValue = f;
}

void Value::print(std::ostream& out, int format) const
{
    if (format == 0) {
        //
    } else {
        // more format TBD
    }
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         Edge  methods                          *
// *                                                                *
// *                                                                *
// ******************************************************************

Edge::Edge()
{
    handle = 0;
    value = Value(0);
}
Edge::Edge(const Edge &e)
{
    handle = e.handle;
    value = e.value;
}
Edge::Edge(const EdgeHandle h, Value val):value(val)
{
    //
    handle = h;
}
Edge::~Edge()
{

}

Edge Edge::part(bool xy) const {
    Edge ans;
    ReductionRule rule = unpackRule(handle);
    if ((isRuleEL(rule) || isRuleAL(rule) || isRuleEH(rule) || isRuleAH(rule))
        && (xy == (isRuleEH(rule) || isRuleAH(rule)))) {
        ans.setEdgeHandle(makeTerminal(INT, 0));
        ans.setComp(hasRuleTerminalOne(rule));
        ans.setSwap(0, 0);
    } else if (isRuleI(rule)) {
        // not supported, throw error
        std::cout << "[REXBDD] ERROR!\t Edge::part(bool xy): Not supportted for I edge!"<< std::endl;
        exit(0);
    } else {
        ans = *this;
    }
    ans.setRule(RULE_X);
    return ans;
}

bool Edge::isComplementTo(const Edge& e) const {
    if ((getNodeLevel() == e.getNodeLevel()) && (getNodeLevel() == 0)) {
        if ((!isTerminalOne(handle) && !isTerminalZero(handle)) 
            || (!isTerminalOne(e.handle) && !isTerminalZero(e.handle))) {
            std::cout << "[REXBDD] ERROR!\t Edge::isComplementTo(Edge e): Not supportted for terminal value > 1!"<< std::endl;
            exit(0);
        }
        return (((isTerminalOne(handle) ^ getComp()) != (isTerminalOne(e.handle) ^ e.getComp()))
                && (this->getRule() == e.getRule()));
    }
    if ((getNodeLevel() != e.getNodeLevel()) || (getNodeHandle() != e.getNodeHandle())) return false;
    if ((getSwap(0) != e.getSwap(0)) || (getSwap(1) != e.getSwap(1))) return false;
    return (getComp() != e.getComp()) && (getRule() == compRule(e.getRule()));
}

bool Edge::isConstantOne() const {
    if (getNodeLevel() > 0) return false;
    bool isTermOne = isTerminalOne(handle);
    bool isTermZero = isTerminalZero(handle);
    if (!isTermOne && !isTermZero) return false;
    if (getRule() == RULE_X) return getComp() ^ isTermOne;
    return (hasRuleTerminalOne(getRule()) == (getComp() ^ isTermOne)) && (getComp() ^ isTermOne);
}

bool Edge::isConstantZero() const {
    if (getNodeLevel() > 0) return false;
    bool isTermOne = isTerminalOne(handle);
    bool isTermZero = isTerminalZero(handle);
    if (!isTermOne && !isTermZero) return false;
    if (getRule() == RULE_X) return !(getComp() ^ isTermOne);
    return (hasRuleTerminalOne(getRule()) == (getComp() ^ isTermOne)) && !(getComp() ^ isTermOne);
}

void Edge::print(std::ostream& out, int format) const
{
    printEdgeHandle(handle, out, format);
    // value?
    if (format == 0) {
        //
    } else {
        // more format TBD
    }
}
