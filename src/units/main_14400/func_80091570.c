#include "common.h"

typedef unsigned char u8;

/* Each header and its variable-length payload share one backing heap allocation. */
typedef struct Block80091570 {
    s32 used;
    s32 size;
    struct Block80091570 *next;
    s32 pad0C;
    u8 payload[0];
} Block80091570;

/* ODD_C: header-plus-payload span as an inline helper; it names the merged
   extent and also keeps GCC from reassociating size + 16 + other->size. */
static inline s32 block_span(s32 size) { return size + 16; }

/* Free ptr in heap: coalesce with a free successor and a free predecessor. */
s32 func_80091570(Block80091570 *heap, void *ptr) {
    Block80091570 *cursor;
    Block80091570 *block;
    Block80091570 *next;
    s32 size;

    if (ptr == 0) return 1;
    /* The header sits immediately before the payload handed to callers. */
    cursor = (Block80091570 *)ptr - 1;
    block = cursor;
    if (cursor->used == 0) {
        return 1;
    }
    next = cursor->next;
    size = cursor->size;
    cursor = next;
    if (cursor != 0 && cursor->used == 0) {
        size = block_span(size) + cursor->size;
        next = cursor->next;
    }
    cursor = heap;
    for (;;) {
        if (cursor->next == block) {
            if (cursor->used == 0) {
                block = cursor;
                size = block_span(size) + block->size;
            }
            break;
        }
        if (block < cursor || cursor->next == 0) {
            break;
        }
        cursor = cursor->next;
    }
    cursor = block;
    cursor->next = next;
    cursor->size = size;
    cursor->used = 0;
    return 0;
}
