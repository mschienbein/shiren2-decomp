#include "func_801E6888_1387A8/types.h"

void func_801E6888_1387A8(s32 index, s32 resource, s32 value, s32 mode, s32 state)
{
    u8 modes[3] = {0, 1, 3};
    u8 states[4] = {5, 0, 1, 5};
    OvlRecord *p = &D_801F6764_148684[index];
    func_80076044(index, resource, 1, value, modes[mode]);
    func_80075E5C(index, states[state]);
    p->b18 = 1;
}
