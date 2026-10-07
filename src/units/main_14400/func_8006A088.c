#include "common.h"

typedef unsigned char u8;

extern u8 D_8013CA00;
extern u8 D_8013CA01;
extern u8 D_8013CA02;
extern u8 D_8013CA03;

void func_8006A088(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    D_8013CA00 = arg0;
    D_8013CA01 = arg1;
    D_8013CA02 = arg2;
    D_8013CA03 = arg3;
}
