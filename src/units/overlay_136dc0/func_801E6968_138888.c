#include "func_801E6968_138888/types.h"

void func_801E6968_138888(void)
{
    s32 i;
    OvlRecord *p = &D_801F6764_148684[30];
    for (i = 29; i != -1; i--) {
        --p;
        if (p->b18) {
            func_80076044(i, 0, 1, 0, 3);
            p->b18 = 0;
        }
    }
}
