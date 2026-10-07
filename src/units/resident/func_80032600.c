#include "common.h"

/* libultra __osSiRawWriteIo */

#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_WRITE(addr, data) (*(volatile u32 *)PHYS_TO_K1(addr) = (u32)(data))

s32 func_80032250(void);

s32 func_80032600(u32 devAddr, u32 data)
{
    if (func_80032250()) {
        return -1;
    }
    IO_WRITE(devAddr, data);
    return 0;
}
