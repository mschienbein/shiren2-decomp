#include "common.h"

typedef unsigned char u8;

/* 0x10-byte heap block header plus allocator-owned trailing payload (`size` bytes).
 * The initial block's `total` spans the complete backing heap allocation. */
typedef struct Block Block;
struct Block {
    s32 used;
    u32 size;
    Block *next;
    u32 total;
    u8 data[0];
};

extern s32 func_800327C0(char *dst, const char *fmt, ...);
extern char D_801C33F0[];

/* Dumps the malloc block chain starting at block, stopping at a chain error. */
void func_80091794(Block *block)
{
    Block *start = block;
    Block *end = (Block *)((u8 *)block + block->total / 4 * 4);
    Block *next;

    for (;;) {
        func_800327C0(D_801C33F0, "%08x:%08x-%08x", block, block->data, (u8 *)block + sizeof(Block) + block->size - 1);
        if (block->used == 0) {
            if (block->size != 0) {
                func_800327C0(D_801C33F0, "        (%08x)[%08x] FREE", block->size, block->next);
            } else {
                func_800327C0(D_801C33F0, "        (%08x)[%08x] ALIN", block->size, block->next);
            }
        } else {
            func_800327C0(D_801C33F0, "        (%08x)[%08x] USE", block->size, block->next);
        }
        next = block->next;
        if (next == 0) {
            return;
        }
        /* local-arithmetic-qualification: alignment test of a chain link read from heap memory. */
        if (next < start || end < next || ((u32)next & 3)) {
            func_800327C0(D_801C33F0, "malloc_memdsp error:%08x over area %08x-%08x", block->next, start, end);
            return;
        }
        block = next;
    }
}
