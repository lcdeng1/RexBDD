*–––––––––––––––––––––––––––––––––––––––––––––
* Class Structure View
*–––––––––––––––––––––––––––––––––––––––––––––
                                     +++++++++
====================================|| Edge: ||========================================
                                     ---------
    |
    |   R(ule)/V(alue) –––––– value    -|
    |                                   |
    |   C(omplement)   -|               |–––– edgeLabel  -|
    |                   |–––– flag     -|                 | 
    |   S(wap)         -|                                 |
    |                                                     |–––– edge
    v                                                     |
   (P)  nodeHandle                                       -|

    * ValueType:
        {VOID, RULE, INT, LONG, FLOAT, DOUBLE}
    * FlagType:
        {NAIVE,
         COMP,
         SWAP_ONE, SWAP_ALL, SWAP_ONEALL, SWAP_MIX,
         CO_SWAP_ONE, CO_...,
         REL_SWAP_FROM, REL_SWAP_TO, REL_SWAP_FROMTO, REL_SWAP_MIX, 
         REL_CO_SWAP_FROM, REL_CO_...
        }
        SWAP_MIX needs two bits, so placeholder for future.
    * nodeHandle:
        1. 64 bits: Single array of pointers in node manager
        2. 32 + 32 bits: L arrays of pointers divided by levels in node manager
            ^    ^
            |    |
            |    index of this array
            level
            Depends on the supported MAX level and number of nodes.

                                 ++++++++++++++++++
================================|| Terminal Node: ||===================================
                                 ------------------
    T: nodeHandle (<=0)
        Type:   Nothing     ----> Null type
                Omega       ----> Terminal for edge-valued BDDs (-1)
                Boolean     ----> Ture (-1), False (0)
                Integer     ----> For integer multi-terminal BDDs
                Real        ----> For real ...

                                     +++++++++
====================================|| Node: ||========================================
                                     ---------
                    (P)         level
                   /   \
                  /     \       edgeLabel[2]
                 /       \
                /         \
              (Q0)       (Q1)   nodeHandle[2]
    
    * hashValue:
        Returned by computeHash();
    * 

    KEY Methods:
        1. computeHash():
            Compute this node's hash value.
            Note: down pointers, edgeLabels, and extra header information?

                                 +++++++++++++++++
================================|| Node Manager: ||====================================
                                 -----------------
                (nodeHandle)
                    |
    Levels:         v
             ________________________________________________________
            | 0 |   4   |   7   |   5   |       ...     |  9   |  3  |
             ––––––––––––––––––––––––––––––––––——––––––––––––––––––––
    Addresses:
             ________________________________________________________
            | x | ptr1  | free  | ptr3  |       ...     | free | ptr |
             ––––––––––––––––––––––––––––––––––——––––––––––––––––––––
                  /
                 /
                / point to the memory slot
               /                   
              /
             /
            |
            v              (Actual Node Stored in Memory Slot)
             _______________________________________________________________
            |       |       |       |       |       ...     |       |       |       
             ––––––––––––––––––––––––––––––––––——–––––––––––––––––––––––––––
             _______________________________________________________________
            |       |       |       |       |       ...     |       |       |       
             ––––––––––––––––––––––––––––––––––——–––––––––––––––––––––––––––
                                                    .
                                                    .
                                                    .
             _______________________________________________________________
            |       |       |       |       |       ...     |       |       |       
             ––––––––––––––––––––––––––––––––––——–––––––––––––––––––––––––––
             
            Must Include:
                * Next pointer (nodeHandle) in Unique Table     64 bits     (8 Bytes)
                * Low and High edgeValue                        0-64*2 bits (0-16 Bytes)
                * Low and High down pointers (nodeHandle)       128 bits    (16 Bytes)
                * Low and High flags                            0-3 bits
                * Mark bit                                      1 bit
                * Padding:
                    edge value type?
                    edge flag type?
                    level?
                Note: the recycled node in memory sets all parts to 0 except for 
                    the specific part (i.e., Low down pointer) which is set to the 
                    pointer to the next free slot.

    KEY Methods:
        1. recycleHandle(nodeHandle h):
            Recycle a used nodeHandle (index). The recycled handle can eventually
            be reused when returned by a call to obtainHandle().
        2. obtainHandle(Node n):
            Get an unoccupied nodelHandle (index), and fill it with the node n.
            Note: check recycled handle, check if enlarge needed.
        3. sweep():
            Sweep each node if it is marked or not.

                                 +++++++++++++++++
================================|| Unique Table: ||====================================
                                 -----------------
                (node hashValue)
                        |
                        |   [check duplicates / insert]
                        |
    nodeHandle:         v
             ______________________________________________________________
            | 6575  | 5312  | null  | 9527  |       ...     |       |      |
             ––––––––––––––––––––––––––––––––––——––––––––––––––––––––––––––
                        |
                        |   next pointer (stored in)
                        |
                        v
                    Next nodeHandle
    KEY Methods:
        1. insertNode(Node n):
            Add a node to the unique table. If unique, returns the handle obtained from 
            node manager; otherwise, returns the existed handle and recycle the given 
            node and its obtained hanlde.
            Note: In either case, the returned node handle becomes the front entry of 
            the hash chain.
        2. sweep():
            Remove all marked or unmarked node handles from the unique table.
                                     +++++++++++
====================================|| Forest: ||======================================
                                     -----------
    * Polices:
        1. ForestType:
            {RexBDDs (including other less reduced BDDs),   -|
             MTBDDs,                                         |–––– Sets forest
             EV(+, %, *)BDDs,                               -|
             BMxDDs                                           –––– Relation forest
            }
        2. Domain:
            {Boolean {0, 1},
             Integer (-∞, ..., -1, 0, 1, 2, ..., +∞) U {+∞},
             Non-negative [0, 1, 2, ..., +∞],
             Real,
             ...,
             Ω,
            }
        3. MaxLevel: L
            Up to 2^32-1 (int)
        4. VariableOrder:
            level2var: level_index
            var2level: variable_index
        5. ReductionRule:
            Supported: 
                {N (Quasi),
                 X (Full),
                 ELt (t \in {0, 1}),
                 EHt,
                 ALt,
                 AHt,
                 It (Identity)
                }
            Any subset of supported
        
    * Unique Tables:
        level/var 1: UT 1
        level/var 2: UT 2
        level/var 3: UT 3
            ...
        level/var L: UT L

    * Node Manager:
        1. Single array of node handles
        2. L arrays divided by Levels

    * Node Marker: 
        Reachable， for garbage collection

    * Roots: Edge[]

    * Forests:
        Array of all forests

    * FID:
        Index in Forests

    KEY Methods:
        1. createForest():
            Create and initialize a forest.
        2. createReducedNode(Node n):
            For "Sets" forest, it returns a forest node handle equal to the one given.
            Note:
                Apply the reduction rules,
                Normalize,
                Check for duplicates in Unique table, if found then return the handle
                Otherwise, obtain a free handle from node manager and fill it,
                Create this new node in node manager,
                Add it to Unique table
        3. evaluateSet(Edge e, VarList bools):
            Evaluate the function encoded by an edge e of a "Sets" forest, it returns 
            the terminal value f(x1=bools[v1], ..., xL=bools[vL]).

        4. evaluateRelation(Edge e, VarList initBools, VarList nextBools):
            Evaluate the function encoded by an edge e of a "Relation" forest, it returns 
            the terminal value f(x1=initBools[v1], x'1=nextBools[v1], ..., xL=initBools[vL], 
            x'L=nextBools[vL]).


    

                                     ++++++++++++++
====================================|| Operation: ||===================================
                                     --------------




                                 ++++++++++++++++++++
================================|| Computing Table: ||=================================
                                 --------------------
                (entries hashValue)
                        |
                        |   [check / insert]
                        |
    Pointers:           v
             _______________________________________________________________
            |  ptr  |  ptr  | null  |  ptr  |       ...     |  null |  ptr  |
             ––––––––––––––––––––––––––––––––––——–––––––––––––––––––––––––––
                    /
                   /
                  /
                 /
                / point to the memory slot
               /                   
              /
             /
            |
            v                   (Results in Memory Slot)
             _______________________________________________________________
            |  next |       |       |       |       ...     |       |       |   -|
             ––––––––––––––––––––––––––––––––––——–––––––––––––––––––––––––––     |
                |                                                                |
                v                                                                |      Depth 3
             _______________________________________________                     |      Size depends on
            |  next |       |       ...     |       |       |                    |––––  Operators
             ––––––––––––––––––––––––––––––––––——–––––––––––                     |
                |                                                                |
                v                                                                |
             _______________________________________________________             |
            |  null |       |       |       |       ...     |       |           -|
             ––––––––––––––––––––––––––––––––––——–––––––––––––––––––
    * Entries:
        Operator Name,
        Operands:
            Forests: FID
            Edges: level, edgeLabels, nodeHandles

    KEY Methods:
    1. check(Entries, Result):
        Check the result of the given entries.
        If cached, return 1 and save the result to Result object;
        otherwise, return 0.
    2. cache(Entries, Result):
        Insert the given entries and corresponding result into computing table.
        If the hash chain has reached the depth 3, overwrite the front.
    3. sweep():
        Remove all the elements that contains marked or unmarked node handles.




*–––––––––––––––––––––––––––––––––––––––––––––
* Desired Operation
*–––––––––––––––––––––––––––––––––––––––––––––

For Forests
    * Unary:
        1. Cardinality:
            Return the number of variable assignments
        2. Complement:
            For Boolean forest, flip the return values
        3. Copy/Convert:
            Copy a function across different type of forests
        4. Max/Min:
            Find the largest/smallest value returned by the function
        5. Select:
            Randomly select one state from a set of states
        6. ShiftUp/Down:
            Swap a level of variable with higher/lower adjacent level
        7. Reorder:
            Change to the target variable order

    * Binary
        1. Union
        2. Intersection
        3. Difference
        4. Cross:
            Combine two functions into a single one, where the operands are "Sets" 
            forests and the result is a "Relation" forest. Specifically, for operands 
            f and g, produces h where
                h(x1, x'1, ..., xL, x'L) = f(x1, ..., xL) * g(x'1, ..., x'L)
        5. Maximum/Minimum:
        6. Plus
        7. Minus
        8. Multiply
        9. Divide
        10. Equal
        11. NotEqual
        12. LessThan
        13. LessThanEqual
        14. GreaterThan
        15. GreaterThanEqual
        16. PrePlus/PostPlus:
            Plus operation used to compute transitive closure and further minimum witness
        17. PreImage/PostImage:
            Image operations on a set-of-states with a transition function. The first operand 
            must be the set-of-states and the second operand must be the transition function. 
            The result is a set-of-states that must be stored in the same forest as the first operand.
            Applies to:
            TC_POST_IMAGE,
            REACHABLE_STATES_DFS, REACHABLE_STATES_BFS,
            REVERSE_REACHABLE_DFS, REVERSE_REACHABLE_BFS.
        18. MatrixMultiply
        ...

    * Ternary
        ...

    * Numerical
        ...

    * Saturation
        ...

For Edges attached to the SAME forest
    * Binary operators: 
        {+, -, *, /, &&, ||, !}
    * Update operators:
        {+=, -=, *=, /=, &=, |=}
