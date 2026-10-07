#include "common.h"

s32 func_80025EA0(void)
{
    s32 status = *(volatile u32 *)0xA450000C;

    if (status & (1 << 31)) {
        return 1;
    }
    return 0;
}
