#ifndef REXBDD_INITIALIZER_H
#define REXBDD_INITIALIZER_H

#include "defines.h"

namespace REXBDD {
    class ForestList;
    class InitializerList;

    // ******************************************************************
    // *                   Library Front End Functions                  *
    // ******************************************************************
    /** Get the information about the library.
        @param  what  Determines the type of information to obtain.
        @return A human-readable information string.
                The string depends on parameter \a what, as follows.
                0: Title string, e.g., "MDD library version 0.0.0"
                1: Copyright string
                2: License string
                3: Reference url
                4: String with library features
                5: Release date
                Anything else: null string.
    */
    const char* getLibInfo(int what = 0);

    void printInfo();

    /**
        Build list of initializers for Meddly.
        Custom-built initialization lists will usually include this list.
            @param    prev  Initializers to execute before the default list;
                            can be null.

            @return   List of initializers.
    */
    InitializerList* defaultInitializerList(InitializerList* prev);
    /** Initialize the library with custom settings.
        Should be called before using any other functions.
            @param  L   List of initializers.  Will execute the "setup()"
                        methods in order now, and the "cleanup()" methods
                        in reverse order on library cleanup.
    */
    void initializeLib(InitializerList* L);
    /** Initialize the library with default settings.
        See meddly_expert.h for functions to initialize
        the library with non-default settings.
        Should be called before using any other functions.
    */
    void initializeLib();
    /** Clean up the library.
        Can be called to free memory used by the library;
        after it is called, the library may be initialized again.
    */
    void cleanUpLib();

}; // namespace

// ******************************************************************
// *                                                                *
// *                     initializer_list class                     *
// *                                                                *
// ******************************************************************

/** Mechanism for initializing and/or cleaning up library structures.
    Any user additions to the library should utilize this class.
    Derive a class from this one, provide the \a setup and \a cleanup
    methods.
*/
class REXBDD::InitializerList {
    public:
        /**
            Constructor.
            Takes the initializer(s) to run before this one.
            Cleanup runs in the reverse order.
        */
        InitializerList(InitializerList* previous);
        virtual ~InitializerList();

        /**
            Initialize the library from the given list.
        */
        static void setUpList(InitializerList* L);

        /**
            Clean up the library using the same list
            passed to initializeLibrary().
        */
        static void cleanUpList();
        
        /// Did we initialize the library?
        static inline bool libIsRunning() { return isRunning; }

    protected:
        virtual void setup() = 0;
        virtual void cleanup() = 0;

    private:
        InitializerList* previous;
        static InitializerList* initializers;
        static bool isRunning;
};

#endif