#include "common.h"

#define PI_DRAM_ADDR_REG 0xA4600000
#define PI_CART_ADDR_REG 0xA4600004
#define PI_RD_LEN_REG 0xA4600008
#define PI_WR_LEN_REG 0xA460000C
#define PI_STATUS_REG 0xA4600010
#define IO_READ(addr) (*(volatile u32 *)(addr))
#define IO_WRITE(addr, data) (*(volatile u32 *)(addr) = (u32)(data))

extern u32 D_80000308; /* osRomBase */
u32 func_800340F0(void *addr); /* osVirtualToPhysical */

s32 func_8002F950(s32 direction, u32 devAddr, void *dramAddr, u32 size)
{
    register u32 stat;

    stat = IO_READ(PI_STATUS_REG);
    while (stat & 3) {
        stat = IO_READ(PI_STATUS_REG);
    }
    IO_WRITE(PI_DRAM_ADDR_REG, func_800340F0(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, (D_80000308 | devAddr) & 0x1FFFFFFF);
    switch (direction) {
    case 0:
        IO_WRITE(PI_WR_LEN_REG, size - 1);
        break;
    case 1:
        IO_WRITE(PI_RD_LEN_REG, size - 1);
        break;
    default:
        return -1;
    }
    return 0;
}
