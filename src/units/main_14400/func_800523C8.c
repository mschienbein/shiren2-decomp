#include "common.h"

extern unsigned char D_8016166E;
extern void func_8012A2F8(s32 flags, s32 value);
extern s32 func_8012A3BC(s32 flags);
extern void func_800533B8(void);
extern void func_800533DC(void);
extern void func_80052CF8(void);

void func_800523C8(void)
{
    if (D_8016166E == 0) {
        do {
            func_8012A2F8(2, 1);
        } while (func_8012A3BC(2) != 0);
        func_800533B8();
        func_800533DC();
        func_80052CF8();
    }
}
