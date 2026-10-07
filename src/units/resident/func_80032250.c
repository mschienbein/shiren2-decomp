#include "common.h"

s32 func_80032250(void)
{
    return (*(volatile u32 *)0xA4800018 & 3) != 0;
}
