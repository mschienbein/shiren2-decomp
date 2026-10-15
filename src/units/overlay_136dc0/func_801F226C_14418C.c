#include "func_801F226C_14418C/types.h"

/* Main callers pass 0/1 at 0x800C6BC0/0x800C75D8; this body ignores it. */
void func_801F226C_14418C(s32 unused) {
    s16 ids[23] = {
        0x2CD, 0x16, 0x2D0, 0x2D9, 0x2D5, 0x17, 0x49E, 0x4A1, 0x49F, 0x19A, 0x19E, 0xE8,
        0x1A6, 0x1AC, 0x1B2, 0x1BA, 0x5F, 0x6A, 0x215, 0x4A2, 0x4A0, 0x2E7, 0x2F3,
    };
    s32 i;

    (void)unused;
    for (i = 22; i != -1; i--) {
        if (D_801F5D10_147C30 == ids[i]) {
            D_801F5D08_147C28 = 0;
            func_801F212C_14404C(D_801F5D10_147C30, 0);
            func_80094FA8(D_80140160);
            D_801F5D10_147C30 = 0;
            return;
        }
    }
}
