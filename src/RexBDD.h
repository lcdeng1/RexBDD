/*
    RexBDD: Reduction-on-Edge Complement-and-Swap Binary Decision Diagram.
    Copyright (C) 2024, Iowa State University.
*/
#ifndef REXBDD_H
#define REXBDD_H

#include "info.h"
#include "defines.h"
#include "setting.h"
#include "node.h"
#include "edge.h"
#include "function.h"
#include "node_manager.h"
#include "unique_table.h"
#include "forest.h"
#include "operators.h"
#include "operations/apply.h"
#include "IO/out_dot.h"

#include <iostream>

inline void myPrint()
{
    std::cout << "Library is under development...^_^\n";
}

inline void printInfo()
{
    std::cout << "****************************[" << PROJECT_NAME << "]****************************" << std::endl;
    std::cout << "Version: \t" << PROJECT_VERSION << std::endl;
    std::cout << "Homepage: \t" << PROJECT_URL << std::endl;
    std::cout << "Copyright: \t" << PROJECT_COPYRIGHT << std::endl;
    std::cout << "License: \t" << PROJECT_LICENSE << std::endl;
    std::cout << "Last Update: \t" << PROJECT_DATE << std::endl;
    std::cout << "*****************************************************************" << std::endl;
    std::cout << "Under development...^_^" << std::endl;
}

#endif