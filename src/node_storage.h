#ifndef REXBDD_NODE_STORAGE_H
#define REXBDD_NODE_STORAGE_H

#include "defines.h"
#include "node.h"

namespace REXBDD {
    class NodeStorage;
}

// ******************************************************************
// *                                                                *
// *                   Node Storage  class                          *
// *                                                                *
// ******************************************************************
/*
 *
 */
class REXBDD::NodeStorage {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/

    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
};


#endif

/** Original node storage mechanism in a forest.
    Memory management is completely separated from the class,
    which makes this implementation a little different
    from the original.

    Every active node is stored in the following format.

 common   {  slot[0] : next pointer >=0 in unique table.
 header --{  slot[1] : Bit 0 (LSB) : 1 for extensible, 0 for not.
                       Bit 1       : 1 for sparse, 0 for full.
                       Bit MSB..2  : size

 unhashed    {       : slots used for any extra information
 header    --{       : as needed on a forest-by-forest basis.
 (optional)  {       : Info does NOT affect node uniqueness.

 hashed      {       : slots used for any extra information
 header    --{       : as needed on a forest-by-forest basis.
 (optional)  {       : Info DOES affect node uniqueness.

             {       : Downward pointers.
             {       : If full storage, there are size pointers
 down -------{       : and entry i gives downward pointer i.
             {       : If sparse storage, there are -size pointers
             {       : and entry i gives a pointer but the index
             {       : corresponds to index[i], below.

             {       : Index entries.
             {       : Unused for full storage.
 index ------{       : If sparse storage, entry i gives the
 (sparse)    {       : index for outgoing edge i, and there are
             {       : -size entries.

             {       : Edge values.
 edge        {       : If full storage, there are size * edgeSize
 values -----{       : slots; otherwise there are -size * edgeSize
             {       : slots.  Derived forests are responsible
             {       : for packing information into these slots.

           { -padlen : Any node padding to allow for future
           {         : expansion, or for memory management purposes
           {         : (e.g., memory hold is larger than requested).
 padding --{         : padlen is number of padded slots.  If the
           {         : first entry after the node proper is negative,
           {         : then it specifies the number of long padding
           {         : slots; otherwise, there is no padding.

 tail    --{ slot[L] : The forest node number,
                       guaranteed to be non-negative
                       (MSB cleared).


 */
#ifdef __SIZEOF_POINTER__ == 8

#endif