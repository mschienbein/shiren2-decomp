#include "common.h"

typedef unsigned char u8;

/* Same heap layout as the allocating consumer func_8002AB40. */
typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;

/* The original returns the reloaded, aligned heap base pointer. */
u8 *func_8002AB90(ALHeap *record, u8 *address, u32 span_word)
{
    /* local-arithmetic-qualification: alignment requires the low address bits;
     * pointer arithmetic alone cannot express the original address & 15 mask.
     * All stored addresses, the argument and the return remain pointers. */
    u32 adjustment = 16U - ((u32)address & 15U);
    u8 *result;

    if (adjustment != 16U) {
        record->base = address + adjustment;
    } else {
        record->base = address;
    }
    result = record->base;
    record->len = span_word;
    record->count = 0;
    record->cur = result;
    return result;
}
