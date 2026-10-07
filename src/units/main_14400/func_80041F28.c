#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_800610A8(void);
void func_801E9F90(s32 arg0, u8 arg1);
void func_80041F28(s32 arg0, u8 arg1) {
    if (func_800610A8() != 0) {
        func_801E9F90(arg0, arg1);
    }
}
