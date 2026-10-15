#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* MSB-first bit reader over a word stream (same layout as func_80132050's). */
typedef struct BitStream { u32 mask; u32 word; const u32 *next; } BitStream;

extern BitStream D_801CF5A8[4];
/* Huffman trees built by func_80132050, viewed as s16 arrays: entry 0 is the
 * root; an internal node n (>= 0x100) has its children at entries 2n + 1 and
 * 2n + 2; leaves (< 0x100) are the decoded byte symbols. */
extern s16 D_801CFE70[];
extern s16 D_801D0E90[];
extern s32 D_801D252C;
extern s32 D_801D2538;
/* Interleaved byte planes (one byte every two); module bss shared with
 * func_801328A8 and the decoder setup code. */
extern u8 *D_801D1698;
extern u8 *D_801D169C;
extern u8 *D_801D16A0;

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
 * the tree entry holding the current node.  Returns the leaf's byte symbol. */
static __inline__ u8 decode_symbol(BitStream *stream, s16 *tree) {
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

/* Decode the luma plane, then the packed two-nibble chroma planes, with zero runs. */
void func_80132324(void) {
    s32 n;
    u8 *dst;
    u8 *lo;
    u8 *hi;
    s32 count;

    n = D_801D252C;
    dst = D_801D1698;
    while (n > 0) {
        u8 v = decode_symbol(&D_801CF5A8[0], D_801CFE70);
        if (v == 0) {
            u8 run = decode_symbol(&D_801CF5A8[2], D_801D0E90);
            s32 len = run + 1;
            n -= len;
            for (count = run; count != -1; count--) {
                dst[1] = 0;
                dst += 2;
            }
        } else {
            dst[1] = v;
            dst += 2;
            n--;
        }
    }

    n = D_801D2538;
    lo = D_801D169C;
    hi = D_801D16A0;
    while (n > 0) {
        u8 code = decode_symbol(&D_801CF5A8[1], D_801CFE70);
        if (code == 0) {
            u8 run = decode_symbol(&D_801CF5A8[3], D_801D0E90);
            s32 len = run + 1;
            n -= len;
            for (count = run; count != -1; count--) {
                hi[1] = 0;
                hi += 2;
                lo[1] = 0;
                lo += 2;
            }
        } else {
            lo[1] = code & 0xF;
            lo += 2;
            hi[1] = code >> 4;
            hi += 2;
            n--;
        }
    }
}
