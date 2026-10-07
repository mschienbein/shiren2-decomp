#include "common.h"

/* libultra __osSiRawReadIo */

#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_READ(addr) (*(volatile u32 *)PHYS_TO_K1(addr))

s32 func_80032250(void);

s32 func_800325B0(u32 devAddr, u32 *data)
{
    if (func_80032250()) {
        return -1;
    }
    *data = IO_READ(devAddr);
    return 0;
}
