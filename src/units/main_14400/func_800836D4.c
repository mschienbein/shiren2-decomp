#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern s32 D_8013E830;
s32 func_8008269C(s32 a);
void func_800835AC(u32 index);
void func_800836D4(s32 v) {
    if (v != D_8013E830) {
        s32 r = func_8008269C(1);
        D_8013E830 = v;
        func_800835AC(r);
    }
}
