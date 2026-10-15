#include "func_801E70F8_139018/types.h"

void func_801E70F8_139018(void) {
    s32 i = 30;
    s32 enabled = func_80041FF8();
    OvlRecord *p = &D_801F6764_148684[i];
    s32 next;

    if ((u8)enabled) {
        while (--i != -1) {
            --p;
            if (p->b12 == 0) {
                continue;
            }
            if (p->b06 != 0 && --p->b06 != 0) {
                continue;
            }
            next = func_8004208C(i, p->b12, p->b13);
            if (next == -1) {
                func_800766A8(i, p->b13);
                p->b12 = 0;
            } else {
                func_800766A8(i, next);
                p->b06 = p->b07;
            }
        }
    }
}
