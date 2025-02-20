#include "lib_manager.h"
#include "info.h"

namespace REXBDD{
  // ******************************************************************
  // *                                                                *
  // *                           Front end                            *
  // *                                                                *
  // ******************************************************************
  const char* getLibInfo(int what)
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
              PROJECT_NAME, PROJECT_VERSION
          );
        }
        return title;

      case 1:
        return PROJECT_COPYRIGHT;

      case 2:
        return PROJECT_LICENSE;

      case 3:
        return PROJECT_URL;

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
        return PROJECT_DATE;
    }
    return 0;
  }

  void printInfo()
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

  InitializerList* defaultInitializerList(InitializerList* prev)
  {
      //  build initializer list of 
      //      memory manager, computing table, node storage, forest
      //  TBD
      return prev;
  }

  void initializeLib(InitializerList* L)
  {
      InitializerList::setUpList(L);
  }

  void cleanUpLib()
  {
      InitializerList::cleanUpList();
  }

  // ******************************************************************
  // *                                                                *
  // *                    initializer_list methods                    *
  // *                                                                *
  // ******************************************************************

  InitializerList::InitializerList(InitializerList* prev)
  {
      previous = prev;
  }

  InitializerList::~InitializerList()
  {
      // DON'T delete previous
  }

  void InitializerList::setUpList(InitializerList* L)
  {
      // TBD
      // wait for other parts
      // isRunning = true;
  }

  void InitializerList::cleanUpList()
  {
      // TBD
      // wait for other parts
      // isRunning = false;
  }
} // end namespace