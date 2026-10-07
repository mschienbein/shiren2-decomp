#include "common.h"

void func_80032880(u32 data)
{
    *(volatile u32 *)0xA4040010 = data;
}
