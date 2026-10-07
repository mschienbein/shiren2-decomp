#include "common.h"
typedef unsigned char u8;

/* Complete 0x10-byte heap view. The first two words are base and cursor;
 * keeping them in one pointer array permits the original cursor-to-base step. */
typedef struct {
    u8 *bounds[2];
    s32 len;
    s32 count;
} ALHeap;
extern ALHeap D_801CA940;

s32 func_8012D87C(void) {
    u8 **p = &D_801CA940.bounds[1];
    /* Preserve the original cursor-first load and reuse its +4 address for
     * the base load; ordinary reads let GCC choose a different load order. */
    u8 *end = *(u8 *volatile *)p;
    return end - *--p;
}
