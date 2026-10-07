#include "common.h"

s32 func_80032850(u32 pc)
{
    u32 status = *(volatile u32 *)0xA4040010;

    if (!(status & 1)) {
        return -1;
    }
    *(volatile u32 *)0xA4080000 = pc;
    return 0;
}
