#include "func_801E7074_138F94/types.h"

void func_801E7074_138F94(s32 index, s32 from, s32 to, s32 delay)
{
    OvlRecord *p = &D_801F6764_148684[index];
    if (from != to) {
        if (from < to) {
            if (to - from < 5) p->b12 = 1;
            else p->b12 = -1;
        } else {
            if (to - from < -4) p->b12 = 1;
            else p->b12 = -1;
        }
        p->b07 = delay;
        p->b06 = delay;
        p->b13 = to;
    }
}
