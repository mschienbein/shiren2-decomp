#include "common.h"
typedef unsigned short u16;
extern u16 D_80156A68;
s32 func_80049CB4(s32 id, ...);
u16 func_80115944(void *owner, u16 value);
void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
/* Trap slot +0x44 supplies a2 (source position), a4 (direction) and a6 (item), unused here. */
s32 func_80125638(void *a0, void *a1, void *a2, void *a3, void *a4, void *a5, void *a6) {
    func_80049CB4(0xF1, a3);
    if (a5 != 0) {
        func_80049CB4(0x41, a5);
        func_800A7B18(a5, a1, (u16)func_80115944(a0, D_80156A68), 0xF);
    }
    return 1;
}
