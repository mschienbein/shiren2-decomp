#include "common.h"

typedef unsigned char u8;

/* Allocation header of this heap (layout owned by func_80091488): each 0x10-byte header
 * is followed by its payload; the first header also records the whole arena size. */
typedef struct HeapBlock {
    s32 used;
    u32 size;
    struct HeapBlock *next;
    u32 arenaSize;
    u8 payload[0];
} HeapBlock;

/* Builds a heap with one free block inside a caller-supplied buffer. */
void *func_800913F4(void *buffer, u32 size)
{
    HeapBlock *block;

    if (size < 0x20) return 0;
    /* local-arithmetic-qualification: rounds the buffer address up to a 16-byte boundary;
     * pointer arithmetic cannot express the address mask. */
    block = (HeapBlock *)(((u32)buffer + 15) & ~15);
    size -= (u8 *)block - (u8 *)buffer;
    if (size < 0x20) return 0;
    block->next = 0;
    block->size = size - 0x10;
    block->used = 0;
    block->arenaSize = size;
    return block;
}
