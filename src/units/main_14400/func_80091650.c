#include "common.h"
typedef unsigned char u8;
typedef struct Header Header;
/* Block header of the func_800913F4 heap (see func_80091488/func_80091570);
 * the first header's +0xC records the extent of the complete allocation. */
struct Header { s32 occupied; s32 size; Header *next; u32 extent; };
/* Seven-word output record (func_80091618's HeapStats). */
typedef struct { s32 total, used, used_max, used_min, free, free_max, free_min; } Stats;
s32 func_80091650(void *heap, Stats *stats) {
    s32 used = 0;
    s32 free_bytes = 0, count = 0;
    Header *node = heap;
    Header *next;
    u32 extent;
    u8 *end;
    stats->free_max = 0;
    stats->used_max = 0;
    stats->used_min = stats->free_min = node->extent;
    extent = node->extent;
    if (!extent) return -1;
    end = (u8 *)node + (extent / 4) * 4;
    for (;;) {
        s32 size = node->size;
        if (!node->occupied && size) {
            free_bytes += size;
            if (stats->free_max < size) stats->free_max = size;
            if (size < stats->free_min) stats->free_min = size;
        } else {
            used += size;
            if (stats->used_max < size) stats->used_max = size;
            if (size < stats->used_min) stats->used_min = size;
        }
        next = node->next;
        count++;
        if (!next) break;
        /* local-arithmetic-qualification: the low pointer bits are inspected for alignment. */
        if ((u8 *)next < (u8 *)heap || end < (u8 *)next || ((u32)next & 3)) return extent;
        node = next;
    }
    stats->total = used + free_bytes + count * 16;
    stats->used = used;
    stats->free = free_bytes;
    if (!used) stats->used_min = 0;
    if (!free_bytes) stats->free_min = 0;
    /* ODD_C: the cursor is pointed back at the first header for the final
     * extent check. Its reference here is the last use of `node` in
     * reg_scan order, so cse.c make_regs_eqv keeps `node` (not `heap`) as
     * the canonical register of the entry copy and the opening reads stay
     * `lw 12(a3)`. Reading ((Header *)heap)->extent directly makes `heap`
     * canonical: the entry copy folds away and the cursor coalesces with
     * `next` (68 words, 280 B); a separate `first` copy gives 37 words. */
    node = heap;
    if (stats->total != node->extent) return extent;
    return 0;
}
