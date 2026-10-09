#include "common.h"

extern s32 D_8016DB18;
extern s32 D_801E4E70;
extern s32 D_801D2C20;

s32 func_800610BC(void);
s32 func_80042A68(void);
void func_800418FC(s32 *outA, s32 *outB);
void func_80061820(s32, s32);

void func_8006179C(void)
{
    s32 a;
    s32 b;

    if (func_800610BC() != 10 && D_8016DB18 == 3 && func_80042A68() != 0) {
        D_801E4E70 = 1;
    } else {
        D_801E4E70 = 0;
    }
    D_801D2C20 = 1;
    func_800418FC(&a, &b);
    func_80061820(a, b);
}
