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

        inline nodeHandle getTarget() const {
            return targetNode;
        }
        inline const EdgeLabel& getLabel() const {
            return label;
        }
        inline ValueType getEdgeValueType() const {
            return label.getValueType();
        }
        inline FlagType getEdgeFlagType() const {
            return label.getFlagType();
        }
        template<typename T>
        inline void getEdgeValueTo(T &v) const {
            label.getValueTo(v);
        }

        //******************************************
        //  Setters
        //******************************************
        void setTarget(nodeHandle t);
        template<typename T>
        inline void setEdgeValue(T v) {
            label.setValue(v);
        }
        template<typename T>
        inline void set(nodeHandle handle, T value) {
            setTarget(handle);
            setEdgeValue(value);
        }

        //******************************************
        //  Check for equality
        //******************************************
        inline bool equals(const Edge e) {
            return
                (parentFID == e.parentFID);
        }
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        friend class Forest;

        void init(const Edge &e);
        
        //
        // Actual edge information
        //

        /// ID of parent forest
        unsigned parentFID;

        /// Target node
        nodeHandle targetNode;

        /// Label on this edge
        EdgeLabel label;

        /// for displaying if needed in the future
        std::string display;

        //
        // for the dd_edge registry in the parent forest
        //

        /// Previous edge in forest registry
        Edge* prevEdge;

        /// Next edge in forest registry
        Edge* nextEdge;
};


#endif