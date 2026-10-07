#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern s32 D_80148090;
void func_800D5A30(u8 *obj);
void func_800D5BF0(u8 *obj, u8 *arg1, u8 *arg2, s32 arg3);
void func_800D72E0(u8 *obj, s32 mode) {
    D_80148090 = mode;
    switch (mode) {
    case 1:
        func_800D5A30(obj);
        break;
    case 2:
    case 3:
        func_800D5BF0(obj, obj, obj + 8, 0);
        break;
    }
}
