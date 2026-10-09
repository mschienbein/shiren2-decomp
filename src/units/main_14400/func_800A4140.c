#include "common.h"
s32 func_800A4140(s32 flags, s32 state) {
    if (flags & 0xC100) return 0;
    if (flags & 0x2000) return state == 1;
    return 1;
}
