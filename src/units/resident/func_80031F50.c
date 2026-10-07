#include "common.h"

/* libultra __osSetGlobalIntMask */

extern u32 D_80037260; /* __OSGlobalIntMask */

u32 func_8002AF70(void);
void func_8002AFE0(u32 mask);

void func_80031F50(u32 mask)
{
    u32 saveMask = func_8002AF70();

    D_80037260 |= mask;
    func_8002AFE0(saveMask);
}
