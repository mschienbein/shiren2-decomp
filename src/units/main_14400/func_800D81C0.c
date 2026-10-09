#include "common.h"

extern unsigned char D_80148190[];

s32 func_800D81C0(s32 id)
{
    return D_80148190[id] - 1;
}
