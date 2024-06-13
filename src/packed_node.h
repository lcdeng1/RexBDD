#ifndef REXBDD_PACKED_NODE_H
#define REXBDD_PACKED_NODE_H

#include "defines.h"
#include "node.h"
#include "setting.h"

namespace REXBDD {
    // first 3 bits for mark and counting up to 7: 1110 0000
    static const char MARK_MASK = (char)(((0x01<<3)-1)<<5);
    class PackedNode;
}

// ******************************************************************
// *                                                                *
// *                     Packed Node class                          *
// *                                                                *
// ******************************************************************
/** Packed node storage mechanism in a forest.
 *  The construction completely depends on the forest setting.
 *
 *  Every active node is stored in the following format:
 *      next    : uint64            {Next pointer (node handle) in the unique table.}
 *      child   : uint64[0...n]     {Down pointers (node handles) for child nodes.}
 *      values  : uint?[n+1...2n-1] {Reduction rules or edge values, slot size depends on setting.}
 *      header  : -+             +- {Bit 0 (LSB): 1 for lables/values, 0 for not.
 *                 +-- uint8 ----+  {Bit 1: 0 for size 2, 1 for size 4.
 *      flags   :  |             |  {Bit 2...6: edge flags.
 *      mark    : -+             +- {Bit 7: mark.}\
 *      padding :
 *      uint64[3]
 * 
 */
class REXBDD::PackedNode {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
    /// Default construction for Rex
    PackedNode();
    /// Construction based on forest (its settings)
    PackedNode(ForestSetting *s);
    ~PackedNode();

    /// Methods =====================================================
    /**
     *  Get the next in unique table
     */
    inline NodeHandle getPackedNext() const {return next;}

    /**
     *  Set the next for this packed node
     */
    inline void setPackedNext(NodeHandle nxt) {next = nxt;}

    /**
     *  For this packed node, is it marked?
     *  Marked if the mark counting greater than the given value
     * 
     */
    inline bool isPackedMarked(char var) const {
        return ((markFlags & MARK_MASK) >> 5) > var;
    }

    /**
     *  Get the mark count for this packed node
     * 
     */
    inline char getPackedMarks() const {
        return (markFlags & MARK_MASK) >> 5;
    }


    /**
     *  Mark and increase 1 the count for this packed node
     *  Counting unchange if reach the max
     */
    inline void markPacked() {
        if (getPackedMarks() != 7) {
            static const char flags = markFlags & ((0x01 << 5) - 1);
            static const char count = ((markFlags & MARK_MASK) >> 5) + 1;
            markFlags = flags | (count << 5);
        }
    }

    /**
     *  Unmark this packed node
     *  Decrease 1 the count if given flag 1, otherwise clear count
     *  Counting unchange if 0
     */
    inline void unmarkPacked(bool f) {
        if (getPackedMarks() > 0) {
            static const char flags = markFlags & ((0x01 << 5) - 1);
            if (f) {
                static const char count = ((markFlags & MARK_MASK) >> 5) - 1;
                markFlags = flags | (count << 5);
            } else {
                markFlags = flags;
            }
        }
    }

    /**
     *  Fill in a packed node from a given unpacked node
     *  Set next to 0 and clears the marked bits
     */
    inline void setNode(Node *P) {
        //
    }



    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
    /// ============================================================
    NodeHandle next;        // Next in unique table
    uint64_t* info;         // Down handles for children nodes, followed by labels/values
    char markFlags;         // mark bits for node, and Flags for children edges
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
 