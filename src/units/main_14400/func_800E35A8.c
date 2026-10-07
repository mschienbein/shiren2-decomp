#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

u16 func_800E08B0(void *obj);
void func_800A7B18(void *obj, void *source, s32 count, s32 extra);
void func_800E35A8(void *obj, void *source, u8 percent, s32 extra, s32 keepAll) {
    s32 count = func_800E08B0(obj) * percent / 100;
    if (count <= 0) {
        count = 1;
    }
    if (!keepAll && func_800E08B0(obj) - count <= 0) {
        count = func_800E08B0(obj) - 1;
    }
    if (count > 0) {
        func_800A7B18(obj, source, count, extra);
    }
}
