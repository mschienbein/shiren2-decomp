#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void func_800E11C0(void *obj, s32 a, u8 b, s32 c);
void func_800E18C0(void *obj, s32 a, u8 b, s32 c);
s32 func_800E1C58(void *obj, s32 a);
s32 func_800E115C(void *obj, s32 mode, s32 a, u8 b, s32 c) {
    switch (mode) {
    case 0:
        func_800E11C0(obj, a, b, c);
        return 1;
    case 1:
        func_800E18C0(obj, a, b, c);
        return 1;
    default:
        return func_800E1C58(obj, a);
    }
}
