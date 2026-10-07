#include "common.h"

u32 func_800B1C6C(void *pos);
s32 func_800A58B8(void *p);
s32 func_800A6E90(void *p) {
    s32 r = 0;
    if (func_800B1C6C(p) & 0x80) r = func_800A58B8(p) == 1;
    return r;
}
