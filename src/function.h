#ifndef REXBDD_FUNCTION_H
#define REXBDD_FUNCTION_H

#include "defines.h"
#include "setting.h"
#include "edge.h"

namespace REXBDD {
    class Func;
    class FuncArray;
    class ExplictFunc;
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                          Func class                            *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Func {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Func();
    Func(Forest* f);
    Func(Forest* f, const Edge& e);
    ~Func();

    /***************************** General **************************/
    inline Forest* getForest() const {return parent;}
    inline Edge getEdge() const {return edge;}
    inline bool isAttachedTo(const Forest* p) const {return getForest() == p;}
    inline bool isSameForest(const Func &e) const {return parent == e.getForest();}

    inline std::string getName() const {return name;}
    inline void setEdge(Edge& e) {edge = e;}
    inline void setName(std::string l) {name = l;}

    /**************************** Make Func *************************/
    // Constant false Func
    void falseFunc();
    // Constant true Func
    void trueFunc();
    // Constant Func
    /* For dimention 1 and 2 */
    void constant(Value val);
    /* For dimention of 2 (Relation) */
    void identity(std::vector<bool> dependance);
    void identity(std::list<int> identities);   // levels staying identity
    // Variable Func
    /* For dimention of 1 (Set) */
    void variable(uint16_t lvl);
    void variable(uint16_t lvl, Value low, Value high);
    /* For dimention of 2 (Relation) */
    void variable(uint16_t lvl, bool isPrime);
    void variable(uint16_t lvl, bool isPrime, Value low, Value high);

    // Assignment operator
    // Func operator=(Func e);


    /************************* Within Operations ********************/

    /** Compute and get the encoded function value by giving the assignment.
     *  Note: the first element of "assignment" (assignment[0]) is not used!
     * 
     */
    Value evaluate(const std::vector<bool>& assignment) const;
    Value evaluate(const std::vector<bool>& aFrom, const std::vector<bool>& aTo) const;

    // ExplictFunc including info of assignments, outcomes, 
    void unionAssignments(const ExplictFunc& assignments);

    uint64_t countNodes(Func func);



    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    // ======================Helper Methods====================
    /// Attach to a forest and link to its registry.
    void attach(Forest* p);
    void init(Func& f);
    inline bool equals(const Func f) const {
        //
        return
            (parent == f.parent) &&
            (edge.handle == f.edge.handle) &&
            (edge.value.getType());
    }
    Edge unionAssignmentRecursive(uint16_t num, Edge& root, ExplictFunc assignments);


    // ========================================================
    friend class Forest;
    Forest*     parent;     // parent forest
    Edge        edge;       // edge information
    std::string name;       // Optional, only used for I/O; defaults to empty string
    /* For the Funcs registry in the parent forest */
    Func*       prevFunc;   // Previous Func edge in parent forest
    Func*       nextFunc;   // Next Func edge in parent forest
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                        FuncArray class                         *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::FuncArray {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    FuncArray();
    FuncArray(Forest* f, int size);
    ~FuncArray();

    inline Forest* getForest() const {return parent;};
    inline bool isAttachedTo(const Forest* p) const {return getForest() == p;}
    inline bool isSameForestSet(const FuncArray &e) const {return parent == e.getForest();}
    
    /// Attach to a forest.
    void attach(Forest* p);
    /// Detach from the forest.
    inline void detach() {attach(nullptr);}
    void add(Func f);


    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    friend class Forest;
    Forest*     parent;     // parent forest
    Func*       set;        // set of the Funcs
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                      ExplictFunc class                         *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::ExplictFunc {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    std::vector<std::vector<bool> >     assignments;
    std::vector<Value>                  outcomes;
    Value                               defaultVal;
};

#endif