# RexBDD
RexBDD stands for Reduction-on-Edge Complement-and-Swap Binary Decision Diagram.
This is an open-source library that supports several types of binary decision diagrams (BDDs), including reduced ordered BDDs, zero-suppressed BDDs, and BDDs integrating multiple reduction rules, complement, and swap flags.

**Note:** this project is implemented in C++, the prototype version implemented in C can be refered to [RexDD](https://github.com/asminer/RexDD).

## Building the library
Project is under development... \^_^

[CMake](https://cmake.org/) is required to build.
1. Set up the build files:
    ```
    $ mkdir build
    $ cd build
    $ cmake ..
    ```
2. Now start to build:
   ```
   $ make
   ```
   Library will be in ```build/src/```. Test and example applications in ```build/tests/``` and ```build/examples/```. This may be sufficient for most users.
3. Remove the results of compilation:
   ```
   $ make clean
   ```


##### Acknowledgments
This project has been supported in part by the [National Science Foundation](http://www.nsf.gov) under grants CCF-2212142.