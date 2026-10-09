#include "common.h"

/* Busy flag for the shared static work buffer; owned by this file. */
s32 D_80138B10 = 0;
extern unsigned char D_80160A70[];

/* The size argument is not read: the static buffer is always returned. */
void *func_800442E0(s32 unusedSize)
{
    D_80138B10 = 1;
    return D_80160A70;
}

/* The object argument (passed by destructors such as func_800445B4) is not read:
 * releasing the single static buffer only clears the busy flag. */
void func_800442FC(void *obj)
{
    D_80138B10 = 0;
}
