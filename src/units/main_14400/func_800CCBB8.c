#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_800CBD2C(void);
s32 func_800CBE44(void);
void func_800CBEBC(u8 arg0, u8 arg1);
void func_800CBF00(void *buf);
void func_800CC248(void);
s32 func_800CCBB8(u8 arg0, u8 arg1, void *arg2) {
    s32 ok = 1;
    if (func_800CBD2C() != 3) {
        ok = 0;
    } else {
        s32 notReady = func_800CBE44() != 1;
        if (notReady) {
            ok = 0;
        }
    }
    if (ok) {
        func_800CBEBC(arg0, arg1);
        func_800CBF00(arg2);
    }
    func_800CC248();
    return ok;
}
