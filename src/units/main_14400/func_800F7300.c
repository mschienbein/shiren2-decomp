#include "common.h"

typedef unsigned char u8;

s32 func_800F426C(void *, s32, s32, u8, s32);
s32 func_800E0F40(u8 *);
s32 func_800F7300(u8 *self, s32 a1, s32 kind, u8 a3, s32 a4) {
    s32 ret = func_800F426C(self, a1, kind, a3, a4);

    if (kind == 8) {
        if ((u8)func_800E0F40(self) == 1) {
            *(s32 *)(self + 0xA8) = 0;
        }
    }
    return ret;
}
