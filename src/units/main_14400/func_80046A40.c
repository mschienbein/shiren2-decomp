#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern u16 D_801F5D0E;
s32 func_80072D54(void);
s32 func_800610A8(void);
void func_8008BCF4(s32 arg);

void func_80046A40(s32 arg) {
    if (func_80072D54() == 0) {
        if (func_800610A8() == 0 || D_801F5D0E == 0) {
            func_8008BCF4(arg);
        }
    }
}
