#include "func_801E8590_13A4B0/types.h"
extern void func_800598B4(u8 a, u8 b, u8 c, u8 d);
extern void func_80059894(s32 value);
void func_801E8590_13A4B0(s32 a, s32 b, s32 c, s32 d) {
    if (!a) func_80059894(0);
    else {
        func_800598B4(a - 10, b - 10, c - 10, d - 10);
        func_80059894(1);
    }
}
