#include "func_801E6BA0_138AC0/types.h"

void func_801E6BA0_138AC0(s32 owner)
{
    s32 i;
    for (i = 7; i != -1; i--) {
        OvlEffect *p = &D_801F6EAA_148DCA[i];
        if (p->b02 && p->b01 == owner) {
            p->b03 = 0;
            func_8008C194(p->b02 - 1);
            p->b02 = 0;
            if (p->b01 != 255) func_801E6394_1382B4(p->b01, 0);
            p->b01 = 0;
        }
    }
}
