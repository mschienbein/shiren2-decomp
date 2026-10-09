#include "common.h"

/* Enable flag shared by the paired start/stop routines; owned by this file. */
s32 D_80138BD0 = 1;
void func_80051E78(void);
void func_80051E8C(void);
void func_80045AAC(void);

void func_80045A60(void)
{
    D_80138BD0 = 1;
    func_80051E78();
}

void func_80045A84(void)
{
    D_80138BD0 = 0;
    func_80051E8C();
    func_80045AAC();
}
