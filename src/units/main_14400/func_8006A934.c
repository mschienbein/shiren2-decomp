#include "common.h"

typedef unsigned char u8;

extern s32 D_8013CA20;
/* Bump-allocator cursor and end of the arena (shared with func_8006A8D8). */
extern u8 *D_8016FD64;
extern u8 *D_8016FD68;

/* name: caller's debug tag string (e.g. "Audio buff"); never read by this allocator.
 * Returns a block of size rounded up to 16 bytes, or null when disabled or exhausted. */
void *func_8006A934(char *name, u32 size)
{
    u8 *cur;

    size = (size + 15) & ~15;
    if (D_8013CA20 == 0) {
        return 0;
    }
    /* local-arithmetic-qualification: the alignment test reads the cursor's low address
     * bits, which only an integer view of the pointer exposes; the cursor stays a pointer. */
    if ((u32)D_8016FD64 & 0xF) {
        D_8016FD64 += 8;
    }
    if (size < (u32)(D_8016FD68 - D_8016FD64)) {
        cur = D_8016FD64;
        D_8016FD64 = cur + size;
    } else {
        cur = 0;
    }
    return cur;
}
