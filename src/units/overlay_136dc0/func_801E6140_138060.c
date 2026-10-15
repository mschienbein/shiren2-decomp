#include "func_801E6140_138060/types.h"

void func_801E6140_138060(s32 index)
{
    OvlRecord *p = &D_801F6764_148684[index];
    if (p->b05) {
        func_80076044(index, 0, 1, 8, 3);
        func_80075E5C(index, 0);
    }
    p->h2E[0] = 0;
    p->h2A = 0;
    p->b05 = 0;
}
