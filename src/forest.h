#ifndef REXBDD_FOREST_H
#define REXBDD_FOREST_H

#include "defines.h"
#include "setting.h"
#include "node.h"
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

class REXBDD::Forest {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Forest();
    Forest(ForestSetting *setting);
    ~Forest();

    
    
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        ForestSetting       settings;
        NodeManager         nodeMan;
        UniqueTable         uniqueTable;
        /// Regustry of Edges
        Edge* rootEdges;


        Forest*             allForests;
};


#endif