#include "common.h"

typedef unsigned long long u64;

/* osDpSetNextBuffer */

extern s32 func_80028440(void);     /* __osDpDeviceBusy */
extern u32 func_800340F0(void *va); /* osVirtualToPhysical */

#define DPC_START_REG  (*(volatile u32 *)0xA4100000)
#define DPC_END_REG    (*(volatile u32 *)0xA4100004)
#define DPC_STATUS_REG (*(volatile u32 *)0xA410000C)

s32 func_80028460(void *bufPtr, u64 size)
{
    if (func_80028440()) {
        return -1;
    }
    DPC_STATUS_REG = 1;
    do {
    } while (DPC_STATUS_REG & 1);
    DPC_START_REG = func_800340F0(bufPtr);
    DPC_END_REG = func_800340F0(bufPtr) + size;
    return 0;
}
