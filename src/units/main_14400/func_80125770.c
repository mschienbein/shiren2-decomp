#include "common.h"
typedef unsigned short u16;
extern u16 D_80156A6A;
s32 func_80049CB4(s32, ...);
u16 func_80115944(void *, u16);
void func_800A7B18(void *, void *, s32, s32);
/* Trap slot +0x44 supplies a2 (source position), a4 (direction) and a6 (item), unused here. */
s32 func_80125770(void *a0, void *a1, void *a2, void *a3, void *a4, void *a5, void *a6) {
    func_80049CB4(0xE7, a3);
    if (a5 != 0) {
        func_80049CB4(0x41, a5);
        func_800A7B18(a5, a1, (u16)func_80115944(a0, D_80156A6A), 14);
    }
    return 1;
}
