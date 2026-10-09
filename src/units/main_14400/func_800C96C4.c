#include "common.h"

typedef unsigned char u8;
extern void func_80048ABC(u32 value);
extern void func_80045DC4(void);
extern s32 D_801F5D04;
extern u8 D_801476C2;

void func_800C96C4(void) {
    func_80048ABC(0);
    func_80045DC4();
    D_801F5D04 = 0;
    D_801476C2 = 1;
}
