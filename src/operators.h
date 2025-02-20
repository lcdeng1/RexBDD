#ifndef REXBDD_OPERATORS_H
#define REXBDD_OPERATORS_H

#include "edge.h"
#include "function.h"
#include "io.h"
#include "operations/apply.h"

namespace REXBDD {
    /* Operators for Edges */

    Edge operator+(const Edge &e1, const Edge &e2);
    Edge operator-(const Edge &e1, const Edge &e2);
    Edge operator*(const Edge &e1, const Edge &e2);
    Edge operator/(const Edge &e1, const Edge &e2);

    Edge operator&(const Edge &e1, const Edge &e2);
    Edge operator|(const Edge &e1, const Edge &e2);
    Edge operator^(const Edge &e1, const Edge &e2);
    Edge operator!(const Edge &e);

    Edge operator+=(Edge &e1, const Edge &e2);
    Edge operator-=(Edge &e1, const Edge &e2);
    Edge operator*=(Edge &e1, const Edge &e2);
    Edge operator/=(Edge &e1, const Edge &e2);

    Edge operator&=(Edge &e1, const Edge &e2);
    Edge operator|=(Edge &e1, const Edge &e2);

    /* Operators for Funcs */
    
    Func operator+(const Func &e1, const Func &e2);
    Func operator-(const Func &e1, const Func &e2);
    Func operator*(const Func &e1, const Func &e2);
    Func operator/(const Func &e1, const Func &e2);

    Func operator&(const Func &e1, const Func &e2);
    Func operator|(const Func &e1, const Func &e2);
    Func operator^(const Func &e1, const Func &e2);
    Func operator!(const Func &e);

    Func operator+=(Func &e1, const Func &e2);
    Func operator-=(Func &e1, const Func &e2);
    Func operator*=(Func &e1, const Func &e2);
    Func operator/=(Func &e1, const Func &e2);

    Func operator&=(Func &e1, const Func &e2);
    Func operator|=(Func &e1, const Func &e2);

    /* Operators for comparing */
    bool operator==(const Edge &e1, const Edge &e2);

    /* These will let us do C++ style output, with our output class */
    
    inline Output& operator<<(Output &s, const ForestSetting &setting);
    inline Output& operator<<(Output &s, const Edge &e);
    inline Output& operator<<(Output &s, char x);
    inline Output& operator<<(Output &s, std::string x);
    inline Output& operator<<(Output &s, int x);
    inline Output& operator<<(Output &s, long x);
    inline Output& operator<<(Output &s, unsigned x);
    inline Output& operator<<(Output &s, unsigned long x);
    inline Output& operator<<(Output &s, double x);
    
} // namespace REXBDD




#endif