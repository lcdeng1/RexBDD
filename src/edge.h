#ifndef REXBDD_EDGE_H
#define REXBDD_EDGE_H

#include "defines.h"
#include "edge_label.h"

#include <string>

namespace REXBDD {
    class Edge;
    class Forest;
    
    // file I/O
    // TBD
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                          Edge class                            *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Edge {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        ///
        Edge();

        /// Construct and attach to a forest.
        // Edge(Forest* p=nullptr);

        /// Copy Constructor.
        // Edge(const Edge &e);

        /// Destructor. Will notify parent as appropriate.
        ~Edge();

        /// Attach to a forest.
        void attach(Forest* p);

        /// Detach from the forest.
        inline void detach() { attach(nullptr); }

        //******************************************
        //  Getters
        //******************************************
        Forest* getParentForest() const;

        inline bool isAttachedTo(const Forest* p) const {
            return getParentForest() == p;
        }
        inline bool isSameForest(const Edge &e) const {
            return parentFID == e.parentFID;
        }
        inline int getEdgeLevel() const {
            return level;
        }
        inline nodeHandle getTarget() const {
            return targetNode;
        }
        inline const EdgeLabel& getLabel() const {
            return label;
        }
        inline ValueType getEdgeValueType() const {
            return label.getValueType();
        }
        template<typename T>
        inline void getEdgeValueTo(T &v) const {
            label.getValueTo(v);
        }
        inline ReductionRule getEdgeRule() const {
            return label.getRule();
        }
        inline bool getEdgeComp() const {
            return label.getComp();
        }
        inline bool getEdgeSwap() const {
            return label.getSwap();
        }
        //
        // More getter TBD here
        //

        //******************************************
        //  Setters
        //******************************************
        inline void setEdgeLevel(int lvl) {
            level = lvl;
        }
        inline void setTarget(nodeHandle t) {
            targetNode = t;
        }
        inline void setEdgeValueType(ValueType t) {
            label.setValueType(t);
        }
        template<typename T>
        inline void setEdgeValue(T v) {
            label.setValue(v);
        }
        inline void setEdgeRule(ReductionRule r) {
            label.setRule(r);
        }
        inline void setEdgeComp(bool c) {
            label.setComp(c);
        }
        inline void setEdgeSwap(bool s) {
            label.setSwap(s);
        }
        template<typename T>
        inline void set(nodeHandle handle, T val) {
            setTarget(handle);
            setEdgeValue(val);
        }
        inline void set(nodeHandle handle, ReductionRule rul) {
            setTarget(handle);
            setEdgeRule(rul);
        }

        //
        // More setter TBD here
        //
        /// Complement edge rule
        void compEdgeRule();
        /// Swap edge rule
        void swapEdgeRule();

        //******************************************
        //  Check for equality
        //******************************************
        bool equals(const Edge e);
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        friend class Forest;

        // used for constructor given an edge, and assignment
        void init(const Edge &e);
        
        //
        // Actual edge information
        //
        int             level;          // Incoming level, >= target node's level
        nodeHandle      targetNode;     // Target node
        EdgeLabel       label;          // Label including rule, value, flags

        Forest*         parent;         // parent forest
        unsigned        parentFID;      // ID of parent forest

        std::string     display;        // for displaying if needed in the future

        //
        // for the dd_edge registry in the parent forest
        //
        /// Previous edge in forest registry
        Edge* prevEdge;

        /// Next edge in forest registry
        Edge* nextEdge;
};


#endif