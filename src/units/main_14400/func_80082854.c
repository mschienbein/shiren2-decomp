#include "common.h"
typedef signed short s16;
extern s16 D_801A9F58;
extern s16 D_801A9F5A;
void func_80082854(s32 first, s32 second) {
    if (first < 0) first = 0;
    D_801A9F58 = first;
    D_801A9F5A = second;
}
