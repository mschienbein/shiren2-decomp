#include "common.h"

s32 func_80028440(void)
{
    u32 status = *(volatile u32 *)0xA410000C;

    if (status & 0x100) {
        return 1;
    }
    return 0;
}
