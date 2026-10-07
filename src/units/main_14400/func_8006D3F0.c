#include "common.h"

typedef unsigned char u8;

extern u8 D_801A70E4;
extern u8 D_801A70E5;
extern s32 D_801A70EC;

void func_8006D3F0(void)
{
    D_801A70E4 = 1;
    D_801A70E5 = 1;
    D_801A70EC = -1;
}
