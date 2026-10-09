#include "common.h"
typedef unsigned short u16;
extern u16 D_8013E822;
u16 D_8013E824 = 2;

void func_800827C8(s32 arg0) {
    switch (arg0) {
    case 8: D_8013E822 = 5; break;
    case 10: D_8013E822 = 4; break;
    case 11: D_8013E822 = 5; break;
    case 15: D_8013E822 = 2; break;
    case 22: D_8013E822 = 15; break;
    case 24: D_8013E822 = 13; break;
    case 26: D_8013E822 = 12; break;
    case 31: D_8013E822 = 10; break;
    case 28: D_8013E822 = 11; break;
    default: D_8013E822 = 3; break;
    }
    D_8013E824 = D_8013E822;
}
