#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

s32 func_800E1CC4(void *, s32);
void func_800B1CEC(s32, void *);
void func_800B2DC4(void *);
void func_800E2298(void *arg0) {
    if (func_800E1CC4(arg0, 0) != 0) {
        func_800B1CEC(1, arg0);
    } else {
        func_800B2DC4(arg0);
    }
}
