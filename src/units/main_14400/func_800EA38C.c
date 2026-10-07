#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void func_800EA3F0(void *, s32, u8, s32);
void func_800E18C0(void *, s32, u8, s32);
s32 func_800E1C58(void *, s32);
s32 func_800EA38C(void *arg0, s32 type, s32 arg2, u8 arg3, s32 arg4) {
    switch (type) {
    case 0:
        func_800EA3F0(arg0, arg2, arg3, arg4);
        return 1;
    case 1:
        func_800E18C0(arg0, arg2, arg3, arg4);
        return 1;
    default:
        return func_800E1C58(arg0, arg2);
    }
}
