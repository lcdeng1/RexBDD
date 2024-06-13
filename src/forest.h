#ifndef REXBDD_FOREST_H
#define REXBDD_FOREST_H

#include "defines.h"
#include "setting.h"
#include "node.h"
#include "packed_node.h"
#include "node_manager.h"
#include "unique_table.h"

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
 * A data structure for managing collections of functions (or sets, 
 * or relations, depending on your conceptual view) represented in a 
 * single decision diagram forest over a common domain.
 *
 */
class REXBDD::Forest {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Forest();
    /**
     * @brief Construct a new Forest object from setting.
     * 
     * @param setting       The specification settings
     */
    Forest(ForestSetting *setting);
    /**
     * @brief Destroy the Forest object
     */
    ~Forest();

    /// Methods =====================================================
    /* Get Setting Information */
    /**
     * @brief Get the ForestSetting used by this forest.
     */
    inline const ForestSetting& getSetting() const {return setting;}
    /**
     * @brief Check if this forest is representing relation.
     */
    inline bool isRelation() const {return setting.getDim() == 2;}
    /**
     * @brief Get the number of levels in forest. 
     */
    inline unsigned getNumLevel() const {return setting.getNumVars();}
    /**
     * @brief Get the level index for a given variable index.
     * 
     * @param var               The variable index.
     */
    inline unsigned getLevelIndex(unsigned var) const {return setting.getLevel(var);}
    /**
     * @brief Get the variable index for a given level index.
     * 
     * @param lvl               The level index.
     */
    inline unsigned getVarIndex(unsigned lvl) const {return setting.getVar(lvl);}
    /**
     * @brief Get the range type in the forest.
     */
    inline RangeType getRangeType() const {return setting.getRangeType();}
    /**
     * @brief Get the value type in the forest.
     */
    inline ValueType getValType() const {return setting.getValType();}
    /**
     * @brief Get the max range that has been set for the forest.
     */
    inline unsigned getMaxRange() const {return setting.getMaxRange();}
    /**
     * @brief Get the min range that has been set for the forest.
     */
    inline unsigned getMinRange() const {return setting.getMinRange();}
    /**
     * @brief Check if the forest has set the negative infinity in range.
     */
    inline bool hasNegInf() const {return setting.hasNegInf();}
    /**
     * @brief Check if the forest has set the positive infinity in range.
     */
    inline bool hasPosInf() const {return setting.hasPosInf();}
    /**
     * @brief Check if the forest has set the undefine value in range.
     */
    inline bool hasUnDef() const {return setting.hasUnDef();}
    /**
     * @brief Get the encode mechanism for the forest.
     */
    inline EncodeMechanism getEncodeMechanism() const {return setting.getEncodeMechanism();}
    /**
     * @brief Get the reduction type for the forest.
     */
    inline ReductionType getReductionType() const {return setting.getReductionType();}
    /**
     * @brief Get the size of the applied reduction rules set for the forest.
     */
    inline int getReductionSize() const {return setting.getReductionSize();}
    /**
     * @brief Check if the forest has applied the given ReductionRule.
     */
    inline bool hasReductionRule(ReductionRule rule) const {return setting.hasReductionRule(rule);}
    /**
     * @brief Get the swap flag type that has been set for the forest.
     */
    inline SwapSet getSwapType() const {return setting.getSwapType();}
    /**
     * @brief Get the complement flag type that has been set for the forest.
     */
    inline CompSet getCompType() const {return setting.getCompType();}
    /**
     * @brief Check if the complement flag has been set for the forest.
     */
    inline bool isCompSet() const {return getCompType() == COMP;}
    /**
     * @brief Get the merge type that has been set for the forest.
     */
    inline MergeType getMergeType() const {return setting.getMergeType();};

    /* Manipulating Setting */
    /* [Warning]: 
            Once this forest is constructed with a given setting and after some BDDs are built,
            the setting should not be changed at will. Otherwise, it may cause unpredictable results.
    */

    /* Manipulating Edges */




    uint64_t getNumNodes();





    /* Manipulating Terminal Node Handle */
    /**
     * @brief Check if the given NodeHandle is terminal.
     * Note: terminal nodes are not stored, they are represented 
     *       by a special NodeHandle value.
     * 
     * @param handle        The given NodeHandle.
     * @return true         – If it's terminal.
     * @return false        – If not.
     */
    bool isTerminal(NodeHandle handle);
    /**
     * @brief Get the terminal value for the given NodeHandle.
     * Note: it would check if the given handle is terminal, 
     *       and return -1 if not.
     * 
     * @param handle        The given NodeHandle.
     * @return uint64_t     – Output termianl value.
     */
    uint64_t getTerminalValue(NodeHandle handle);
    /**
     * @brief Make the special NodeHandle for terminal Ω. 
     * This is usually used for edge valued BDDs.
     * 
     * @return NodeHandle   – Output terminal node handle.
     */
    NodeHandle makeTerminal();
    /**
     * @brief Make the special NodeHandle for termianl with 
     * a given value.
     * 
     * @param val           The given terminal value.
     * @return NodeHandle   – Output terminal node handle.
     */
    NodeHandle makeTerminal(int val);
    /**
     * @brief Make the special NodeHandle for termianl with 
     * a given value.
     * 
     * @param val           The given terminal value.
     * @return NodeHandle   – Output terminal node handle.
     */
    NodeHandle makeTerminal(long val);
    /**
     * @brief Make the special NodeHandle for termianl with 
     * a given value.
     * 
     * @param val           The given terminal value.
     * @return NodeHandle   – Output terminal node handle.
     */
    NodeHandle makeTerminal(float val);
    /**
     * @brief Make the special NodeHandle for termianl with 
     * a given value.
     * 
     * @param val           The given terminal value.
     * @return NodeHandle   – Output terminal node handle.
     */
    NodeHandle makeTerminal(double val);

    /* Node Information */

    int getNodeLevel(NodeHandle handle);

    NodeHandle getChildHandle(NodeHandle handle, int index);

    void getChildLabel(NodeHandle handle, int index, EdgeLabel &label);

    PackedNode* getPackedNode(NodeHandle handle);

    NodeHandle getNodeNext(NodeHandle handle);
    void setNodeNext(NodeHandle handle, NodeHandle next);
    bool areDuplicates(NodeHandle handle, Node node);






    /* Reduction */
    Edge reduceNode(Node* P);
    
    Edge reduceEdge(Node* P);

    
    
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Helper Methods ==============================================
        


    /// =============================================================
        ForestSetting       setting;        // Specification setting of this forest
        NodeManager         nodeMan;        // Node manager
        UniqueTable         uniqueTable;    // Unique table
        Edge*               rootEdges;      // Registry of Root edges


        Forest*             allForests;     // Registry of forests TBD
};


#endif