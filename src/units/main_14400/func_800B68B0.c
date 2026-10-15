#include "map_rect.h"

typedef struct { MapRect bounds; unsigned char counts10[4]; } Obj800B6C14;

s32 func_800B68B0(Obj800B6C14 *obj)
{
    s32 total = 0;
    s32 i;
    for (i = 0; i < 4; i++) {
        total += obj->counts10[i];
    }
    return total;
}
