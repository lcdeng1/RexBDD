#ifndef REXBDD_OPERATION_H
#define REXBDD_OPERATION_H

#include "../defines.h"
#include "../forest.h"
#include "compute_table.h"

namespace REXBDD {
    class Operation;
    /// Argument and result types for apply operations.
    enum class OpndType {
        FOREST          = 0,
        BOOLEAN         = 1,
        INTEGER         = 2,
        REAL            = 3,
        HUGEINT         = 4,
        FLOATVECT       = 5,
        DOUBLEVECT      = 6,
        EXPLICIT_FUNC   = 7
    };
    /// Built-in Unary operation type
    enum class UnaryOperationType{
        UOP_COPY,
        UOP_CARDINALITY,
        UOP_MASS,
        UOP_HIGHEST,
        UOP_LOWEST,
        UOP_COMPLEMENT,
        UOP_CONCRETIZE_RST,
        UOP_CONCRETIZE_OSM,
        UOP_CONCRETIZE_TSM,
        UOP_EQUANTIFY,
        UOP_UQUANTIFY,
        UOP_REORDER
    };
    class UnaryOperation;
    class UnaryList;
    /// Built-in Binary operation type
    enum class BinaryOperationType{
        BOP_UNION,
        BOP_INTERSECTION,
        BOP_DIFFERENCE,
        BOP_MINIMUM,
        BOP_MAXIMUM,
        BOP_PLUS,
        BOP_MINUS,
        BOP_MULTIPLY,
        BOP_DIVIDE,
        BOP_EQUAL,
        BOP_NOTEQUAL,
        BOP_LESSTHAN,
        BOP_LESSTHANEQ,
        BOP_GREATERTHAN,
        BOP_GREATERTHANEQ,
        BOP_CROSS,
        BOP_PREPLUS,
        BOP_POSTPLUS,
        BOP_PREIMAGE,
        BOP_POSTIMAGE,
        BOP_VM,
        BOP_MV,
        BOP_MM
    };
    class BinaryOperation;
    class BinaryList;

    /// Numerical operation

    class NumericalOperation;
    class NumericalList;

    /// Saturation

    class SaturationOperation;
    class SaturationList;

    extern UnaryList UOPs;
    extern BinaryList BOPs;
};

// ******************************************************************
// *                                                                *
// *                                                                *
// *                       Operation class                          *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Operation {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    Operation();
    /*-------------------------------------------------------------*/
    protected:
    /*-------------------------------------------------------------*/
    virtual ~Operation();
    // computing tables TBD
    ComputeTable        cache;

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
};

// ******************************************************************
// *                                                                *
// *                    UnaryOperation  class                       *
// *                                                                *
// ******************************************************************

class REXBDD::UnaryOperation : public Operation {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    UnaryOperation(UnaryOperationType type, Forest* source, Forest* target);
    UnaryOperation(UnaryOperationType type, Forest* source, OpndType target);


    /* Main part: check forest comatability and then calls corresponding op compute */
    void compute(const Func& source, Func& target);
    void compute(const Func& source, long& target);
    void compute(const Func& source, double& target);
    /*-------------------------------------------------------------*/
    protected:
    /*-------------------------------------------------------------*/
    virtual ~UnaryOperation();
    // computing tables TBD

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Helper Methods ==============================================
    bool checkForestCompatibility() const;
    Edge computeCOPY(const uint16_t lvl, const Edge& source);
    Edge computeCOMPLEMENT(const uint16_t lvl, const Edge& source);
    long computeCARD(const uint16_t lvl, const Edge& source);
    // list
    friend class UnaryList;
    // UnaryList&          parent;
    UnaryOperation*     next;
    // arguments
    Forest*             sourceForest;
    Forest*             targetForest;
    OpndType            targetType;
    UnaryOperationType  opType;
};

// ******************************************************************
// *                                                                *
// *                       UnaryList  class                         *
// *                                                                *
// ******************************************************************

class REXBDD::UnaryList {
    std::string name;
    UnaryOperation* front;
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    UnaryList(const std::string n = "");
    inline void reset(const std::string n) {
        front = nullptr;
        name = n;
    }
    inline std::string getName() const {return name;}
    inline bool isEmpty() const {return !front;}
    inline UnaryOperation* add(UnaryOperation* uop) {
        if (uop) {
            uop->next = front;
            front = uop;
        }
        return uop;
    }
    inline void remove(UnaryOperation* uop) {
        if (front == uop) {
            front = front->next;
            return;
        }
        searchRemove(uop);
    }
    inline UnaryOperation* find(const UnaryOperationType opT, const Forest* sourceF, const Forest* targetF) {
        if (!front) return nullptr;
        if ((front->opType == opT) && (front->sourceForest == sourceF) && (front->targetForest == targetF)) return front;
        return mtfUnary(opT, sourceF, targetF);
    }
    inline UnaryOperation* find(const UnaryOperationType opT, const Forest* sourceF, const OpndType targetT) {
        if (!front) return nullptr;
        if ((front->opType == opT) && (front->sourceForest == sourceF) && (front->targetType == targetT)) return front;
        return mtfUnary(opT, sourceF, targetT);
    }
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    void searchRemove(UnaryOperation* uop);
    UnaryOperation* mtfUnary(const UnaryOperationType opT, const Forest* sourceF, const Forest* targetF);
    UnaryOperation* mtfUnary(const UnaryOperationType opT, const Forest* sourceF, const OpndType targetT);

};

// ******************************************************************
// *                                                                *
// *                   BinaryOperation  class                       *
// *                                                                *
// ******************************************************************

class REXBDD::BinaryOperation : public Operation {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    BinaryOperation(BinaryOperationType type, Forest* source1, Forest* source2, Forest* res);

    /* Main part: computation */
    void compute(const Func& source1, const Func& source2, Func& res);
    void compute(const Func& source1, const ExplictFunc source2, Func& res);
    /*-------------------------------------------------------------*/
    protected:
    /*-------------------------------------------------------------*/
    virtual ~BinaryOperation();
    // computing tables TBD

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// Helper Methods ==============================================
    bool checkForestCompatibility() const;
    Edge computeUNION(const uint16_t lvl, const Edge& source1, const Edge& source2);
    Edge computeINTERSECTION(const uint16_t lvl, const Edge& source1, const Edge& source2);
    Edge computeIMAGE(const uint16_t lvl, const Edge& source1, const Edge& trans, bool isPre = 0);
    // elementwise related
    Edge operateLL(const uint16_t lvl, const Edge& e1, const Edge& e2);
    Edge operateHH(const uint16_t lvl, const Edge& e1, const Edge& e2);
    Edge operateLH(const uint16_t lvl, const Edge& e1, const Edge& e2);
    // list
    friend class BinaryList;
    // BinaryList&         parent;
    BinaryOperation*    next;
    // arguments
    Forest*             source1Forest;
    Forest*             source2Forest;
    OpndType            source2Type;
    Forest*             resForest;
    BinaryOperationType opType;
};

// ******************************************************************
// *                                                                *
// *                       BinaryList  class                         *
// *                                                                *
// ******************************************************************

class REXBDD::BinaryList {
    std::string name;
    BinaryOperation* front;
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    BinaryList(const std::string n = "");
    inline void reset(const std::string n) {
        front = nullptr;
        name = n;
    }
    inline std::string getName() const {return name;}
    inline bool isEmpty() const {return !front;}
    inline BinaryOperation* add(BinaryOperation* bop) {
        if (bop) {
            bop->next = front;
            front = bop;
        }
        return bop;
    }
    inline void remove(BinaryOperation* bop) {
        if (front == bop) {
            front = front->next;
            return;
        }
        searchRemove(bop);
    }
    inline BinaryOperation* find(const BinaryOperationType opT, const Forest* source1F, const Forest* source2F, const Forest* resF) {
        if (!front) return nullptr;
        if ((front->opType == opT) && (front->source1Forest == source1F) && (front->source2Forest == source2F) && (front->resForest == resF)) return front;
        return mtfBinary(opT, source1F, source2F, resF);
    }
    inline BinaryOperation* find(const BinaryOperationType opT, const Forest* source1F, const OpndType source2T, const Forest* resF) {
        if (!front) return nullptr;
        if ((front->opType == opT) && (front->source1Forest == source1F) && (front->source2Type == source2T) && (front->resForest == resF)) return front;
        return mtfBinary(opT, source1F, source2T, resF);
    }
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    void searchRemove(BinaryOperation* bop);
    BinaryOperation* mtfBinary(const BinaryOperationType opT, const Forest* source1F, const Forest* source2F, const Forest* resF);
    BinaryOperation* mtfBinary(const BinaryOperationType opT, const Forest* source1F, const OpndType source2T, const Forest* resF);

};

// ******************************************************************
// *                                                                *
// *                NumericalOperation  class                       *
// *                                                                *
// ******************************************************************

// class REXBDD::NumericalOperation : public Operation {
//     /*-------------------------------------------------------------*/
//     public:
//     /*-------------------------------------------------------------*/
//     NumericalOperation();
//     /*-------------------------------------------------------------*/
//     protected:
//     /*-------------------------------------------------------------*/
//     virtual ~NumericalOperation();
//     // computing tables TBD

//     /*-------------------------------------------------------------*/
//     private:
//     /*-------------------------------------------------------------*/
// };

// ******************************************************************
// *                                                                *
// *                SaturationOperation  class                       *
// *                                                                *
// ******************************************************************

// class REXBDD::SaturationOperation : public Operation {
//     /*-------------------------------------------------------------*/
//     public:
//     /*-------------------------------------------------------------*/
//     SaturationOperation();
//     /*-------------------------------------------------------------*/
//     protected:
//     /*-------------------------------------------------------------*/
//     virtual ~SaturationOperation();
//     // computing tables TBD

//     /*-------------------------------------------------------------*/
//     private:
//     /*-------------------------------------------------------------*/
// };


#endif