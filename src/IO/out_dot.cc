#include "out_dot.h"
#include "../forest.h"

// #define REXBDD_DOT_TRACE

using namespace REXBDD;
// ******************************************************************
// *                                                                *
// *                      DotMaker methods                          *
// *                                                                *
// ******************************************************************

DotMaker::DotMaker(const Forest* f, const std::string bn)
{
    basename = bn;
    std::string fName = basename + ".gv";
    outfile.open(fName);
    if (!outfile) {
        std::cout << "[REXBDD] Error!\t Could not open or create the file: "<< fName << std::endl;
        exit(1);
    }
    parent = f;
}
DotMaker::~DotMaker()
{
    //
}

void DotMaker::buildGraph(const Func& func)
{
#ifdef REXBDD_DOT_TRACE
    std::cout << "buildGraph: setting" << std::endl;
    std::cout << std::endl;
#endif
    /* setting info */
    uint16_t numVars = parent->getSetting().getNumVars();
    int numRules = parent->getSetting().getReductionSize();
    CompSet cs = parent->getSetting().getCompType();
    SwapSet ss = parent->getSetting().getSwapType();
#ifdef REXBDD_DOT_TRACE
    std::cout << "buildGraph: unmark" << std::endl;
    std::cout << std::endl;
#endif
    parent->unmark();
#ifdef REXBDD_DOT_TRACE
    std::cout << "buildGraph: marking" << std::endl;
    std::cout << std::endl;
#endif
    parent->markNodes(func);
#ifdef REXBDD_DOT_TRACE
    std::cout << "buildGraph: start" << std::endl;
    std::cout << std::endl;
#endif
    /* start */
    outfile << "digraph g\n{\n";
    outfile << "\trankdir=TB\n";
    outfile << "\n\tnode [shape = none]\n";
    outfile << "\tv0 [label=\"\"]\n";
    for (uint32_t i = 1; i <= numVars; i++)
    {
        outfile << "\tv" << i << " [label=\"x" << i << "\"]\n";
        outfile << "\tv" << i << " -> v" << i-1 << " [style=invis]\n";
    }
#ifdef REXBDD_DOT_TRACE
    std::cout << "buildGraph: legend" << std::endl;
    std::cout << std::endl;
#endif
    /* legend for edge label */
    std::string label = "";
    label += "<";
    if (numRules > 0) label += "X";
    if (cs != NO_COMP) label += ", c";
    if (ss == ONE || ss == ALL) label += (ss == ONE) ? ", s_o" : ", s_a";
    if (ss == FROM || ss == FROM_TO) label += ", s_f";
    if (ss == TO || ss == FROM_TO) label += ", s_t";
    label += ">";
    outfile << "\tv" << numVars+1 << " [label=\"" << label << "\"]\n";
    outfile << "\tv" << numVars+1 << " -> v" << numVars << " [style=invis]\n";
    /* build the function */
    buildEdge(numVars, func.getEdge());
    /* end */
    outfile << "}";
    outfile.close();    // close out file
}

void DotMaker::buildEdge(const uint16_t lvl, const Edge& edge, const NodeHandle rootHandle, const bool isLow)
{
#ifdef REXBDD_DOT_TRACE
    std::cout << "buildEdge: lvl = " << lvl << std::endl;
    edge.print(std::cout);
    std::cout << std::endl;
#endif
    /* setting info */
    uint16_t numVars = parent->getSetting().getNumVars();
    int numRules = parent->getSetting().getReductionSize();
    CompSet cs = parent->getSetting().getCompType();
    SwapSet ss = parent->getSetting().getSwapType();
    EncodeMechanism em = parent->getSetting().getEncodeMechanism();
    char numChild = (parent->getSetting().isRelation()) ? 4 : 2;
    /* edge lable */
    std::string label = "";
    label += "<";
    if (numRules > 0) {
        ReductionRule rule = edge.getRule();
        label += ((rule == RULE_X) && !parent->getSetting().hasReductionRule(rule)) ? "N" : rule2String(rule);
    }
    if (cs != NO_COMP) {
        label += (edge.getComp()) ? ", c" : ", _";
    }
    if (ss != NO_SWAP) {
        if (ss == ONE) {
            label += (edge.getSwap(0)) ? ", s_o" : ", _";
        } else if (ss == ALL) {
            label += (edge.getSwap(0)) ? ", s_a" : ", _";
        } else if (ss == FROM || ss == FROM_TO) {
            label += (edge.getSwap(0)) ? ", s_f" : ", _";
        } else if (ss == TO) {
            label += (edge.getSwap(1)) ? ", s_t" : ", _";
        }
        if (ss == FROM_TO) {
            label += (edge.getSwap(1)) ? ", s_t" : ", _";
        }
    }
    label += ">";
    /* root edge */
    std::string root = "N";
    std::string style = (isLow) ? "dashed" : "solid";
    if ((lvl == numVars) && (rootHandle == 0)) {
        root += std::to_string(lvl+1);
        outfile << "\t{rank=same v"<<numVars+1<<" "<<root<<" [shape = point]}\n";
    } else {
        root += std::to_string(lvl);
        root += "_";
        root += std::to_string(rootHandle);
    }
    if (em == TERMINAL) {
        if (edge.getNodeLevel() == 0) {
            EdgeHandle handle = edge.getEdgeHandle();
            outfile << "\t"<<root<<" -> \"T"<<unpackTermiValue(handle)<<"\" [style = "<<style<<" label = \""<<label<<"\"]\n";
            outfile << "\t{rank=same v0 \"T"<<unpackTermiValue(handle)<<"\" [label = \""<<unpackTermiValue(handle)<<"\", shape = square]}\n";
        } else {
            outfile << "\t"<<root<<" -> \"N"<<edge.getNodeLevel()<<"_"<<edge.getNodeHandle()<<"\" [style = "<<style<<" label = \""<<label<<"\"]\n";
            outfile << "\t{rank=same v"<<edge.getNodeLevel()<<" N"<<edge.getNodeLevel()<<"_"<<edge.getNodeHandle()
            <<" [label = \"N"<<edge.getNodeLevel()<<"_"<<edge.getNodeHandle()<<"\", shape = circle]}\n";
            // build child edges of target node if it's marked
            if (parent->getNode(edge.getNodeLevel(), edge.getNodeHandle()).isMarked()) {
                for (char i=0; i<numChild; i++) {
                    buildEdge(edge.getNodeLevel(),
                                parent->getChildEdge(edge.getNodeLevel(), edge.getNodeHandle(), i),
                                edge.getNodeHandle(),
                                (i==0 || i==2));
                }
            }
            // unmark
            parent->getNode(edge.getNodeLevel(), edge.getNodeHandle()).unmark();
        }
    } else {
        // for edge valued TBD
    }
}

void DotMaker::runDot(const std::string ext)
{
    if (ext == "" || ext == "dot") return;
    /* build the command string */
    std::string cmd = "dot -T";
    cmd += ext;
    cmd += " ";
    cmd += basename;
    cmd += ".gv";
    cmd += " > ";
    cmd += basename;
    cmd += ".";
    cmd += ext;
    std::cout << "run dot: " << cmd << std::endl;
    /* run the command */
#ifdef _WIN32
    int result = system(("cmd /C " + cmd).c_str());
#else
    int result = system(("sh -c \"" + cmd + "\"").c_str());
#endif
    if (result) {
        std::cout << "[REXBDD] Error!\t Failed to run dot and build file: "<< basename << "." << ext << std::endl;
        throw error(MISCELLANEOUS, __FILE__, __LINE__);
    }
    std::string testcmd = "dot --version";
    if (system(testcmd.c_str())) {
        std::cout << "[REXBDD] Error!\t Failed to run dot and build file: "<< basename << "." << ext << std::endl;
        exit(1);
    }
}