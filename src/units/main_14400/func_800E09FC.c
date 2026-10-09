#include "common.h"
extern u32 D_8013960C;
extern unsigned short func_800E08F0(void *object);
extern s32 func_800E0534(void *object, s32 amount);
void func_800E09FC(void *object, s32 *accumulator, unsigned short threshold) {
    s32 count = 0;
    *accumulator += func_800E08F0(object);
    while (*accumulator >= threshold) { *accumulator -= threshold; count++; }
    if (count > 0) {
        D_8013960C <<= 1;
        func_800E0534(object, count);
        D_8013960C >>= 1;
    }
}
