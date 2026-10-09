#include "common.h"

typedef unsigned char u8;
extern s32 func_800A3A20(u8 kind);
extern s32 func_800A3A7C(u32 kind);

s32 func_800A3A8C(u8 kind)
{
    s32 result = 0;
    if (func_800A3A20(kind) || func_800A3A7C(kind))
        result = 1;
    return result;
}
