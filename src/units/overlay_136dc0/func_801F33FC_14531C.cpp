#include "func_801F33FC_14531C/types.h"


extern "C" void func_801F33FC_14531C(void)
{
    s32 i;

    for (i = 30;;) {
        i--;
        if (i == -1) {
            break;
        }
        if (D_801F51E8_147108->pairs.units[i] == NULL) {
            continue;
        }
        OvlBDir direction(func_801F49BC_1468DC(D_801F51E8_147108->pairs.units[i], D_801F51E8_147108->pairs.targets[i]));
        func_800A665C(D_801F51E8_147108->pairs.units[i], &direction);
        func_80049CB4(2);
    }
}
