#include "common.h"

typedef unsigned char u8;

extern void *func_800A8CB0(s32 cell);
extern s32 func_800A6E90(void *obj);

s32 func_80042A2C(u8 id) {
    return func_800A6E90(func_800A8CB0(id));
}
