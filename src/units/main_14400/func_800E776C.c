#include "common.h"

typedef unsigned char u8;

extern void *D_801476B8;
s32 func_800E74E0(void *arg0, void *arg1, s32 arg2);
s32 func_800E776C(void *arg0, u8 arg1) {
    return func_800E74E0(arg0, D_801476B8, arg1);
}
