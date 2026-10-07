#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern u16 D_801569CE;
s32 func_800E1CC4(void *target, s32 kind);
/* The helper receives the slot's receiver pointer but does not use it. */
void func_80117B00(void *a, void *b, void *target, s16 amount);

void func_80117CAC(void *a, void *b, void *target) {
    s32 scale;

    if (func_800E1CC4(target, 3) != 0) {
        scale = 2;
    } else {
        scale = 1;
    }
    func_80117B00(a, b, target, D_801569CE * scale);
}
