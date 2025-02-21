#include "operation.h"
#include "../IO/out_dot.h"

// #define REXBDD_TRACE_OPERATION

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                                                                *
// *                      Operation  methods                        *
// *                                                                *
// *                                                                *
// ******************************************************************

Operation::Operation()
{
    //
}
Operation::~Operation()
{
    //
}

// ******************************************************************
// *                                                                *
// *                    UnaryOperation  class                       *
// *                                                                *
// ******************************************************************
UnaryOperation::UnaryOperation(UnaryOperationType type, Forest* source, Forest* target)
:opType(type)
{
    sourceForest = source;
    targetForest = target;
    targetType = OpndType::FOREST;
}
UnaryOperation::UnaryOperation(UnaryOperationType type, Forest* source, OpndType target)
:opType(type)
{
    sourceForest = source;
    targetForest = source;
    targetType = target;
}
UnaryOperation::~UnaryOperation()
{
    cache.~ComputeTable();
}

void UnaryOperation::compute(const Func& source, Func& target)
{
    if (!checkForestCompatibility()) {
        throw error(ErrCode::INVALID_OPERATION, __FILE__, __LINE__);
    }
    uint16_t numVars = sourceForest->getSetting().getNumVars();
    Edge ans;
    // copy to target forest
    ans = computeCOPY(numVars, source.getEdge());
    if (opType == UnaryOperationType::UOP_COMPLEMENT) {
        // target forest allows complement flag
        if ((targetForest->getSetting().getCompType() != NO_COMP)
        && (targetForest->getSetting().getEncodeMechanism() == TERMINAL)) {
            ans.complement();
            // normalize edge
            if (!targetForest->getSetting().hasReductionRule(ans.getRule())) {
                ans = targetForest->normalizeEdge(numVars, ans);
            }
            target.setEdge(ans);
            return;
        }
        // here is the forest that does not allow complement bit, recursively compute
        ans = computeCOMPLEMENT(numVars, ans);
    } else {
        // TBD
    }
    target.setEdge(ans);
    // other info TBD
}
void UnaryOperation::compute(const Func& source, long& target)
{
    if (!checkForestCompatibility()) {
        throw error(ErrCode::INVALID_OPERATION, __FILE__, __LINE__);
    }
    // TBD
}
void UnaryOperation::compute(const Func& source, double& target)
{
    if (!checkForestCompatibility()) {
        throw error(ErrCode::INVALID_OPERATION, __FILE__, __LINE__);
    }
    // TBD
}
bool UnaryOperation::checkForestCompatibility() const
{
    bool ans = 1;
    // TBD
    return ans;
}
Edge UnaryOperation::computeCOPY(const uint16_t lvl, const Edge& source)
{
    // Direct return if the target forest is the source forest
    if (sourceForest == targetForest) {
        return source;
    }
    // Terminal case

    // check compute table

    Edge ans;
    //TBD
    return ans;
}
Edge UnaryOperation::computeCOMPLEMENT(const uint16_t lvl, const Edge& source)
{
    /* Assuming this is within the same target forest
     Here is forest that does not allow complement bit */
    
    Edge ans;    
    // terminal case
    if (source.getNodeLevel() == 0) {
        ans = source;
        ans.complement();
        return targetForest->normalizeEdge(lvl, ans);
    } else {
        // check cache, TBD
        std::vector<Edge> childEdges;
        if (targetForest->getSetting().isRelation()) {
            childEdges = std::vector<Edge>(4);
        } else {
            childEdges = std::vector<Edge>(2);
        }
        for (char i=0; (size_t)i<childEdges.size(); i++) {
            childEdges[i] = targetForest->getChildEdge(source.getNodeLevel(), source.getNodeHandle(), i);
            childEdges[i] = computeCOMPLEMENT(source.getNodeLevel()-1, childEdges[i]);
        }
        EdgeLabel label = 0;
        packRule(label, compRule(source.getRule()));
        ans = targetForest->reduceEdge(lvl, label, source.getNodeLevel(), childEdges, source.getValue());
    }
    // cache, TBD
    return ans;
}


// ******************************************************************
// *                                                                *
// *                       UnaryList  methods                       *
// *                                                                *
// ******************************************************************
UnaryList::UnaryList(const std::string n)
{
    reset(n);
}

UnaryOperation* UnaryList::mtfUnary(const UnaryOperationType opT, const Forest* sourceF, const Forest* targetF)
{
    UnaryOperation* prev = front;
    UnaryOperation* curr = front->next;
    while (curr) {
        if ((curr->opType == opT) && (curr->sourceForest == sourceF) && (curr->targetForest == targetF)) {
            // Move to front
            prev->next = curr->next;
            curr->next = front;
            front = curr;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

UnaryOperation* UnaryList::mtfUnary(const UnaryOperationType opT, const Forest* sourceF, const OpndType targetT)
{
    UnaryOperation* prev = front;
    UnaryOperation* curr = front->next;
    while (curr) {
        if ((curr->opType == opT) && (curr->sourceForest == sourceF) && (curr->targetType == targetT)) {
            // Move to front
            prev->next = curr->next;
            curr->next = front;
            front = curr;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

void UnaryList::searchRemove(UnaryOperation* uop)
{
    if (!front) return;
    UnaryOperation* prev = front;
    UnaryOperation* curr = front->next;
    while (curr) {
        if (curr == uop) {
            prev->next = curr->next;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                 BinaryOperation  methods                       *
// *                                                                *
// *                                                                *
// ******************************************************************
BinaryOperation::BinaryOperation(BinaryOperationType type, Forest* source1, Forest* source2, Forest* res)
:opType(type)
{
    source1Forest = source1;
    source2Forest = source2;
    resForest = res;
}
BinaryOperation::~BinaryOperation()
{
    cache.~ComputeTable();
}

void BinaryOperation::compute(const Func& source1, const Func& source2, Func& res)
{
    if (!checkForestCompatibility()) {
        throw error(ErrCode::INVALID_OPERATION, __FILE__, __LINE__);
    }
    Edge ans;
    Func source1Equ;
    Func source2Equ;
    uint16_t numVars = resForest->getSetting().getNumVars();
    // Unary operation to copy from source1 forest to target forest
    UnaryOperation* cp1 = UOPs.find(UnaryOperationType::UOP_COPY, source1.getForest(), res.getForest());
    if (!cp1) {
        cp1 = UOPs.add(new UnaryOperation(UnaryOperationType::UOP_COPY, source1.getForest(), res.getForest()));
    }
    // copy sources to the same target forest, if they are both set-of-states or relation
    if (source1Forest->getSetting().isRelation() == source2Forest->getSetting().isRelation()) {
        source1Equ = Func(res.getForest());
        cp1->compute(source1, source1Equ);
        source2Equ = Func(res.getForest());
        UnaryOperation* cp2 = UOPs.find(UnaryOperationType::UOP_COPY, source2.getForest(), res.getForest());
        if (!cp2) {
            cp2 = UOPs.add(new UnaryOperation(UnaryOperationType::UOP_COPY, source2.getForest(), res.getForest()));
        }
        cp2->compute(source2, source2Equ);
    }
    // compute the result
    if (opType == BinaryOperationType::BOP_UNION) {
        ans = computeUNION(numVars, source1Equ.getEdge(), source2Equ.getEdge());
    } else if (opType == BinaryOperationType::BOP_INTERSECTION) {
        ans = computeINTERSECTION(numVars, source1Equ.getEdge(), source2Equ.getEdge());
    } else if (opType == BinaryOperationType::BOP_PREIMAGE) {
        ans = computeIMAGE(numVars, source1.getEdge(), source2.getEdge(), 1);
        Func ansEqu(source1.getForest(), ans);
        cp1->compute(ansEqu, res);
        return;
    } else if (opType == BinaryOperationType::BOP_POSTIMAGE) {
        ans = computeIMAGE(numVars, source1.getEdge(), source2.getEdge());
        Func ansEqu(source1.getForest(), ans);
        cp1->compute(ansEqu, res);
        return;
    } else {
        // TBD
    }
    // passing result
    res.setEdge(ans);
    cache.reportStat(std::cout);
}

void BinaryOperation::compute(const Func& source1, const ExplictFunc source2, Func& res)
{
    //
}

bool BinaryOperation::checkForestCompatibility() const
{
    bool ans = 1;
    // TBD
    return ans;
}
Edge BinaryOperation::computeUNION(const uint16_t lvl, const Edge& source1, const Edge& source2)
{
    Edge ans;
    Edge e1, e2;
    // normalize edges
    e1 = resForest->normalizeEdge(lvl, source1);
    e2 = resForest->normalizeEdge(lvl, source2);

    // Base case 1: two edges are the same
    if (e1 == e2) return e1;
    // Base case 2: two edges are complemented
    // Base case 3: one edge is constant ONE edge
    if (e1.isComplementTo(e2) || e1.isConstantOne() || e2.isConstantOne()) {
        EdgeHandle constant = makeTerminal(INT, 1);
        if (resForest->getSetting().getValType() == FLOAT) {
            constant = makeTerminal(FLOAT, 1.0f);
        }
        ans.setEdgeHandle(constant);
        ans = resForest->normalizeEdge(lvl, ans);
        return ans;
    }
    // Base case 4: one edge is constant ZERO edge
    if (e1.isConstantZero()) return e2;
    if (e2.isConstantZero()) return e1;

    uint16_t m1, m2;
    m1 = e1.getNodeLevel();
    m2 = e2.getNodeLevel();
    // ordering
    if (m1<m2) {
        SWAP(e1, e2);
        SWAP(m1, m2);
    }
    
    // check cache here
    if (cache.check(lvl, e1, e2, ans)) return ans;

    // Case that edge1 is a short edge
    if (m1 == lvl) {
        Edge x1, y1, x2, y2;
        x1 = resForest->cofact(lvl, e1, 0);
        y1 = resForest->cofact(lvl, e1, 1);
        x2 = resForest->cofact(lvl, e2, 0);
        y2 = resForest->cofact(lvl, e2, 1);
        std::vector<Edge> child(2);
        child[0] = computeUNION(lvl-1, x1, x2);
        child[1] = computeUNION(lvl-1, y1, y2);
        EdgeLabel root = 0;
        packRule(root, RULE_X);
        ans = resForest->reduceEdge(lvl, root, lvl, child);

        // save to cache
        cache.add(lvl, e1, e2, ans);
        return ans;
    }

    // Here we have m1>=m2, it's time to decide pattern types and use pattern operations
    char t1, t2;
    t1 = rulePattern(e1.getRule());
    t2 = rulePattern(e2.getRule());
    if (t1 == 'L') {
        if (t2 == 'L' || t2 == 'U') {
            ans = operateLL(lvl, e1, e2);
        } else {
            ans = operateLH(lvl, e1, e2);
        }
    } else if (t1 == 'H') {
        if (t2 == 'H' || t2 == 'U') {
            ans = operateHH(lvl, e1, e2);
        } else {
            ans = operateLH(lvl, e2, e1);
        }
    } else {
        if (t2 == 'L' || t2 == 'U') {
            ans = operateLL(lvl, e1, e2);
        } else {
            ans = operateHH(lvl, e1, e2);
        }
    }
    // save cache
    cache.add(lvl, e1, e2, ans);
    return ans;
}

Edge BinaryOperation::computeINTERSECTION(const uint16_t lvl, const Edge& source1, const Edge& source2)
{
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "compute INTERSECTION: lvl: " << lvl << "; e1: ";
    source1.print(std::cout);
    std::cout << "; e2: ";
    source2.print(std::cout);
    std::cout << std::endl;
#endif

    Edge ans;
    Edge e1, e2;
    // normalize edges
    e1 = resForest->normalizeEdge(lvl, source1);
    e2 = resForest->normalizeEdge(lvl, source2);

    // Base case 1: two edges are the same
    if (e1 == e2) return e1;
    // Base case 2: two edges are complemented
    // Base case 3: one edge is constant ZERO edge
    if (e1.isComplementTo(e2) || e1.isConstantZero() || e2.isConstantZero()) {
        EdgeHandle constant = makeTerminal(INT, 0);
        if (resForest->getSetting().getValType() == FLOAT) {
            constant = makeTerminal(FLOAT, 0.0f);
        }
        ans.setEdgeHandle(constant);
        ans = resForest->normalizeEdge(lvl, ans);
        return ans;
    }
    // Base case 4: one edge is constant ONE edge
    if (e1.isConstantOne()) return e2;
    if (e2.isConstantOne()) return e1;

    uint16_t m1, m2;
    m1 = e1.getNodeLevel();
    m2 = e2.getNodeLevel();
    // ordering
    if (m1 < m2) {
        SWAP(e1, e2);
        SWAP(m1, m2);
    }
    
    // check cache here
    if (cache.check(lvl, e1, e2, ans)) return ans;

    // Case that edge1 is a short edge
    if (m1 == lvl) {
        Edge x1, y1, x2, y2;
        x1 = resForest->cofact(lvl, e1, 0);
        y1 = resForest->cofact(lvl, e1, 1);
        x2 = resForest->cofact(lvl, e2, 0);
        y2 = resForest->cofact(lvl, e2, 1);
        std::vector<Edge> child(2);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "\trecursive 0 from level: " << lvl << std::endl;
#endif
        child[0] = computeINTERSECTION(lvl-1, x1, x2);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "\trecursive 1 from level: " << lvl << std::endl;
#endif
        child[1] = computeINTERSECTION(lvl-1, y1, y2);
        EdgeLabel root = 0;
        packRule(root, RULE_X);
        ans = resForest->reduceEdge(lvl, root, lvl, child);

        // save to cache
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "\tcase 1: save to cache: ";
    ans.print(std::cout);
    std::cout << std::endl;
#endif
        cache.add(lvl, e1, e2, ans);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "\tsave done\n";
#endif
        return ans;
    }

    // Here we have m1>=m2, it's time to decide pattern types and use pattern operations
    char t1, t2;
    t1 = rulePattern(e1.getRule());
    t2 = rulePattern(e2.getRule());
    if (t1 == 'L') {
        if (t2 == 'L' || t2 == 'U') {
            ans = operateLL(lvl, e1, e2);
        } else {
            ans = operateLH(lvl, e1, e2);
        }
    } else if (t1 == 'H') {
        if (t2 == 'H' || t2 == 'U') {
            ans = operateHH(lvl, e1, e2);
        } else {
            ans = operateLH(lvl, e2, e1);
        }
    } else {
        if (t2 == 'L' || t2 == 'U') {
            ans = operateLL(lvl, e1, e2);
        } else {
            ans = operateHH(lvl, e1, e2);
        }
    }
    // save cache
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "\tcase 2: save to cache: ";
    ans.print(std::cout);
    std::cout << std::endl;
#endif
    cache.add(lvl, e1, e2, ans);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "\tsave done\n";
#endif
    return ans;
}
Edge BinaryOperation::computeIMAGE(const uint16_t lvl, const Edge& source1, const Edge& trans, bool isPre)
{
    //
    Edge ans;
    //TBD
    return ans;
}
Edge BinaryOperation::operateLL(const uint16_t lvl, const Edge& e1, const Edge& e2)
{
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "operateLL: lvl: " << lvl << "; e1: ";
    e1.print(std::cout);
    std::cout << "; e2: ";
    e2.print(std::cout);
    std::cout << std::endl;
#endif

    uint16_t m1, m2;
    m1 = e1.getNodeLevel();
    m2 = e2.getNodeLevel();

    Edge x1, x2, y1, y2;
    x1 = e1.part(0);
    x2 = e2.part(0);
    y1 = e1.part(1);
    y2 = (m1==m2) ? e2.part(1) : resForest->cofact(m1+1, e2, 1);

    Edge x, y;
    if (opType == BinaryOperationType::BOP_UNION) {
        x = computeUNION(m1, x1, x2);
        y = computeUNION(m1, y1, y2);
    } else if (opType == BinaryOperationType::BOP_INTERSECTION) {
        x = computeINTERSECTION(m1, x1, x2);
        y = computeINTERSECTION(m1, y1, y2);
    }
    // more operations TBD
    Edge ans = resForest->buildHalf(lvl, m1+1, x, y, 0);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "build Low with x: ";
    x.print(std::cout);
    std::cout << "; y: ";
    y.print(std::cout);
    std::cout << std::endl;
    std::cout << "after operateHH from lvl: " << lvl << "; result: ";
    ans.print(std::cout);
    std::cout << std::endl;
#endif
    return ans;

}
Edge BinaryOperation::operateHH(const uint16_t lvl, const Edge& e1, const Edge& e2)
{
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "operateHH: lvl: " << lvl << "; e1: ";
    e1.print(std::cout);
    std::cout << "; e2: ";
    e2.print(std::cout);
    std::cout << std::endl;
#endif
    uint16_t m1, m2;
    m1 = e1.getNodeLevel();
    m2 = e2.getNodeLevel();

    Edge x1, x2, y1, y2;
    x1 = e1.part(0);
    x2 = (m1==m2) ? e2.part(0) : resForest->cofact(m1+1, e2, 0);
    y1 = e1.part(1);
    y2 = e2.part(1);

    Edge x, y;
    if (opType == BinaryOperationType::BOP_UNION) {
        x = computeUNION(m1, x1, x2);
        y = computeUNION(m1, y1, y2);
    } else if (opType == BinaryOperationType::BOP_INTERSECTION) {
        x = computeINTERSECTION(m1, x1, x2);
        y = computeINTERSECTION(m1, y1, y2);
    }
    // more operations TBD
    Edge ans = resForest->buildHalf(lvl, m1+1, x, y, 0);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "build High with x: ";
    x.print(std::cout);
    std::cout << "; y: ";
    y.print(std::cout);
    std::cout << std::endl;
    std::cout << "after operateHH from lvl: " << lvl << "; result: ";
    ans.print(std::cout);
    std::cout << std::endl;
#endif
    return ans;

}
Edge BinaryOperation::operateLH(const uint16_t lvl, const Edge& e1, const Edge& e2)
{
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "operateLH: lvl: " << lvl << "; e1: ";
    e1.print(std::cout);
    std::cout << "; e2: ";
    e2.print(std::cout);
    std::cout << std::endl;
#endif
    uint16_t m1, m2, m;
    m1 = e1.getNodeLevel();
    m2 = e2.getNodeLevel();

    Edge x1, x2, y1, y2;
    x1 = e1.part(0);
    y2 = e2.part(1);
    if (m1 == m2) {
        y1 = e1.part(1);
        x2 = e2.part(0);
        m = m1;
    } else if (m1>m2) {
        y1 = e1.part(1);
        x2 = resForest->cofact(m1+1, e2, 0);
        m = m1;
    } else {
        y1 = resForest->cofact(m2+1, e1, 1);
        x2 = e2.part(0);
        m = m2;
    }
    Edge x, y, z;
    if (opType == BinaryOperationType::BOP_UNION) {
        x = computeUNION(m, x1, x2);
        z = computeUNION(m, y1, y2);
        if (lvl - m == 1) {
            EdgeLabel root = 0;
            packRule(root, RULE_X);
            std::vector<Edge> child(2);
            child[0] = x;
            child[1] = z;
            return resForest->reduceEdge(lvl, root, lvl, child);
        }
        y = computeUNION(m, x1, y2);
    } else if (opType == BinaryOperationType::BOP_INTERSECTION) {
        x = computeINTERSECTION(m, x1, x2);
        z = computeINTERSECTION(m, y1, y2);
        if (lvl - m == 1) {
            EdgeLabel root = 0;
            packRule(root, RULE_X);
            std::vector<Edge> child(2);
            child[0] = x;
            child[1] = z;
            return resForest->reduceEdge(lvl, root, lvl, child);
        }
        y = computeINTERSECTION(m, x1, y2);
    }
    // more operations TBD
    Edge ans = resForest->buildUmb(lvl, m+1, x, y, z);
#ifdef REXBDD_TRACE_OPERATION
    std::cout << "build Umbrella with x: ";
    x.print(std::cout);
    std::cout << "; y: ";
    y.print(std::cout);
    std::cout << "; z: ";
    z.print(std::cout);
    std::cout << std::endl;
    std::cout << "after operateHH from lvl: " << lvl << "; result: ";
    ans.print(std::cout);
    std::cout << std::endl;
#endif
    return ans;
}
// ******************************************************************
// *                                                                *
// *                       BinaryList  methods                      *
// *                                                                *
// ******************************************************************
BinaryList::BinaryList(const std::string n)
{
    reset(n);
}

BinaryOperation* BinaryList::mtfBinary(const BinaryOperationType opT, const Forest* source1F, const Forest* source2F, const Forest* resF)
{
    BinaryOperation* prev = front;
    BinaryOperation* curr = front->next;
    while (curr) {
        if ((curr->opType == opT) && (curr->source1Forest == source1F) && (curr->source2Forest == source2F) && (curr->resForest == resF)) {
            // Move to front
            prev->next = curr->next;
            curr->next = front;
            front = curr;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

BinaryOperation* BinaryList::mtfBinary(const BinaryOperationType opT, const Forest* source1F, const OpndType source2T, const Forest* resF)
{
    BinaryOperation* prev = front;
    BinaryOperation* curr = front->next;
    while (curr) {
        if ((curr->opType == opT) && (curr->source1Forest == source1F) && (curr->source2Type == source2T) && (curr->resForest == resF)) {
            // Move to front
            prev->next = curr->next;
            curr->next = front;
            front = curr;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

void BinaryList::searchRemove(BinaryOperation* bop)
{
    if (!front) return;
    BinaryOperation* prev = front;
    BinaryOperation* curr = front->next;
    while (curr) {
        if (curr == bop) {
            prev->next = curr->next;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}


// // ******************************************************************
// // *                                                                *
// // *                                                                *
// // *                 NumericalOperation  methods                    *
// // *                                                                *
// // *                                                                *
// // ******************************************************************
// NumericalOperation::NumericalOperation()
// {
//     //
// }

// // ******************************************************************
// // *                                                                *
// // *                                                                *
// // *                 SaturationOperation  methods                   *
// // *                                                                *
// // *                                                                *
// // ******************************************************************
// SaturationOperation::SaturationOperation()
// {
//     //
// }