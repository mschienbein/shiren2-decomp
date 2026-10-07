#include "common.h"

/* libultra __osSpRawStartDma */

#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_WRITE(addr, data) (*(volatile u32 *)PHYS_TO_K1(addr) = (u32)(data))

#define SP_MEM_ADDR_REG 0x04040000
#define SP_DRAM_ADDR_REG 0x04040004
#define SP_RD_LEN_REG 0x04040008
#define SP_WR_LEN_REG 0x0404000C

s32 func_80032700(void);
u32 func_800340F0(void *p);

s32 func_80032730(s32 direction, u32 devAddr, void *dramAddr, u32 size)
{
    if (func_80032700()) {
        return -1;
    }
    IO_WRITE(SP_MEM_ADDR_REG, devAddr);
    IO_WRITE(SP_DRAM_ADDR_REG, func_800340F0(dramAddr));
    if (direction == 0) {
        IO_WRITE(SP_WR_LEN_REG, size - 1);
    } else {
        IO_WRITE(SP_RD_LEN_REG, size - 1);
    }
    return 0;
}
