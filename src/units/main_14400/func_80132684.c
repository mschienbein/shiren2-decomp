#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* MSB-first bit reader over a word stream (same layout as func_80132050's). */
typedef struct BitStream { u32 mask; u32 word; const u32 *next; } BitStream;

/*
 * Huffman trees built by func_80132050, viewed as s16 arrays: entry 0 is the
 * root; an internal node n (>= 0x100) has its children at entries 2n + 1 and
 * 2n + 2; a leaf n (< 0x100) of a value tree keeps its value at entry 2n + 1.
 */
extern s16 D_801CF660[];
extern s16 D_801D0E90[];
extern s16 D_801D1694;
extern s16 D_801D1696;

static __inline__ s32 read_bit(BitStream *stream) {
    s32 bit;
    if (!stream->mask) {
        stream->word = *stream->next++;
        stream->mask = 0x80000000;
    }
    bit = (stream->word & stream->mask) != 0;
    stream->mask >>= 1;
    return bit;
}

/* Walk from the root, following one bit per internal node; `slot` points at
 * the tree entry holding the current node. */
static __inline__ s16 find_leaf(s16 *tree, BitStream *stream) {
    s16 *slot = tree;
    s16 node = *slot;

    while (node >= 0x100) {
        /* local-arithmetic-qualification: the in-bounds form &tree[node * 2]
         * (and tree + node * 2) emits the base/offset addu operands in the
         * reverse order under GCC 2.7.2; the address stays inside the tree. */
        s16 *pair = (s16 *)((u32)(node * 4) + (u32)tree);
        slot = &pair[1];
        if (read_bit(stream)) {
            slot = &pair[2];
        }
        node = *slot;
    }
    return node;
}

static __inline__ s16 decode_value(s16 *tree, BitStream *stream) {
    s16 node = find_leaf(tree, stream);
    s16 *leaf = tree + node * 2;
    return leaf[1];
}

/* Decode one value: a zero symbol loads a run length into *run; escape symbols chain. */
s32 func_80132684(u8 *run, BitStream *values, BitStream *runs) {
    s16 value;
    s16 next;

    if (*run == 0) {
        value = decode_value(D_801CF660, values);
        if (value == 0) {
            *run = find_leaf(D_801D0E90, runs);
        } else {
            if (value == D_801D1696 || value == D_801D1694) {
                do {
                    next = decode_value(D_801CF660, values);
                    value += next;
                } while (next <= D_801D1696 || next >= D_801D1694);
            }
            return value;
        }
    } else {
        (*run)--;
    }
    return 0;
}
