#include "common.h"

typedef unsigned char u8;
s32 func_800610A8(void);
u8 func_801E99C4(u8, s32);

u8 func_800424EC(s32 id)
{
    if (!func_800610A8()) {
        return 0;
    }
    return func_801E99C4(id, 1);
}
