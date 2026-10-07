#include "common.h"

typedef unsigned char u8;

s32 func_800D7D84(u8 a, u8 b);
s32 func_800D8040(s32 id);

s32 func_800D8088(u8 a, u8 b) {
    return func_800D8040(func_800D7D84(a, b));
}
