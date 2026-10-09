#include "common.h"
s32 func_800A4124(s32 flags, s32 mode)
{
    if (mode == 2) return (flags & 0xE100) == 0;
    return 0;
}
