#include "common.h"

typedef unsigned char u8;

extern u8 D_8016166E;

void func_8005312C(s32, u8);

void func_800526B0(u8 a, u8 b)
{
    if (D_8016166E == 0) {
        func_8005312C(a, b);
    }
}
