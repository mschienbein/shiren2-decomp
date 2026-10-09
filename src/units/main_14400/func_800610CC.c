#include "common.h"
/* Eight display-list pointers (set by func_800611F8, emitted by func_800610F4). */
extern void *D_80169A18[8];
void func_800610CC(void)
{
    s32 i = 7;
    s32 offset = 7 * sizeof(void *);
    /* Clears slots 7..0; offset is the byte offset of slot i within the array. */
    for (; i >= 0; i--, offset -= sizeof(void *)) {
        *(void **)((unsigned char *)D_80169A18 + offset) = 0;
    }
}
