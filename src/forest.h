#ifndef REXBDD_FOREST_H
#define REXBDD_FOREST_H

#include "defines.h"
#include "setting.h"
#include "edge.h"
#include "terminal.h"
#include "node.h"
#include "function.h"
#include "node_manager.h"
#include "unique_table.h"
#include "statistics.h"

namespace REXBDD {
    class Forest;
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                          forest class                          *
// *                                                                *
// *                                                                *
// ******************************************************************
/**
 * @brief Forest class.
 * 
 * A data structure for managing collections of functions (for sets, 
 * or relations, depending on your conceptual view) represented in a 
 * single decision diagram forest over a common domain.
 *
 */
class REXBDD::Forest {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    /**
     * @brief Construct a new Forest object from setting.
     * 
     * @param setting       The specification settings
     */
    Forest(const ForestSetting &setting);
    /**
     * @brief Destroy the Forest object
     */
    ~Forest();

    /// Methods =====================================================
    /**
     * @brief Get the stored node from the NodeManager by giving its level and node handle.
     * 
     * @param level         The level of the node.
     * @param handle        The handle of the node.
     * @return Node         – Output the node stored in NodeManager.
     */
    inline Node& getNode(const uint16_t level, const NodeHandle& handle) const {
        return nodeMan->getNodeFromHandle(level, handle);
    }

    /**
     * @brief Get the stored target node from the NodeManager by giving its incoming edge handle.
     * 
     * @param edge          The given incoming edge handle
     * @return Node         – Output the targer node stored in NodeManager.
     */
    inline Node& getNode(const EdgeHandle& edge) const {
        return getNode(unpackLevel(edge), unpackTarget(edge));
    }
    
    /**
     * @brief Get the stored target Node from the NodeManager by giving its incoming edge.
     * 
     * @param edge          The given incoming edge.
     * @return Node         – Output the targer node stored in NodeManager.
     */
    inline Node& getNode(const Edge& edge) const {
        return getNode(edge.getNodeLevel(), edge.getNodeHandle());
    }

    /**
     * @brief Get the stored node's next pointer (NodeHandle of another unique node at the same level having 
     * the same hash value) by giving its level and node handle.
     * 
     * @param level         The given node level.
     * @param handle        The given node handle.
     * @return NodeHandle   - Output the next pointer.
     */
    inline NodeHandle getNodeNext(const uint16_t level, const NodeHandle handle) const {
        return nodeMan->getNodeFromHandle(level, handle).getNext();
    }

    /**
     * @brief Set the stored nodes's next pointer.
     * 
     * @param level         The givien node level.
     * @param handle        The givien node handle.
     * @param next          The next handle to be set.
     */
    inline void setNodeNext(const uint16_t level, const NodeHandle handle, const NodeHandle next) {
        nodeMan->getNodeFromHandle(level, handle).setNext(next);
    }

    /**
     * @brief Compute and get the hash value of a node stored in NodeManager, by giving the node's level and node handle.
     * 
     * @param level         The given node level.
     * @param handle        The given node handle.
     * @return uint64_t     - Output the node's hash value.
     */
    inline uint64_t getNodeHash(const uint16_t level, const NodeHandle handle) const {
        return nodeMan->getNodeFromHandle(level, handle).hash(nodeSize);
    }

    /**
     * @brief Store a node in NodeManager and return its node handle, by giving the node's level and itself.
     * Note: This does not guarantee that the node is uniquely stored.
     * 
     * @param level         The level of the node to be stored.
     * @param node          The node.
     * @return NodeHandle   - Output the stored node's handle.
     */
    inline NodeHandle obtainFreeNodeHandle(const uint16_t level, const Node& node) {
        return nodeMan->getFreeNodeHandle(level, node);
    }
    
    /**
     * @brief Uniquely store a node and get its node handle by giving its level and itself.
     * 
     * @param level         The level of the node.
     * @param node          The node to insert.
     * @return NodeHandle   - Output the unique node handle stored in UniqueTable.
     */
    inline NodeHandle insertNode(const uint16_t level, const Node& node) {
        return uniqueTable->insert(level, node);
    }

    /**
     * @brief Get the level of a child node, by giving its parent node's level, node handle, and child index.
     * Note: if the child index is out of the valid range, i.e., 3 for a "set" bdd node, it will throw an error
     *       and exit with the error code.
     * 
     * @param level         The level of the parent node.
     * @param handle        The parent node handle.
     * @param child         The child index.
     * @return uint16_t     - Output the level
     */
    inline uint16_t getChildLevel(const uint16_t level, const NodeHandle handle, const char child) const {
        // check if node in this forest skips level
        if (setting.getReductionSize()==0) return level-1;
        // node storage must have child level information
        return getNode(level, handle).childNodeLevel(child, setting.isRelation());
    }

    /**
     * @brief Get the node handle for a child node, by giving its parent node's level, node handle, and child index.
     * 
     * @param level         The level of the parent node.
     * @param handle        The parent node handle.
     * @param child         The child index.
     * @return NodeHandle   - Output the node handle.
     */
    inline NodeHandle getChildNodeHandle(const uint16_t level, const NodeHandle handle, const char child) const {
        return getNode(level, handle).childNodeHandle(child, setting.isRelation());
    }

    /**
     * @brief Get the edge handle for a child edge, by giving its parent node's level, node handle, and child index.
     * 
     * @param level         The level of the parent node.
     * @param handle        The parent node handle.
     * @param child         The child index.
     * @return EdgeHandle   - Output the edge handle.
     */
    inline EdgeHandle getChildEdgeHandle(const uint16_t level, const NodeHandle handle, const char child) const {
        // the answer
        EdgeHandle ans = 0;
        // find the node
        Node& node = getNode(level, handle);
        bool isRel = setting.isRelation();
        // fill edge rule
        packRule(ans, node.edgeRule(child,isRel));
        // fill complement
        packComp(ans, node.edgeComp(child, isRel));
        // fill swap
        packSwap(ans, node.edgeSwap(child, 0, isRel));
        packSwapTo(ans, node.edgeSwap(child, 1, isRel));
        // fill level
        uint16_t childLvl = getChildLevel(level, handle, child);
        packLevel(ans, childLvl);
        // fill target
        if (childLvl == 0) {
            // terminal, check the terminal type
            uint32_t data = node.childNodeHandle(child,isRel);
            packTarget(ans, data);
            if (node.isChildTerminalSpecial(child)) {
                // assuming this forest allows the special value
                // special terminal value, then update the "header"
                ans |= SPECIAL_VALUE_FLAG_MASK;
            } else {
                // terminal value should be INT or FLOAT
                ValueType valType = setting.getValType();
                if (valType == INT || valType == LONG) {
                    ans |= INT_VALUE_FLAG_MASK;
                } else {
                    ans |= FLOAT_VALUE_FLAG_MASK;
                }
            }
        } else {
            packTarget(ans, node.childNodeHandle(child, isRel));
        }
        return ans;
    }

    inline Edge getChildEdge(const uint16_t level, const NodeHandle handle, const char child) const {
        Edge ans;
        ans.handle = getChildEdgeHandle(level, handle, child);
        if ((setting.getEncodeMechanism() != TERMINAL) && (unpackLevel(ans.handle) != 0)) {
            // get the valid value, TBD
            // Node& node = getNode(level, handle);
        }

        return ans;
    }

    /**
     * @brief Get the terminal value for a child node, by giving its parent node's level, node handle, and child index.
     * Note: if the child node at level above 0, which means it's not a terminal node, it will throw error and exit.
     * 
     * @param level         The level of the parent node.
     * @param handle        The parent node handle.
     * @param child         The child index.
     * @return Value        - Output the terminal value.
     */
    inline Value getChildTerminalValue(const uint16_t level, const NodeHandle handle, const char child) const {
        if (getChildLevel(level, handle, child)>0) {
            std::cout << "[REXBDD] ERROR!\t No value for nonterminal node!"<< std::endl;
            exit(0);
        }
        /* Node is already stored, assuming its terminal value is allowed */
        bool isRel = setting.isRelation();
        Value val(0);
        Node& node = getNode(level, handle);
        NodeHandle data = node.childNodeHandle(child,isRel);
        if (node.isChildTerminalSpecial(child)) {
            // special value
            SpecialValue value = *reinterpret_cast<SpecialValue*>(&data);
            val.setValue(&value, VOID);
        } else {
            // the forest value type
            ValueType valType = setting.getValType();
            if (valType == INT) {
                int value = *reinterpret_cast<int*>(&data);
                val.setValue(&value, INT);
            } else if (valType == FLOAT) {
                float value = *reinterpret_cast<float*>(&data);
                val.setValue(&value, FLOAT);
            } else if (valType == LONG) {
                long value = *reinterpret_cast<long*>(&data);
                val.setValue(&value, LONG);
            } else if (valType == DOUBLE) {
                double value = *reinterpret_cast<double*>(&data);
                val.setValue(&value, DOUBLE);
            }
        }
        return val;
    }

    /**
     * @brief Get the child edge label (including information of rules and flags) of a node, by giving the node's 
     * incoming edge handle and the index of child edge.
     * 
     * @param level         The level of the parent node.
     * @param handle        The parent node handle.
     * @param child         The child index.
     * @return EdgeLabel    - Output the target node's child edge label.
     */
    inline EdgeLabel getChildLabel(const uint16_t level, const NodeHandle handle, const char child) const {
        bool isRel = setting.isRelation();
        return getNode(level, handle).edgeLabel(child, isRel);
    }

    /**
     * @brief Get the child edge of a node, by giving the node's incoming edge and the index of child edge.
     * 
     * @param edge          The incoming edge.
     * @param index         The index of target node's child.
     * @return Edge         - Output the target node's child edge.
     */
    inline Edge getChildEdge(const Edge& edge, const int index) const {
        // TBD
        Edge res(edge);
        return res;
    }

    inline Edge cofact(const uint16_t lvl, const Edge& edge, const char index) {
        if (lvl == 0) return edge;
        if (lvl == edge.getNodeLevel()) {
            char childIndex = index;
            if (edge.getSwap(0)) {
                // only for BDDs
                childIndex = 1 - childIndex;
            }
            Edge ans = getChildEdge(lvl, edge.getNodeHandle(), childIndex);
            if (edge.getComp()) ans.complement();
            if ((setting.getSwapType() == ALL) && edge.getSwap(0)) ans.swap();
            return ans;
        }
        Edge ans;
        ReductionRule rule = edge.getRule();
        if ((isRuleEL(rule) && (index == 0)) || (isRuleEH(rule) && (index == 1))
            || (isRuleAL(rule) && (lvl - edge.getNodeLevel() == 1) && (index == 0))
            || (isRuleAH(rule) && (lvl - edge.getNodeLevel() == 1) && (index == 1))
            || (isRuleI(rule) && ((index == 1) || (index == 2)))) {
            EdgeHandle childH = makeTerminal(INT, (int)hasRuleTerminalOne(rule));
            if (setting.getValType() == FLOAT) {
                childH = makeTerminal(FLOAT, (float)hasRuleTerminalOne(rule));
            }
            packRule(childH, RULE_X);
            ans.setEdgeHandle(childH);
            ans = normalizeEdge(lvl-1, ans);
            return ans;
        }
        ans = edge;
        if ((lvl - edge.getNodeLevel()) == 1) {
            ans.setRule(RULE_X);
            return ans;
        }
        ans = normalizeEdge(lvl-1, ans);
        return ans;
    }

    /************************* Reduction ****************************/
    /**
     * @brief Reduce and return an edge pointing to a unique reduced node stored in NodeManager, by giving the beginning level 
     * of an unreduced incoming edge, its unreduced edge label, and unreduced target node (its level, child edges).
     * 
     * @param beginLevel    The beginning level of the unreduced incoming edge.
     * @param label         The unreduced incoming edge label.
     * @param nodeLevel     The level of the unreduced target node.
     * @param down          The vector of child edges of the unreduced target node.
     * @param value         [Optional] The value attached on the incoming edge.
     * @return Edge         - Output: the reduced edge pointing to a reduced node uniquely stored.
     */
    Edge reduceEdge(const uint16_t beginLevel, const EdgeLabel label, const uint16_t nodeLevel, const std::vector<Edge>& down, const Value& value = Value());




    /************************* Within Operations ********************/
    /**
     * @brief Unmark all nodes in the forest. This is usually used to initialize 
     * for counting the marked nodes or sweeping the unmarked nodes.
     * 
     */
    inline void unmark() const {nodeMan->unmark();}

    /**
     * @brief Mark all the nonterminal nodes reachable from the given Func edge
     * in the forest.
     * 
     * @param Func          The Func edge.
     */
    inline void markNodes(const Func& func) const {markNodes(func.edge);}

    /**
     * @brief Mark all the nonterminal nodes reachable from all Func edge in 
     * the forest.
     * 
     */
    inline void markAllFuncs() const {
        // TBD
    }

    /***************************** Cardinality **********************/
    uint64_t countNodes();   // all Funcs
    uint64_t countNodes(FuncArray funcs);
    uint64_t countNodesAtLevel(uint16_t lvl);
    uint64_t countNodesAtLevel(uint16_t lvl, Func func);
    uint64_t countNodesAtLevel(uint16_t lvl, FuncArray funcs);

    uint64_t mass(Func func);

    uint64_t count(Func func, int val); // ...
    // mpz_t count(); // gmp here?
    uint64_t count(Func func, long min, long max); // min and max are both included
    uint64_t count(Func func, double min, double max); // min and max are both included
    uint64_t count(Func func, SpecialValue val);
    unsigned minValue(Func func); // return type
    unsigned maxValue(Func func); // return type
    // TBD

    /************************* Garbage Collection *******************/
    void deleteNode(NodeHandle handle);
    inline void sweepNodeMan(uint16_t level) {nodeMan->sweep(level);}
    void markSweep();
    // TBD

    /************************* Statistics Information ***************/
    inline uint32_t getNodeManUsed(const uint16_t level) const {
        return nodeMan->numUsed(level);
    }
    inline uint32_t getNodeManAlloc(const uint16_t level) const {
        return nodeMan->numAlloc(level);
    }
    inline uint32_t getUTEntriesNum(const uint16_t level) const {
        return uniqueTable->getNumEntries(level);
    }
    uint64_t getPeakNodes();    // largest result of getCurrentNodes(), since the last call to resetPeakNodes()
    uint64_t getCurrentNodes(); // number of nodes in UT, including disconnected
    void resetPeakNodes();
    // TBD

    /****************************** I/O *****************************/
    // for transform files
    void exportFunc(std::ostream& out, FuncArray func);
    void exportForest(std::ostream& out);
    FuncArray importFunc(std::istream& in);
    void importForest(std::istream& in);
    
    // TBD


    /************************* Setting Information ******************/
    /**
     * @brief Get the ForestSetting used by this forest.
     */
    inline const ForestSetting& getSetting() const {return setting;}
    inline void exportSetting(std::ostream out, int format) const {setting.output(out, format);}

    /*************************** Reordering *************************/
    void shiftUp(unsigned lvl);
    void shiftDown(unsigned lvl);
    void reorder(unsigned* level2Var);

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Helper Methods ==============================================

    /** Check the comopatibility of specifications, find and report conflicts.
     *  This is usually used before constructing Forest with this setting.
     *  Return 1: pass; 0: failed
     */
    bool checkCompatibility() const;

    /**
     * @brief Normalize a node to ensure canonicity.
     * 
     * @param nodeLevel     The given node level.
     * @param down          The vector of child edges of the node.
     * @param out           Output: edge label (rule/value, flags).
     */
    Edge normalizeNode(const uint16_t nodeLevel, const std::vector<Edge>& down);

    /**
     * @brief Normalize a long edge to be legal for this forest setting. If the reduction rule
     * is not allowed, the skipped nodes along this edge will be built and reduced.
     * 
     * @param level         The start level of the given long edge.
     * @param edge          The given long edge to be normalized.
     * @return Edge         Output: normalized edge.
     */
    Edge normalizeEdge(const uint16_t level, const Edge& edge);
    
    /**
     * @brief Reduce a node assuming the incoming edge is a short edge with 0 edge value.
     * 
     * @param nodeLevel     The level of the unreduced node.
     * @param down          The vector of child edges of the unreduced node.
     * @return Edge         - Output: reduced edge.
     */
    Edge reduceNode(const uint16_t nodeLevel, const std::vector<Edge>& down);

    /**
     * @brief Merge the incoming edge having EdgeLabel "label", which is respect of 
     * level "lvl1", target to the node at level "lvl2"; edge "*reduced" represents 
     * the node at level "lvl2" after calling reduceNode method. Edge "*out" can not 
     * be null, it will be written the merged edge.
     * 
     * @param beginLevel    The represent level of the incoming edge.
     * @param mergeLevel    The target node level of the incoming edge.
     * @param label         The incoming edge label.
     * @param reduced       The reduced edge.
     * @param value         [Optional] The value attached on the incoming edge.
     * @return Edge         - Output: merged edge (label, target node handle).
     */
    Edge mergeEdge(const uint16_t beginLevel, const uint16_t mergeLevel, const EdgeLabel label, const Edge& reduced, const Value& value = Value());

    /**
     * @brief Check if the swap-all bit is useless, by giving the parent forest and edge
     * 
     * @param F         the parent forest.
     * @param e         the given edge.
     * @return char     0: necessary; 1: can be directly deleted; 2: the same as complement bit.
     */
    char isSwapAllUseless(Edge& e);

    Edge unreduceEdge(const uint16_t level, const Edge& edge);

    // these are only used by BDDs operations

    Edge buildHalf(const uint16_t beginLvl, const uint16_t endLvl, const Edge& e1, const Edge& e2, const bool isLow);
    Edge buildUmb(const uint16_t beginLvl, const uint16_t endLvl, const Edge& e1, const Edge& e2, const Edge& e3);

    /* Marker */
    void markNodes(const Edge& edge) const;

    /// =============================================================
    friend class NodeManager;
    friend class UniqueTable;
    friend class Func;
    friend class UnaryOperation;
    friend class BinaryOperation;
        ForestSetting       setting;        // Specification setting of this forest.
        NodeManager*        nodeMan;        // Node manager.
        UniqueTable*        uniqueTable;    // Unique table.
        Func*               funcs;          // Registry of Func edges.
        FuncArray*          funcSets;       // Sets of Func used for I/O.
        Statistics*         stats;          // Performance measurement.
        int                 nodeSize;       // Number of uint32 slots for one Node storage.
};


#endif