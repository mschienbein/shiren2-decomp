#include "common.h"

typedef unsigned short u16;

u16 func_800E08B0(void *obj);
u16 func_800E08F0(void *obj);

s32 func_800E9390(void *obj) {
    u16 limit = func_800E08B0(obj);
    u16 value = func_800E08F0(obj);

    return limit * 4 <= value;
}
