#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct BitStream { u32 mask; u32 word; const u32 *next; } BitStream;
typedef struct { s16 left; s16 right; } TreeNode;
typedef struct Tree { s16 root; TreeNode nodes[512]; } Tree;
extern u16 D_801D1692;
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
s32 func_80132050(BitStream *stream, Tree *tree) {
    if (read_bit(stream)) {
        s32 index = (s16)D_801D1692++;
        tree->nodes[index].left = func_80132050(stream, tree);
        tree->nodes[index].right = func_80132050(stream, tree);
        return index;
    } else {
        u8 b7 = read_bit(stream);
        u8 b6 = read_bit(stream);
        u8 b5 = read_bit(stream);
        u8 b4 = read_bit(stream);
        u8 b3 = read_bit(stream);
        u8 b2 = read_bit(stream);
        u8 b1 = read_bit(stream);
        u8 b0 = read_bit(stream);
        return b0 | ((b7 << 7) | (b6 << 6) | (b5 << 5) | (b4 << 4) |
                     (b3 << 3) | (b2 << 2) | (b1 << 1));
    }
}
