#ifndef REXBDD_TERMINAL_H
#define REXBDD_TERMINAL_H

#include "defines.h"
#include "error.h"

namespace REXBDD {
    /// Type of terminal value
    typedef enum {
        /// Null type
        NOTHING,
        /// Nothing; for EV
        OMEGA,
        /// Boolean, for MT
        BOOLEAN,
        /// Integer, for MT
        INTEGER,
        /// Real, for MT
        REAL
    } TermiType;

    class Terminal;

    // file I/O
    // TBD

}


/**
    Unified terminal object.
*/
class REXBDD::Terminal {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    /// Don't initialize
    Terminal() {
        type = NOTHING;
    }

    /// 


    //******************************************
    //  Getters for the type and value
    //******************************************
    inline bool isOmega() const {
        return (TermiType::OMEGA == type);
    }
    inline bool isBoolean() const {
        return (TermiType::BOOLEAN == type);
    }
    inline bool isInteger() const {
        return (TermiType::INTEGER == type);
    }
    inline bool isReal() const {
        return (TermiType::REAL == type);
    }
    inline bool hasType(TermiType t) const {
        return (t == type);
    }
    inline nodeHandle getOmega() const {
        REXBDD_DCASSERT(isOmega());
        return termiOmega;
    }
    inline bool getBoolean() const {
        REXBDD_DCASSERT(isBoolean());
        return termiBool;
    }
    inline int getInteger() const {
        REXBDD_DCASSERT(isInteger());
        return termiInt;
    }
    inline float getReal() const {
        REXBDD_DCASSERT(isReal());
        return termiReal;
    }
    // Get the value into any type
    // Allows for type conversion
    template <typename T>
    inline void getValue(T &v) const {
        switch (type) {
            case TermiType::OMEGA:
                    v = T(termiOmega);
                    return;

            case TermiType::BOOLEAN:
                    v = T(termiBool);
                    return;

            case TermiType::INTEGER:
                    v = T(termiInt);
                    return;

            case TermiType::REAL:
                    v = T(termiReal);
                    return;

            default:
                    REXBDD_DCASSERT(false);
        }
    }

    //******************************************
    //  Get the node handle for this terminal
    //******************************************
    inline nodeHandle getHandle() const {
        switch (type) {
        case TermiType::OMEGA:
            return termiOmega;

        case TermiType::BOOLEAN:
            return termiBool ? -1 : 0;

        case TermiType::REAL:
            // TBD
        
        default:
            break;
        }
    }

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    union {
        // for special values; usually -1
        nodeHandle  termiOmega;
        bool        termiBool;
        int         termiInt;
        float       termiReal;
    };
    // Type of the terminal
    TermiType type;
};


#endif