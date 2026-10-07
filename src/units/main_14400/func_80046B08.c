#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

s32 func_80072D54(void);
void func_8008BBC0(s32 flag);
void func_80046B08(s32 value) {
    if (func_80072D54() == 0) {
        func_8008BBC0(value == 0);
    }
}
