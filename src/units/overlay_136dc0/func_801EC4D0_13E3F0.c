#include "func_801EC4D0_13E3F0/types.h"
extern void func_80045A24(s32 sound);

void func_801EC4D0_13E3F0(void)
{
    if (!D_801F51E8_147108->field992) {
        func_80045A24(0x1A5);
        D_801F51E8_147108->field992 = 1;
    }
}
