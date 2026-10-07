#include "common.h"

s32 func_80032700(void)
{
    return (*(volatile u32 *)0xA4040010 & 0x1C) != 0;
}
