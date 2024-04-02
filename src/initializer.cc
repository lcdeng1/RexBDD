#include "defines.h"
#include "initializer.h"
#include "revision.h"

// ******************************************************************
// *                                                                *
// *                    initializer_list methods                    *
// *                                                                *
// ******************************************************************

REXBDD::InitializerList::InitializerList(InitializerList* prev)
{
    previous = prev;
}

REXBDD::InitializerList::~InitializerList()
{
    // DON'T delete previous
}

void REXBDD::InitializerList::setUpList(InitializerList* L)
{
    // TBD
    // wait for other parts
}

void REXBDD::InitializerList::cleanUpList()
{
    // TBD
    // wait for other parts
}

// ******************************************************************
// *                                                                *
// *                           Front  end                           *
// *                                                                *
// ******************************************************************

REXBDD::InitializerList* REXBDD::defaultInitializerList(InitializerList* prev)
{
    //  build initializer list of 
    //      memory manager, computing table, node storage, forest
    //  TBD
    return prev;
}

void REXBDD::initializeLib(InitializerList* L)
{
    InitializerList::setUpList(L);
}

void REXBDD::cleanUpLib()
{
    InitializerList::cleanUpList();
}

const char* REXBDD::getLibInfo(int what)
{
    static char* title = 0;
  switch (what) {
    case 0:
      if (!title) {
        title = new char[80];
        snprintf(title, 80,
#ifdef DEVELOPMENT_CODE
          "%s version %s.dev",
#else
          "%s version %s",
#endif
            PACKAGE_NAME, PACKAGE_VERSION
        );
      }
      return title;

    case 1:
      return PACKAGE_COPYRIGHT;

    case 2:
      return PACKAGE_LICENSE;

    case 3:
      return PACKAGE_URL;

    case 4:
      return "Data Structures and operations available:\n\
                (1) BDDs: Union, Intersection, Difference.\n\
                (2) Binary Matrix Diagrams (BMxDs): Union, Intersection, Difference.\n\
                (3) Multi-Terminal BDDs (MTBDDs) with integer or real terminals:\n\
                    Arithmetic: Plus, Minus, Multiply, Divide, Min, Max.\n\
                    Logical: <, <=, >, >=, ==, !=.\n\
                    Conversion to and from BDDs.\n\
                (4) Multi-Terminal BMxDs (MTBMxDs) with integer or real terminals:\n\
                    Arithmetic: Plus, Minus, Multiply, Divide, Min, Max.\n\
                    Logical: <, <=, >, >=, ==, !=.\n\
                    Conversion to and from BMxDs.\n\
            ";

    case 5:
      return PACKAGE_DATE;
  }
  return 0;
}