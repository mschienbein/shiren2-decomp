#include "common.h"

typedef struct { unsigned char pad00[0x10]; unsigned char counts10[4]; } Obj800B6C14;

s32 func_800B68B0(Obj800B6C14 *obj)
{
    s32 total = 0;
    s32 i;
    for (i = 0; i < 4; i++) {
        total += obj->counts10[i];
    }
    return total;
}
