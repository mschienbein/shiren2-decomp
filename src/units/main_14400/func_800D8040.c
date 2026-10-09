#include "common.h"

typedef unsigned char u8;

extern u8 D_80148190[];

extern s32 func_800D7860(s32 index);

s32 func_800D8040(s32 id) {
    s32 result;

    if ((u32)id >= 0xA3) {
        return 0;
    }
    result = 0;
    if (D_80148190[id] != 0) {
        result = func_800D7860(id) == 0;
    }
    return result;
}
