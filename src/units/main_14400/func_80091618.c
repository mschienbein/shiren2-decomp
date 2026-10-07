#include "common.h"

/* Seven-word output record written by func_80091650, offsets 0x00..0x18. */
typedef struct {
    s32 total;
    s32 used;
    s32 largest_used;
    s32 smallest_used;
    s32 free;
    s32 largest_free;
    s32 smallest_free;
} HeapStats;

extern void *D_80140070;
s32 func_80091650(void *heap, HeapStats *out);

s32 func_80091618(HeapStats *out) {
    if (D_80140070 == 0) {
        return -1;
    }
    return func_80091650(D_80140070, out);
}
