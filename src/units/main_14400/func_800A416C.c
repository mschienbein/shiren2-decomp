#include "common.h"
s32 func_800A416C(s32 flags, s32 mode)
{
    if (flags & 0xC100) return 0;
    if (flags & 0x2000) return mode == 1;
    return 1;
}
