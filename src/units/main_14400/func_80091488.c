#include "common.h"

typedef unsigned char u8;

/*
 * Allocation header of the heap built by func_800913F4 inside a caller-supplied buffer.
 * Each 0x10-byte header is immediately followed by its payload; `size` is the payload
 * capacity (multiple of 0x10) and `next` is the following header in the same buffer.
 * The payload is the GNU zero-length trailing member, backed by the rest of the buffer.
 */
typedef struct HeapBlock {
    s32 used;
    u32 size;
    struct HeapBlock *next;
    u32 arenaSize; /* +0xC: whole buffer size, written on the first header by func_800913F4 */
    u8 payload[0];
} HeapBlock;

/* Best-fit allocation; splits the chosen block when the remainder can hold another header. */
void *func_80091488(HeapBlock *b, u32 size) {
    HeapBlock *best = 0;
    u32 bestSize = 0;
    HeapBlock *bestNext = 0;
    u32 need;

    size = (size + 15) & ~15;
    need = size + sizeof(HeapBlock);
    if (size == 0) {
        return best;
    }
    do {
        if (b->used == 0 && b->size >= size) {
            if (b->size < bestSize || bestSize == 0) {
                best = b;
                bestSize = b->size;
                bestNext = best->next;
            }
        }
        b = b->next;
    } while (b != 0);
    if (bestSize != 0) {
    if (need < bestSize) {
        /* The split header starts right after the aligned payload: need - header size == size. */
        best->next = (HeapBlock *)&best->payload[need - sizeof(HeapBlock)];
        best->used = 1;
        best->size = size;
        b = best->next;
        b->next = bestNext;
        b->size = bestSize - need;
        b->used = 0;
    } else {
        best->used = 1;
    }
    return best->payload;
    }
    return 0;
}
