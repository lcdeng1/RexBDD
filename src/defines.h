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
#include <cmath>

//
// Typedefs and constants
//
#define LEVEL_LIMIT 32    // This limits the max number of possible levels: 2^32-1
#define HANDLE_LENGTH 64

namespace REXBDD {
    /**
     *  Null-terminated sequence of primes to use as hash table sizes.
     *  About this list:
     *      (1) They approximately double in size until about 2 million
     *      (2) They increase by about 1.4 (square root of 2) until
     *          about 400 million
     *      (3) They increase by about 1.26 (cube root of 3) to the end
     *      (4) The list stops around 2^49.
     *
     */
    static const uint64_t PRIMES[] = {
        2, 5, 11, 23, 47, 97, 193, 389, 773, 
        1009, 2027, 4057, 8117, 16249, 32503, 65011, 130027, 260081, 520193,
        1040387, 2080777, 2913139, 4078397, 5709757, 7993663, 11191153, 15667633,
        21934691, 30708577, 42992021, 60188837, 84264377, 117970133, 165158207,
        231221491, 323710097, 407874751, 513922187, 647541971, 815902909,
        1028037671, 1295327513, 1632112667, 2056461989, 2591142131, 3264839087,
        4113697319, 5183258669, 6530905951, 8228941499, 10368466351, 13064267633,
        16460977223, 20740831313, 26133447503, 32928143879, 41489461309,
        52276721261, 65868668837, 82994522759, 104573098691, 131762104351,
        166020251489, 209185516877, 263573751283, 332102926681, 418449687637,
        527246606423, 664330724111, 837056712409, 1054691457653, 1328911236643,
        1674428158229, 2109779479439, 2658322144133, 3349485901661, 4220352236131,
        5317643817529, 6700231210093, 8442291324737, 10637287069183,
        13402981707197, 16887756951073, 21278573758373, 26811002935583,
        33781863698897, 42565148260643, 53632086808429, 67576429378651,
        85146301017101, 107284339281571, 135178267494863, 170324617043543,
        214609017474941, 270407362018439, 340713276143243, 429298727940493,
        540916397205031, 0 };

    /**
     *  Handles for nodes storage
     *  This effectively limits the number of possible nodes per forest.
     *  Each handle is constructed as:
     *                  [ level | index ]
     *  "level" occupies the least number of bits to cover the largest 
     *  number of levels in the forest: |log(maxLevel)| <= LEVEL_LIMIT
     *  "index" in node manager occupies the rest bits
     *
     */
    typedef uint64_t NodeHandle;

}; // namespace

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


#endif
