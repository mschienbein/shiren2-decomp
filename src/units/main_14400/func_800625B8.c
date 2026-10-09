#include "common.h"
extern float *D_8013B804;
void func_800625B8(s32 x, s32 y, s32 value) {
    x -= 10;
    y -= 10;
    if (D_8013B804 != 0) D_8013B804[y * 56 + x] = value;
}
