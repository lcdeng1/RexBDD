// #include "edge.h"
#include "node.h"

#include <iostream>

using namespace REXBDD;

int main() {
    // Edge e;
    // std::cout << "Value type: " << e.getEdgeValueType() << std::endl;
    // std::cout << "Value int: " << e.getLabel().getIntValue() << std::endl;
    // std::cout << "Value long: " << e.getLabel().getLongValue() << std::endl;
    // std::cout << "Value float: " << e.getLabel().getFloatValue() << std::endl;
    // std::cout << "Value double: " << e.getLabel().getDoubleValue() << std::endl;
    // std::cout << "Value comp: " << e.getLabel().getComp() << std::endl;
    // std::cout << "Value swap: " << e.getLabel().getSwap() << std::endl;
    // std::cout << "Value rule: " << e.getLabel().getRule() << std::endl;
    // std::cout << "-------------------------------------------------" << std::endl;
    
    // float val = 15.134;
    // e.setEdgeValue(val);

    // std::cout << "Value type: " << e.getEdgeValueType() << std::endl;
    // std::cout << "Value int: " << e.getLabel().getIntValue() << std::endl;
    // std::cout << "Value long: " << e.getLabel().getLongValue() << std::endl;
    // std::cout << "Value float: " << e.getLabel().getFloatValue() << std::endl;
    // std::cout << "Value double: " << e.getLabel().getDoubleValue() << std::endl;
    // std::cout << "Value comp: " << e.getLabel().getComp() << std::endl;
    // std::cout << "Value swap: " << e.getLabel().getSwap() << std::endl;
    // std::cout << "Value rule: " << e.getLabel().getRule() << std::endl;

    // std::cout << "-------------------------------------------------" << std::endl;
    
    // ReductionRule rule = EH0;
    // e.setEdgeValue(rule);
    // e.set(111, 16);

    // std::cout << "Value type: " << e.getEdgeValueType() << std::endl;
    // std::cout << "Value int: " << e.getLabel().getIntValue() << std::endl;
    // std::cout << "Value long: " << e.getLabel().getLongValue() << std::endl;
    // std::cout << "Value float: " << e.getLabel().getFloatValue() << std::endl;
    // std::cout << "Value double: " << e.getLabel().getDoubleValue() << std::endl;
    // std::cout << "Value comp: " << e.getLabel().getComp() << std::endl;
    // std::cout << "Value swap: " << e.getLabel().getSwap() << std::endl;
    // std::cout << "Value rule: " << e.getLabel().getRule() << std::endl;


    // std::cout << "size of edge label: " << sizeof(EdgeLabel) << std::endl;
    // std::cout << "size of edge: " << sizeof(Edge) << std::endl;
    // std::cout << "size of node: " << sizeof(Node) << std::endl;

    // EdgeLabel l;
    // try
    // {
    //     l.setRule((ReductionRule)23);
    // }
    // catch(const REXBDD::error& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }
    
    // std::cout << "rule of label l: " << l.getRule() << std::endl;

    std::cout << "size of handle: " << sizeof(NodeHandle) << std::endl;
    std::cout << "size of int: " << sizeof(int) << std::endl;
    


    return 0;
}