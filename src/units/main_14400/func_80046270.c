#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern u8 D_80138BF4[];
void func_800265E0(void *dst, s32 size);

void func_80046270(void) {
    func_800265E0(D_80138BF4, 0x48);
}
