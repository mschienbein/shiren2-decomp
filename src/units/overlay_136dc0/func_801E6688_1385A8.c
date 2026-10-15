#include "func_801E6688_1385A8/types.h"

void func_801E6688_1385A8(s32 restore)
{
    s32 i;
    if (restore) {
        i = D_801F6F0A_148E2A;
        for (i--; i != -1; i--) {
            OvlCell *p = &D_801F7060_148F80[i];
            func_800625B8(p->x, p->y, p->value);
        }
    }
    D_801F6F0A_148E2A = 0;
}
