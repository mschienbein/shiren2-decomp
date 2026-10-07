#include "common.h"

/* local-arithmetic-qualification: slots initially hold file-relative offsets.
 * The in-bounds form (char *)base + (s32)*slots emits addu v0,a1,v0,
 * whereas 8012C314 uses addu v0,v0,a1. Keep the offset-first integer
 * addition local; the bank argument and relocated stored slots remain pointers. */
void func_8012C300(void **slots, void *base, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++, slots++) {
        if (*slots != 0) {
            *slots = (void *)((s32)*slots + (s32)base);
        }
    }
}
