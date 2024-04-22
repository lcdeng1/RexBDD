/** @name defines.h
    @type File
    @args \ 

  The base of all files.  So if you change this, everything gets to recompile.

  This file is for good global defines, such as ASSERT and TRACE and crud.

  Since this file is only intended for global definitions, there is no
  associated defines.c or defines.cc file.
 */

#ifndef REXBDD_DEFINES_H
#define REXBDD_DEFINES_H


// #ifdef HAVE_CONFIG_H
// #include "config.h"
// #endif

// Things that everyone will need
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdarg>
#include <limits>

// Handy Constants

namespace REXBDD {
  // const int INF = std::numeric_limits<int>::max();
  // const float NAN = std::numeric_limits<float>::quiet_NaN();
  template<typename T>
  inline T Inf()    { return std::numeric_limits<T>::max(); }
  // inline float Nan()  { return std::numeric_limits<float>::quiet_NaN(); }
  // inline bool isNan(float t) { return t != t; }
  // inline bool isNan(int t) { return false; }

  // Handy Macros

    /// Standard MAX "macro".
    template <class T> inline T MAX(T X,T Y) { return ((X>Y)?X:Y); }
    /// Standard MIN "macro".
    template <class T> inline T MIN(T X,T Y) { return ((X<Y)?X:Y); }
    /// Standard ABS "macro".
    template <class T> inline T ABS(T X) { return ((X<0)?(-X):(X)); }
    /// SWAP "macro".
    template <class T> inline void SWAP(T &x, T &y) { T tmp=x; x=y; y=tmp; }
    /// POSITIVE "macro".
    template <class T> inline bool POSITIVE(T X) { return (X>0) ? true : false; }

    /// Update a maximum
    template <class T> inline void UPDATEMAX(T &X, T Y) { if (Y > X) X = Y; }

    // Number of digits
    template <class T>
    inline unsigned digits(T a) {
        unsigned d;
        for (d=1; a; d++) { a /= 10; }
        return d;
    }

  /// Get the "top level" of an operation.  Works for primed & unprimed.
  inline int topLevel(int k1, int k2) {
    if (ABS(k1) > ABS(k2)) return k1;
    if (ABS(k2) > ABS(k1)) return k2;
    return MAX(k1, k2);
  }

  /// Determine if level k1 is above k2.  Works for primed & unprimed.
  inline bool isLevelAbove(int k1, int k2) {
    if (ABS(k1) > ABS(k2)) return true;
    if (ABS(k2) > ABS(k1)) return false;
    return k1 > k2;
  }

}; // namespace


// Use this for assertions that will fail only when your
// code is wrong.  Handy for debugging.
#ifdef DCASSERTS_ON
#include <cassert>
#define REXBDD_DCASSERT(X) assert(X)
#else
#define REXBDD_DCASSERT(X)
#endif


//
// Typedefs and constants
//

namespace REXBDD {

    /** Handles for nodes.
        This should be either int or long, and effectively limits
        the number of possible nodes per forest.
        As an int, we get 2^32-1 possible nodes per forest,
        which should be enough for most applications.
        As a long on a 64-bit machine, we get 2^64-1 possible nodes
        per forest, at the expense of nearly doubling the memory used.
        This also specifies the incoming count range for each node.
    */
    typedef long long  nodeHandle;

    /** Handles for relation nodes.
        TBD: can we just use node_handle everywhere?
     */
    typedef int  rel_nodeHandle;

    /** Node addresses.
        This is used for internal storage of a node,
        and should probably not be changed.
        The typedef is given simply to clarify the code
        (hopefully :^)
    */
    typedef unsigned long nodeAddress;

    // int POINTER_SIZE = sizeof(void*);
}; // namespace

#endif
