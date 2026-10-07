#include "common.h"

/* libultra __osSiRawStartDma */

#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_READ(addr) (*(volatile u32 *)PHYS_TO_K1(addr))
#define IO_WRITE(addr, data) (*(volatile u32 *)PHYS_TO_K1(addr) = (u32)(data))

#define SI_DRAM_ADDR_REG 0x04800000
#define SI_PIF_ADDR_RD64B_REG 0x04800004
#define SI_PIF_ADDR_WR64B_REG 0x04800010
#define SI_STATUS_REG 0x04800018
#define PIF_RAM_START 0x1FC007C0

void func_80034720(void *p, s32 len);
void func_8002B000(void *p, s32 len);
u32 func_800340F0(void *p);

s32 func_80032500(s32 direction, void *dramAddr)
{
    if (IO_READ(SI_STATUS_REG) & 3) {
        return -1;
    }
    if (direction == 1) {
        func_80034720(dramAddr, 64);
    }
    IO_WRITE(SI_DRAM_ADDR_REG, func_800340F0(dramAddr));
    if (direction == 0) {
        IO_WRITE(SI_PIF_ADDR_RD64B_REG, PIF_RAM_START);
    } else {
        IO_WRITE(SI_PIF_ADDR_WR64B_REG, PIF_RAM_START);
    }
    if (direction == 0) {
        func_8002B000(dramAddr, 64);
    }
    return 0;
}
