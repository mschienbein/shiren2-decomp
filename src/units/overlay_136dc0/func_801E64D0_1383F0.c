#include "func_801E64D0_1383F0/types.h"

void func_801E64D0_1383F0(s32 value, s32 x0, s32 y0, s32 x1, s32 y1)
{
    OvlCell *p = &D_801F7060_148F80[D_801F6F0A_148E2A];
    s32 x, y;
    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            p->value = func_80062554(x, y);
            p->x = x;
            p->y = y;
            if (++D_801F6F0A_148E2A >= 100) D_801F6F0A_148E2A--;
            else p++;
            func_800625B8(x, y, value);
        }
    }
}
