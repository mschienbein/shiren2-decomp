#include "common.h"

typedef unsigned short u16;
extern void *D_801476B8;
extern u16 func_800E8D0C(void *obj);
s32 func_80041A74(void)
{
    return (short)func_800E8D0C(D_801476B8);
}
