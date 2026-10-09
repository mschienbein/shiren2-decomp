#include "common.h"

typedef struct Unit Unit;

extern Unit *D_801476B8;
u32 func_800E8D90(Unit *unit);

s32 func_80041AA0(void)
{
    return (short)func_800E8D90(D_801476B8);
}
